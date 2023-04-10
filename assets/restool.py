import sys
import plm_decode, projectile_decode, eproj_decode, animtiles_decode, palfx_decode, enemy_instr_decode
import consts 
import text_lexer
import re
from decode_common import *
import glob
import inspect

plm_parser = plm_decode.PlmParser()
projectile_parser = projectile_decode.ProjParser()
eproj_parser = eproj_decode.EprojParser()
animtiles_parser = animtiles_decode.AnimtilesParser()
palfx_parser = palfx_decode.PalfxParser()
enemy_instr_parser = enemy_instr_decode.EnemyInstrParser()

kGlobalObjects = {}
def add_global(obj):
  kGlobalObjects[obj.__name__] = obj

class LinePrinter:
  def __init__(self):
    self.lines = []
    self.indent = 0
  def print(self, s):
    self.lines.append('  ' * self.indent + s)
  def add_indent(self):
    self.indent += 1
  def remove_indent(self):
    self.indent -= 1

class TUint8:
  def __init__(self, hex = False, signed = False):
    self.hex = hex
    self.signed = signed
  def parse_binary(self, ea, extra):
    v = ROM.get_byte(ea)
    if self.signed and v >= 0x80: v -= 0x100
    return 1, (v, self)
  def print_text(self, v):
    return '0x%.2x' % v if (self.hex and v > 0) else str(v)
  def is_default(self, v):
    return v == 0
  def write_binary(self, writer, v, extra):
    writer.write_byte((v or 0) & 0xff)

uint8 = TUint8()
uint8_hex = TUint8(hex = True)
int8 = TUint8(signed = True)

class Skip:
  def __init__(self, size = 1):
    self.size = size
  def parse_binary(self, ea, extra):
    return self.size, None
  def write_binary(self, writer, v, extra):
    assert (v or 0) == 0
    writer.write_byte(0)

class TUint16:
  def __init__(self, hex = False, signed = False):
    self.hex = hex
    self.signed = signed
  def parse_binary(self, ea, extra):
    return 2, (ROM.get_word(ea), self)
  def print_text(self, v):
    if self.signed and not self.hex:
      if v >= 0x8000: v -= 0x10000
    return '0x%.2x' % v if (self.hex and v > 0) else str(v)
  def is_default(self, v):
    return v == 0
  def write_binary(self, writer, v, extra):
    writer.write_word(v or 0)

uint16 = TUint16()
int16 = TUint16(signed = True)
uint16_hex = TUint16(hex = True)

class TLongPtr:
  def __init__(self, hex = False):
    self.hex = hex
  def parse_binary(self, ea, extra):
    return 3, (ROM.get_word(ea) + ROM.get_byte(ea + 2) * 65536, self)
  def print_text(self, v):
    return get_ea_name(v)
  def is_default(self, v):
    return v == 0
  def write_binary(self, writer, v, extra):
    writer.write_symbol_long(v or 0)
LongPtr = TLongPtr()

def get_bank(self, ea, extra):
  bank = self.__bank__ or extra.get('bank')
  if bank == None:
    raise Exception('Bank not specified %s for object at %s' % (self, get_ea_name(ea)))
  if bank == 'auto':
    bank = ea >> 16
  return bank

class CodePtr:
  def __init__(self, bank, unknown_prefix = 'unk', visitor = None):
    self.__bank__ = bank
    self.unknown_prefix = unknown_prefix
    self.visitor = visitor
  def parse_binary(self, ea, extra):
    bank = get_bank(self, ea, extra)
    v = ROM.get_word(ea)
    if v != 0:
      v += (bank << 16)
      if self.visitor: self.visitor(v, extra)
    return 2, (mark_address(v), self)
  def print_text(self, v):
    return get_ea_name(v, unknown_prefix = self.unknown_prefix)
  def is_default(self, v):
    return v == 0
  def write_binary(self, writer, v, extra):
    bank = get_bank(self, writer.get_ea(), extra)
    writer.write_symbol_word(v, bank)

class Fields:
  def __init__(self, tp, ea):
    self.tp = tp
    self.name = get_ea_name(ea) if ea else None
    self.fields = {}
    self.list = []
    self.args = None

  def print(self, out, compact = False):
    s = self.tp.__name__
    if self.args:
      s += '(%s)' % ', '.join(str(a) for a in self.args)
    if self.name != None:
      s += ' %s' % self.name
    if compact and len(self.list) == 0:
      s += ' {'
      r = []
      for k, v in self.fields.items():
        r.append(' %s: %s' % (k, v[1].print_text(v[0])))
      out.print(s + ','.join(r) + ' }')
    else:
      out.print(s + ' {')
      out.add_indent()
      for k, v in self.fields.items():
        out.print('%s: %s' % (k, v[1].print_text(v[0])))
      for v, tp in self.list:
        if not compact: out.print('')
        tp.print_actual_text(out, v)
      out.remove_indent()
      out.print('}')
  def set_args(self, args):
    self.args = args
  def add_list_entry(self, v, tp):
    self.list.append((v, tp))

class TLocationBased:
  __bank__ = None
  def parse_binary(self, ea, extra):
    bank = get_bank(self, ea, extra)
    v = ROM.get_word(ea)
    if v & 0x8000:
      v += (bank << 16)
      visit_location(v, self, extra)
    return 2, (v, self)
  def print_text(self, ea):
    return get_ea_name(ea)
  def write_binary(self, writer, v, extra):
    if v == None or isinstance(v, int):
      writer.write_word(v or 0)
    else:
      bank = get_bank(self, writer.get_ea(), extra)
      writer.write_symbol_word(v, bank)

class TLongLocationBased:
  __bank__ = None
  def parse_binary(self, ea, extra):
    v = ROM.get_long(ea)
    if v & 0x8000:
      visit_location(v, self, extra)
    return 3, (v, self)
  def print_text(self, ea):
    return get_ea_name(ea)
  def write_binary(self, writer, v, extra):
    writer.write_symbol_long(v or 0)

class EnableIf:
  def __init__(self, cond, tp):
    self.cond = cond
    self.tp = tp
  def parse_binary(self, ea, extra):
    if not (extra['enable_if'] & self.cond):
      return 0, None
    return self.tp.parse_binary(ea, extra)
  def write_binary(self, writer, v, extra):
    if v == None: return
    extra['enable_if'] |= self.cond
    self.tp.write_binary(writer, v, extra)

def parse_binary_fields(rv, ea, fields, extra, remove_default = False):
  assert (ea & 0x8000) != 0
  start_ea = ea
  for name, tp in fields.items():
    try:
      size, value = tp.parse_binary(ea, extra)
      if remove_default and value != None and hasattr(value[1], 'is_default') and value[1].is_default(value[0]):
        value = None
    except:
      print('Error parsing field %s at 0x%x of type %s' % (name, ea, tp))
      raise
    ea += size
    if value != None:
      rv.fields[name] = value
  return ea - start_ea

class TStruct(TLocationBased):
  __compact__ = False
  __remove_default__ = False
  __remove_name__ = False
  def parse_actual_binary(self, ea, extra):
    rv = Fields(self, None if self.__remove_name__ else ea)
    size = parse_binary_fields(rv, ea, self.__fields__, extra, self.__remove_default__)
    assert self.__size__ == None or size == self.__size__
    return size, (rv, self)
  def print_actual_text(self, out, v):
    v.print(out, self.__compact__)
  def write_actual_binary(self, writer, fields, extra):
    for name, tp in self.__fields__.items():
      tp.write_binary(writer, fields.dict.get(name), extra)
    for v in fields.list:
      write_global_object(writer, v, extra)

# Array of primitive objects
class TArray(TLocationBased):
  def __init__(self, size = None):
    self.size = size
  def parse_actual_binary(self, ea, extra):
    start_ea, rv = ea, []
    size = self.size if self.size != None else extra.get(self.__name__ + '_size')
    if size == None:
      raise Exception('Array size not specified')
    for i in range(size):
      size, value = self.__type__.parse_binary(ea, extra)
      ea += size
      rv.append(value)
    return ea - start_ea, ((get_ea_name(start_ea), rv), self)
  def get_array_size(self, extra):
    return 
  def print_actual_text(self, out, v):
    assert len(v) == 2
    inner = ', '.join(a[1].print_text(a[0]) for a in v[1])
    if v[0] == None:
      out.print('%s { %s }' % (self.__name__, inner))
    else:
      out.print('%s %s { %s }' % (self.__name__, v[0], inner))
  def write_actual_binary(self, writer, fields, extra):
    if len(fields.dict) != 0:
      writer.error('Array cannot have named fields')
    size = self.size if self.size != None else extra.get(self.__name__ + '_size')
    if size != None and len(fields.list) != size:
      writer.error('Array size mismatch')
    for value in fields.list:
      self.__type__.write_binary(writer, value, extra)

def MakeArray(tp, size, name, bank = None):
  class CustomArray(TArray):
    __name__ = name
    __type__ = tp
    __bank__ = bank
  return CustomArray(size)

def print_struct_array(self, out, v):
  if v[0] == None:
    out.print('%s {' % self.__name__)
  else:
    out.print('%s %s {' % (self.__name__, v[0]))
  out.add_indent()
  for a in v[1]:
    a[1].print_actual_text(out, a[0])
  out.remove_indent()
  out.print('}')

class StructArray(TLocationBased):
  def __init__(self, tp, deref_with_bank = None, name = None, terminator = 0xffff):
    self.tp = tp
    self.__name__  = self.tp.__name__ + 's' if name == None else name
    self.deref_with_bank = deref_with_bank
    self.__bank__ = tp.__bank__
    self.terminator = terminator
  def parse_actual_binary(self, ea, extra):
    ea_org = ea
    f = Fields(self, ea)
    while True:
      if self.terminator != None:
        if ROM.get_word(ea) == self.terminator:
          break
      else:
        if len(f.list) == extra[self.__name__ + '_size']:
          break
      if self.deref_with_bank != None:
        addr = self.deref_with_bank << 16 | ROM.get_word(ea)
        g_already_emitted.add(addr)
        size, elem = self.tp.parse_actual_binary(addr, extra)
        ea += 2
      else:
        size, elem = self.tp.parse_actual_binary(ea, extra)
        elem[0].name = None
        ea += size
      f.list.append(elem)
    return ea + 2 - ea_org, (f, self)
  def print_actual_text(self, out, v):
    assert isinstance(v, Fields), repr(v)
    v.print(out, True)
  def want_terminator(self, writer, fields):
    return True
  def write_actual_binary(self, writer, fields, extra):
    if len(fields.dict) != 0:
      writer.error('Array cannot have named fields')
    if self.deref_with_bank == None:
      for f in fields.list:
        if f.ident == self.tp.__name__:
          self.tp.write_actual_binary(writer, f, extra)
      if self.terminator != None and self.want_terminator(writer, fields):
        writer.write_word(self.terminator)
      for f in fields.list:
        if f.ident != self.tp.__name__:
          write_global_object(writer, f)
    else:
      assert self.terminator == None
      for f in fields.list:
        assert f.name != None
        if f.ident == self.tp.__name__:
          self.tp.write_binary(writer, f.name, extra)
        write_global_object(writer, f)

class Blob(TLongLocationBased):
  __name__ = 'Blob'
  def parse_actual_binary(self, ea, extra):
    size = extra['blob_size']
    if size == None:
      return 0, None
    return size, ((get_ea_name(ea), ROM.get_bytes(ea, size)), self)
  def write_actual_binary(self, writer, f, extra):
    assert isinstance(f.value, bytes)
    writer.write_bytes_with_wrap(f.value)
  def print_actual_text(self, out, v):
    b = v[1]
    out.print('%s %s = $hex(' % (self.__name__, v[0]))
    out.add_indent()
    for i in range(0, len(b), 64):
      out.print(b[i:i+64].hex())
    out.remove_indent()                
    out.print(')')
add_global(Blob())

class PackedBlob(Blob):
  __name__ = 'PackedBlob'
  def parse_actual_binary(self, ea, extra):
    size = get_compressed_size(ROM, ea)
    return size, ((get_ea_name(ea), ROM.get_bytes(ea, size)), self)
add_global(PackedBlob())

g_locations = {}
g_already_emitted = set()
g_location_file_map = {
  'Plm' : 'plm',
  'Enemy' : 'enemy',
}
g_current_file = {}
g_file_sets = {'' : g_current_file}

def push_current_file(which_file):
  global g_current_file
  old_file = g_current_file
  g_current_file = g_file_sets.setdefault(which_file, {})
  return old_file

def pop_current_file(old_file):
  global g_current_file
  g_current_file = old_file

def visit_location(ea, tp, extra):
  if (ea & 0x8000) == 0:
    raise Exception('While parsing %s, Invalid location 0x%x' % (tp, ea))
  loc = g_locations.get(ea)
  if loc == None:
    g_locations[ea] = tp # avoid infinite recursion
    which_file = g_location_file_map.get(tp.__name__)
    if which_file != None:
      old_file = push_current_file(which_file)
    res = tp.parse_actual_binary(ea, extra)[1]
    g_current_file[ea] = res
    g_locations[ea] = res
    if which_file != None:
      pop_current_file(old_file)

Palette = MakeArray(uint16_hex, 16, 'Palette')
ItemDropChances = MakeArray(uint8, 6, 'ItemDropChances', 0xb4)
Vulnerability = MakeArray(uint8, 22, 'Vulnerability', 0xb4)
add_global(Palette)
add_global(ItemDropChances)
add_global(Vulnerability)

add_global(MakeArray(uint16_hex, None, 'Palette256'))
add_global(MakeArray(uint16_hex, None, 'Tile16'))

class Name:
  def parse_binary(self, ea, extra):
    v = ROM.get_word(ea)
    if v == 0: return 2, (None, self)
    ea = 0xb40000 | v
    s = ''
    for i in range(10):
      s += chr(ROM.get_byte(ea+i))
    return 2, (s.strip(), self)
  def print_text(self, s):
    return 'null' if s == None else f'"{s}"'
  def write_binary(self, writer, v, extra):
    writer.write_word(0 if v == None else (consts.kEnemyNameToAddr[v] & 0xffff))

CodePtrNoBank = CodePtr(None)

class Sprite(TStruct):
  __name__ = 'Sprite'
  __size__ = 5
  __compact__ = True
  __fields__ = {
    'x': uint16,
    'y': int8,
    'chr': uint16_hex
  }

class SpriteMap(TLocationBased):
  __name__ = 'SpriteMap'
  tp = Sprite()
  def __init__(self, bank = None):
    if bank:
      self.__bank__ = bank
  def parse_actual_binary(self, ea, extra):
    ea_org = ea
    n, r = ROM.get_word(ea), []
    assert n < 256, (hex(ea), n)
    ea += 2
    for i in range(n):
      size, elem = self.tp.parse_actual_binary(ea, extra)
      elem[0].name = None
      ea += size
      r.append(elem)
    return ea + 2 - ea_org, ((get_ea_name(ea_org), r), self)
  def print_actual_text(self, out, v):
    print_struct_array(self, out, v)
  def write_actual_binary(self, writer, v, extra):
    writer.write_word(len(v.list))
    assert len(v.dict) == 0
    for x in v.list:
      assert x.ident == 'Sprite', x.ident
      self.tp.write_actual_binary(writer, x, extra)
add_global(SpriteMap())
add_global(MakeArray(SpriteMap('auto'), None, 'SpriteMapArray'))


class Hitbox(TStruct):
  __name__ = 'Hitbox'
  __size__ = 12
  __compact__ = True
  __fields__ = {
    'left': int16,
    'top': int16,
    'right': int16,
    'bottom': int16,
    'f0' : CodePtr('auto'),
    'f1': CodePtr('auto'),
  }

class Hitboxes(TLocationBased):
  __name__ = 'Hitboxes'
  __bank__ = 'auto'
  tp = Hitbox()
  def parse_actual_binary(self, ea, extra):
    ea_org = ea
    n, r = ROM.get_word(ea), []
    assert n < 256
    ea += 2
    for i in range(n):
      size, elem = self.tp.parse_actual_binary(ea, extra)
      elem[0].name = None
      ea += size
      r.append(elem)
    return ea + 2 - ea_org, ((get_ea_name(ea_org), r), self)
  def print_actual_text(self, out, v):
    print_struct_array(self, out, v)
  def write_actual_binary(self, writer, v, extra):
    writer.write_word(len(v.list))
    assert len(v.dict) == 0
    for x in v.list:
      assert x.ident == 'Hitbox'
      self.tp.write_actual_binary(writer, x, extra)
add_global(Hitboxes())

BigSprite = MakeArray(uint16_hex, None, 'BigSprite', bank = 'auto')

class BigSpriteMap(TLocationBased):
  __name__ = 'BigSpriteMap'
  __bank__ = 'auto'
  def parse_actual_binary(self, ea, extra):
    f = Fields(self, ea)
    ea += 2
    while (ram := ROM.get_word(ea)) != 0xffff:
      n = ROM.get_word(ea + 2)
      (_,rv), rt = BigSprite.parse_actual_binary(ea, {'BigSprite_size' : n + 2})[1]
      f.list.append(((None, rv), rt))
      ea += 2 * n + 4
    return 0, (f, self)
  def print_actual_text(self, out, v):
    v.print(out, True)
  def write_actual_binary(self, writer, v, extra):
    writer.write_word(0xfffe)
    assert len(v.dict) == 0
    for x in v.list:
      assert x.ident == 'BigSprite'
      BigSprite.write_actual_binary(writer, x, extra)
    writer.write_word(0xffff)

add_global(BigSpriteMap())

class ExtSprite(TStruct):
  __name__ = 'ExtSprite'
  __size__ = 8
  __compact__ = True
  __fields__ = {
    'x': int16,
    'y': int16,
    'spr' : SpriteMap('auto'),
    'box': Hitboxes(),
  }

class ExtSpriteBig(TStruct):
  __name__ = 'ExtSpriteBig'
  __size__ = 8
  __compact__ = True
  __fields__ = {
    'x': int16,
    'y': int16,
    'big' : BigSpriteMap(),
    'box': Hitboxes(),
  }

class ExtSpriteMap(TLocationBased):
  __name__ = 'ExtSpriteMap'
  __bank__ = 'auto'
  tp = ExtSprite()
  tpb = ExtSpriteBig()
  def parse_actual_binary(self, ea, extra):
    ea_org = ea
    f = Fields(self, ea)
    n = ROM.get_byte(ea)
    v = ROM.get_byte(ea + 1)
    if v != 0:
      f.fields['upper_byte'] = ROM.get_byte(ea + 1), uint8
    ea += 2
    for i in range(n):
      if get_word(get_word(ea + 4) | (ea & 0xff0000)) == 0xfffe:
        size, elem = self.tpb.parse_actual_binary(ea, extra)
      else:
        size, elem = self.tp.parse_actual_binary(ea, extra)
      elem[0].name = None
      ea += size
      f.list.append(elem)
    return ea + 2 - ea_org, (f, self)
  def print_actual_text(self, out, v):
    v.print(out, True)
  def write_actual_binary(self, writer, v, extra):
    upper = v.dict.get('upper_byte', 0)
    writer.write_word(upper << 8 | len(v.list))
    for x in v.list:
      assert x.ident in ('ExtSprite', 'ExtSpriteBig')
      if x.ident == 'ExtSpriteBig':
        self.tpb.write_actual_binary(writer, x, {})
      else:
        self.tp.write_actual_binary(writer, x, {})
add_global(ExtSpriteMap())

class Enemy(TStruct):
  __name__ = 'Enemy'
  __size__ = 0x40
  __bank__ = 0xa0
  __remove_default__ = True
  __fields__ = {
    'tile_data_size' : uint16_hex,
    'palette_ptr': Palette,
    'health': uint16,
    'damage': uint16,
    'x_radius': uint16,
    'y_radius': uint16,
    'bank': uint8_hex,
    'hurt_ai_time': uint8,
    'hurt_sfx': uint16,
    'boss_fight_value': uint16,
    'ai_init': CodePtrNoBank,
    'num_parts': uint16,
    'field_16': uint16,
    'main_ai': CodePtrNoBank,
    'grapple_ai': CodePtrNoBank,
    'hurt_ai': CodePtrNoBank,
    'frozen_ai': CodePtrNoBank,
    'time_is_frozen_ai': CodePtrNoBank,
    'death_anim': uint16,
    'field_24': uint16,
    'field_26': uint16,
    'powerbomb_reaction': CodePtrNoBank,
    'field_2A': uint16,
    'field_2C': uint16,
    'field_2E': uint16,
    'touch_ai': CodePtrNoBank,
    'shot_ai': CodePtrNoBank,
    'field_34': uint16,
    'tile_data': Blob(),
    'layer': uint8,
    'item_drop_chances': ItemDropChances,
    'vulnerability': Vulnerability,
    'name': Name(),
  }
  def parse_actual_binary(self, ea, extra):
    blob_size = (ROM.get_word(ea) & 0x7fff) or None
    bank = ROM.get_byte(ea + 12)
    return super().parse_actual_binary(ea, {'bank' : bank, 'blob_size' : blob_size})
  def write_actual_binary(self, writer, fields, extra):
    bank = fields.dict.get('bank') or writer.error('Enemy must have a bank')
    super().write_actual_binary(writer, fields, {'bank' : bank})
add_global(Enemy())

class EnemyPopulation(TStruct):
  __name__ = 'EnemyPopulation'
  __bank__ = 0xa1
  __size__ = 16
  __compact__ = True
  __fields__ = {
    'enemy': Enemy(),
    'x': uint16,
    'y': uint16,
    'init': uint16_hex,
    'props': uint16_hex,
    'eprops': uint16_hex,
    'p1': uint16_hex,
    'p2': uint16_hex,
  }
class EnemyPopulations(StructArray):
  def __init__(self):
    super().__init__(EnemyPopulation())
  def parse_actual_binary(self, ea, extra):
    size, data = super().parse_actual_binary(ea, extra)
    v = ROM.get_byte(ea + size)
    if v != 0:
      data[0].fields['enemies_to_clear'] = v, uint8
    return size + 1, data
  def write_actual_binary(self, writer, fields, extra):
    for f in fields.list:
      assert f.ident == self.tp.__name__
      self.tp.write_actual_binary(writer, f, extra)
    writer.write_word(self.terminator)
    writer.write_byte(fields.dict.get('enemies_to_clear', 0))

add_global(EnemyPopulation())
add_global(EnemyPopulations())
EnemyPopulationArrayV = EnemyPopulations()

class Layer3Fx(TStruct):
  __name__ = 'Layer3Fx'
  __bank__ = 0x83
  __size__ = 16
  __remove_name__ = True
  __fields__ = {
    'door': CodePtr(0x83),
    'base_y': int16,
    'target_y': int16,
    'y_vel': int16,
    'timer': uint8,
    'type': uint8,
    'default_layer_blend': uint8,
    'layer3_layer_blend': uint8,
    'fx_liquid_options': uint8,
    'palette_fx_bitset': uint8,
    'animtiles_bitset': uint8,
    'palette_blend': uint8,
  }

class Layer3FxArray(StructArray):
  def parse_actual_binary(self, ea, extra):
    ea_org = ea
    f = Fields(self, ea)
    while (head := ROM.get_word(ea)) != 0xffff:
      size, elem = self.tp.parse_actual_binary(ea, extra)
      ea += size
      f.list.append(elem)
      if head == 0:
        break
    return ea + 2 - ea_org, (f, self)
  def want_terminator(self, writer, fields):
    for f in fields.list[::-1]:
      if f.ident == self.tp.__name__:
        return f.dict.get('door') != None
    return True
add_global(Layer3FxArray(Layer3Fx()))

class EnemyTileset(TStruct):
  __name__ = 'EnemyTileset'
  __bank__ = 0xb4
  __size__ = 4
  __compact__ = True
  __fields__ = {
    'enemy': Enemy(),
    'vram_dst': uint16_hex,
  }
add_global(StructArray(EnemyTileset()))

class Scrolls(TArray):
  __name__ = 'Scrolls'
  __type__ = uint8
  __bank__ = 0x8f
  def print_text(self, ea):
    return str(ea) if ea < 0x8000 else get_ea_name(ea)
add_global(Scrolls())

class PlmScrolls(TLocationBased):
  __name__ = 'PlmScrolls'
  __bank__ = 0x8f
  def parse_actual_binary(self, ea, extra):
    start_ea, rv = ea, []
    while ROM.get_byte(ea) != 0x80:
      rv.append(uint8.parse_binary(ea + 0, extra)[1])
      rv.append(uint8.parse_binary(ea + 1, extra)[1])
      ea += 2
    return ea - start_ea, ((get_ea_name(start_ea), rv), self)
  def print_actual_text(self, out, v):
    inner = ', '.join(a[1].print_text(a[0]) for a in v[1])
    out.print('%s %s { %s }' % (self.__name__, v[0], inner))
  def write_actual_binary(self, writer, fields, extra):
    if len(fields.dict) != 0:
      writer.error('Array cannot have named fields')
    if len(fields.list) % 2:
      writer.error('Array size mismatch')
    for value in fields.list:
      uint8.write_binary(writer, value, extra)
    writer.write_byte(0x80)
add_global(PlmScrolls())

class Door(TStruct):
  __name__ = 'Door'
  __size__ = 12
  __bank__ = 0x83
  __compact__ = True
  __fields__ = {
    'room': CodePtr(0x8f), # lazy loaded
    'flags': uint8,
    'orientation': uint8,
    'x': uint8,
    'y': uint8,
    'screenx': uint8,
    'screeny': uint8,
    'dist': uint16,
    'code': CodePtr(0x8f),
  }
  @classmethod
  def lazy_load(self):
    self.__fields__['room'] = Room()
  def parse_actual_binary(self, ea, extra):
    if ROM.get_word(ea) == 0:
      return 2, (Fields(self, ea), self)
    return super().parse_actual_binary(ea, extra)
  
  def write_actual_binary(self, writer, fields, extra):
    if len(fields.dict) == 0:
      writer.write_word(0)
    else:
      super().write_actual_binary(writer, fields, extra)
add_global(Door())

def MakeLoadBgType(name, fields):
  class LoadBgType(TStruct):
    __name__ = name
    __size__ = None
    __fields__ = fields
    __compact__ = True
    __remove_name__ = True
    __bank__ = 0x8f
  return LoadBgType()

class LoadBgCmd(TLocationBased):
  __name__ = 'LoadBgCmd'
  __bank__ = 0x8f
  __commands__ = {
    2 : MakeLoadBgType('TransferToVram', {'src': LongPtr, 'dst': uint16_hex, 'size' : uint16}),
    4 : MakeLoadBgType('Decompress', {'src': PackedBlob(), 'dst': CodePtr(0x7e)}),
    6 : MakeLoadBgType('ClearFxTilemap', {}),
    8 : MakeLoadBgType('TransferToVramSetBG3', {'src': LongPtr, 'dst': uint16_hex, 'size' : uint16}),
    10: MakeLoadBgType('ClearBG2Tilemap', {}),
    12: MakeLoadBgType('ClearKraidLayer2', {}),
    14: MakeLoadBgType('DoorDepXferVram', {'door' : Door(), 'src': Blob(), 'dst': uint16_hex, 'size' : uint16}),
  }
  def parse_actual_binary(self, ea, extra):
    rv, ea_start = Fields(self, ea), ea
    while True:
      cmd = ROM.get_word(ea)
      if cmd == 0:
        break
      cmd = self.__commands__[cmd]
      extra2 = extra;
      if cmd.__name__ == 'DoorDepXferVram':
        extra2 = {'blob_size': ROM.get_word(ea + 9)}
      size, r = cmd.parse_actual_binary(ea + 2, extra2)
      rv.add_list_entry(r[0], r[1])
      ea += 2 + size
    return ea - ea_start, (rv, self)
  def print_actual_text(self, out, v):
    v.print(out, True)
  def write_actual_binary(self, writer, fields, extra):
    cmdmap = {a.__name__ : (i, a) for i, a in self.__commands__.items()}
    for f in fields.list:
      value, cmd = cmdmap[f.ident]
      writer.write_word(value)
      cmd.write_actual_binary(writer, f, extra)
    writer.write_word(0)
add_global(LoadBgCmd())

class XraySpecialCase(TStruct):
  __name__ = 'XraySpecialCase'
  __size__ = 4
  __bank__ = 0x8F
  __compact__ = True
  __fields__ = {
    'x' : uint8,
    'y' : uint8,
    'data' : uint16_hex,
  }
add_global(StructArray(XraySpecialCase(), terminator = 0))

PlmParserPtr = CodePtr(0x84, visitor = lambda x, e: plm_parser.visit(x))

class Plm(TStruct):
  __name__ = 'Plm'
  __size__ = None
  __bank__ = 0x84
  __compact__ = True
  __fields__ = {
    'code' : CodePtr(0x84, unknown_prefix='sub'),
    'instr' : PlmParserPtr,
    'instr2' : EnableIf(1, PlmParserPtr)
  }
  def parse_actual_binary(self, ea, extra):
    plm_size = plm_decode.plm_header_size(ea)
    return super().parse_actual_binary(ea, {'enable_if': 1 if plm_size == 6 else 0})
  def write_actual_binary(self, writer, fields, extra):
    super().write_actual_binary(writer, fields, {'enable_if' : 0})
add_global(Plm())

class RoomPlm(TStruct):
  __name__ = 'RoomPlm'
  __size__ = 6
  __bank__ = 0x8f
  __compact__ = True
  __fields__ = {
    'plm' : Plm(),
    'x' : uint8,
    'y' : uint8,
    'arg' : EnableIf(1, uint16_hex),
    'arg_plm' : EnableIf(2, PlmScrolls()),
  }
  def parse_actual_binary(self, ea, extra):
    extra = {'enable_if' : 2 if ROM.get_word(ea) in (0xB703, 0xB707) else 1}
    return super().parse_actual_binary(ea, extra)
  def write_actual_binary(self, writer, fields, extra):
    extra = {'enable_if' : 0}
    super().write_actual_binary(writer, fields, extra)
    assert extra['enable_if'] in (1, 2)
add_global(StructArray(RoomPlm(), terminator = 0))

class RoomState(TStruct):
  __name__ = 'RoomState'
  __size__ = 26
  __fields__ = {
    'data': PackedBlob(),
    'gfx_set': uint8,
    'music': uint8,
    'music_ctrl': uint8,
    'layer3_fx': Layer3FxArray(Layer3Fx()),
    'enemy_population': EnemyPopulationArrayV,
    'enemy_tilesets': StructArray(EnemyTileset()),
    'nudge_limit': uint16_hex,
    'scrolls': Scrolls(),
    'xray_special_case': StructArray(XraySpecialCase(), terminator = 0),
    'main_code_ptr': CodePtr(0x8f),
    'room_plm': StructArray(RoomPlm(), terminator = 0),
    'bg_data_ptr': LoadBgCmd(),
    'room_setup_code': CodePtr(0x8f),
  }

class Room(TLocationBased):
  __name__ = 'Room'
  __bank__ = 0x8f
  __fields__ = {
    'roomnr': uint8,
    'area': uint8,
    'x': uint8,
    'y': uint8,
    'w': uint8,
    'h': uint8,
    'up_scroll': uint8,
    'down_scroll': uint8,
    'cre_bitset': uint8,
    'doorouts': CodePtr(0x8f),
  }
  def parse_door_defs(self, roomdef_ea, lvl_data):
    ea = 0x8f0000 | ROM.get_word(roomdef_ea + 9)
    num_doors = 0
    while True:
      w = ROM.get_word(ea + num_doors * 2)
      if not (w >= 0x88FC and w <= 0xABF0):
        break
      num_doors += 1
    new_ea = 0x8f0000 | ROM.get_word(roomdef_ea + 9)
    assert(new_ea & 0x8000)
    sl = StructArray(Door(), deref_with_bank = 0x83, name = 'DoorOuts', terminator = None)
    size, res = sl.parse_actual_binary(new_ea, {'DoorOuts_size' : num_doors})
    return res

  kRoomStateSelects = {
    0xe5e6 : 'Default',
    0xe612 : 'IsEventSet',
    0xe629 : 'IsBossDead',
    0xe669 : 'PowerBombs',
    0xe652 : 'MorphBallMissiles',
    0xe5ff : 'TourianBoss01',
  }

  def parse_actual_binary(self, ea, extra):
    fout, ea_org = Fields(self, ea), ea
    fout.fields['name'] = consts.kRoomNames[(ROM.get_byte(ea + 1), ROM.get_byte(ea))], Name()
    parse_binary_fields(fout, ea, self.__fields__, extra)
    extra = {'Scrolls_size': ROM.get_byte(ea + 4) * ROM.get_byte(ea + 5) }
    ea += 11
    selects = []
    while True:
      f = Fields(RoomState(), ea = None)
      fout.add_list_entry(f, f.tp)
      w = ROM.get_word(ea)
      f.tp.__name__ = self.kRoomStateSelects[w]
      if w == 0xe5e6: # finish
        parse_binary_fields(f, ea + 2, f.tp.__fields__, extra)
        selects.append(f)
        # Add an DoorOuts {} block
        doorouts = self.parse_door_defs(ea_org, ROM.get_long(ea + 2))
        fout.add_list_entry(doorouts[0], doorouts[1])
        del fout.fields['doorouts']
        break
      elif w in (0xe612, 0xe629):
        f.set_args((ROM.get_byte(ea + 2), ))
        ea += 5
      elif w in (0xe669, 0xE652, 0xE5FF):
        ea += 4
      else:
        raise Exception('RoomDefStateSelect unknown 0x%x' % w)
      parse_binary_fields(f, 0x8f0000 | ROM.get_word(ea - 2), f.tp.__fields__, extra)
      selects.append(f)

    def move_in(ea):
      if ea != 0 and (t := g_locations.get(ea)) and ea not in g_already_emitted:
        g_already_emitted.add(ea)
        fout.add_list_entry(t[0], t[1])

    for f in selects:
      move_in(f.fields['scrolls'][0])

    # move in the room plm crap
    for ea in [f.fields['room_plm'][0] for f in selects]:
      if (t := g_locations.get(ea)) == None:
        continue
      for f, tp in t[0].list:
        if tp.__name__ == 'RoomPlm' and (v := f.fields.get('arg_plm')) != None:
          move_in(v[0])
          
    # go through fields and create inner blocks
    for movein in ('xray_special_case', 'enemy_population', 'enemy_tilesets', 'room_plm', 'bg_data_ptr', 'layer3_fx', 'data'):
      for f in selects:
        move_in(f.fields[movein][0])
    # go through the nondefault selects and clear the fields that match the defaults
    for nd in selects[:-1]:
      for k, v in list(nd.fields.items()):
        if k in f.fields and v == f.fields[k]:
          del nd.fields[k]
    return ea - ea_org, (fout, self)
  def print_actual_text(self, out, v):
    v.print(out)
  def write_actual_binary(self, writer, fields, extra):
    extra = {'Scrolls_size': fields.dict['w'] * fields.dict['h'] }
    kRoomStateSelectsRev = {a: b for b, a in self.kRoomStateSelects.items()}
    state_select, default, door_outs = [], None, None
    end_ea = 11
    for f in fields.list:
      if (default == None) and f.ident in kRoomStateSelectsRev:
        state_select.append(f)
        end_ea += 5 if f.ident in ('IsEventSet', 'IsBossDead') else (2 if f.ident == 'Default' else 4)
        if f.ident == 'Default': default = f
      elif f.ident == 'DoorOuts':
        assert door_outs == None
        door_outs = f
      else:
        write_global_object(writer, f, extra)
    assert default != None and door_outs != None
    end_ea += writer.get_ea() + 26 * len(state_select)
    fields.dict['doorouts'] = door_outs.name#end_ea & 0xffff

    for name, tp in self.__fields__.items():
      tp.write_binary(writer, fields.dict.get(name), extra)

    for f in state_select:
      end_ea -= 26
      writer.write_word(kRoomStateSelectsRev[f.ident])
      if f.ident in ('IsEventSet', 'IsBossDead'):
        writer.write_byte(f.args[0])
      if f.ident != 'Default':
        assert (end_ea >> 16) == 0x8f
        writer.write_word(end_ea & 0xffff)
    for f in state_select[::-1]:
      if f.ident != 'Default':
        for k, v in default.dict.items():
          if k not in f.dict:
            f.dict[k] = v
      RoomState().write_actual_binary(writer, f, extra)
    if door_outs:
      assert len(door_outs.dict) == 0
      if door_outs.name != None:
        writer.cur_addr = get_name_ea(door_outs.name)
      for f in door_outs.list:
        assert f.ident == 'Door'
        write_global_object(writer, f, extra)
        kGlobalObjects['Door'].write_binary(writer, f.name, extra)
add_global(Room())
      
class LoadStation(TStruct):
  __name__ = 'LoadStation'
  __bank__ = 0x80
  __size__ = 14
  __compact__ = True
  __fields__ = {
    'room': Room(),
    'door': Door(),
    'door_bts' : uint16,
    'screen_x' : uint16,
    'screen_y' : uint16,
    'samus_y' : uint16,
    'samus_x' : uint16,
  }
add_global(StructArray(LoadStation(), terminator = None))

class TileSet(TStruct):
  __name__ = 'TileSet'
  __bank__ = 0x8f
  __size__ = 9
  __compact__ = True
  __fields__ = {
    'tile_table': PackedBlob(),
    'tiles': PackedBlob(),
    'palette': PackedBlob(),
  }
add_global(TileSet())
add_global(StructArray(TileSet(), deref_with_bank = 0x8f, terminator = None))

class SpriteTilesTransfer(TStruct):
  __name__ = 'SpriteTilesTransfer'
  __bank__ = 0x8f
  __size__ = 7
  __compact__ = True
  __fields__ = {
    'size': uint16,
    'src': Blob(),
    'dst': uint16_hex,
  }
  def parse_actual_binary(self, ea, extra):
    blob_size = ROM.get_word(ea)
    return super().parse_actual_binary(ea, {'blob_size' : blob_size})
add_global(StructArray(SpriteTilesTransfer(), terminator = 0))

class PoseEntry(TStruct):
  __name__ = 'PoseEntry'
  __bank__ = 0x91
  __size__ = 6
  __compact__ = True
  __fields__ = {
    'new': uint16_hex,
    'cur': uint16_hex,
    'pose' : uint16_hex,
  }
add_global(StructArray(PoseEntry()))
add_global(MakeArray(StructArray(PoseEntry()), 253, 'PoseTransitionTable', None))

class PoseParams(TStruct):
  __name__ = 'PoseParams'
  __bank__ = 0x91
  __size__ = 8
  __compact__ = True
  __fields__ = {
    'x_dir': uint8,
    'move_type': uint8,
    'new_pose' : uint8,
    'dir' : uint8,
    'y_offs' : uint8,
    'junk1' : Skip(1),
    'y_radius' : uint8,
    'junk2' : Skip(1),
  }
add_global(StructArray(PoseParams(), terminator = None, name = 'PoseParamsList'))

class SamusTileAnimationDefs(TStruct):
  __name__ = 'SamusTileAnimationDefs'
  __bank__ = 0x92
  __size__ = 4
  __compact__ = True
  __fields__ = {
    'top_idx': uint8,
    'top_pos': uint8,
    'bottom_idx': uint8,
    'bottom_pos': uint8,
  }
add_global(StructArray(SamusTileAnimationDefs(), terminator = None, name = 'SamusTileAnimationDefsList'))
add_global(MakeArray(CodePtr(0x92), 253, 'SamusTileAnimationDefsToplevel', bank = 0x92))
add_global(MakeArray(CodePtr(0x92), None, 'SamusTileDefsHalf', bank = 0x92))
  

class SamusTileAnimationTileDefs(TStruct):
  __name__ = 'SamusTileAnimationTileDefs'
  __bank__ = 0x92
  __size__ = 7
  __compact__ = True
  __fields__ = {
    'src': Blob(),
    'part1_size': uint16_hex,
    'part2_size': uint16_hex,
  }
  def parse_actual_binary(self, ea, extra):
    blob_size = ROM.get_word(ea + 3) + ROM.get_word(ea + 5)
    return super().parse_actual_binary(ea, {'blob_size' : blob_size})

SamusTileAnimationTileDefsList = StructArray(SamusTileAnimationTileDefs(), terminator = None, name = 'SamusTileAnimationTileDefsList')
add_global(SamusTileAnimationTileDefsList)

class SamusSpeedTableEntry(TStruct):
  __name__ = 'SamusSpeedTableEntry'
  __bank__ = 0x90
  __size__ = 12
  __compact__ = True
  __fields__ = {
    'accel': uint16_hex,
    'accel_sub': uint16_hex,
    'max_speed': uint16_hex,
    'max_speed_sub': uint16_hex,
    'decel': uint16_hex,
    'decel_sub': uint16_hex,
  }
add_global(StructArray(SamusSpeedTableEntry(), terminator = None, name = 'SamusSpeedTableArray'))

ProjectileParserPtr = CodePtr(0x93, visitor = lambda x, e: projectile_parser.visit(mark_address(x)))

class ProjectileDataTable(TStruct):
  __name__ = 'ProjectileDataTable'
  __bank__ = 0x93
  __compact__ = True
  __size__ = 0x16
  __fields__ = {
    'dmg': uint16,
    'p0' : ProjectileParserPtr,
    'p1' : ProjectileParserPtr,
    'p2' : ProjectileParserPtr,
    'p3' : ProjectileParserPtr,
    'p4' : ProjectileParserPtr,
    'p5' : ProjectileParserPtr,
    'p6' : ProjectileParserPtr,
    'p7' : ProjectileParserPtr,
    'p8' : ProjectileParserPtr,
    'p9' : ProjectileParserPtr,
  }
add_global(MakeArray(ProjectileDataTable(), None, 'ProjectileDataTableArray'))
add_global(ProjectileDataTable())

class ProjectileDataTableOnlyOne(TStruct):
  __name__ = 'ProjectileDataTableOnlyOne'
  __bank__ = 0x93
  __compact__ = True
  __size__ = 4
  __fields__ = {
    'dmg': uint16,
    'p0' : ProjectileParserPtr,
  }
add_global(ProjectileDataTableOnlyOne())

EprojParserPtr = CodePtr(0x86, visitor = lambda x, e: eproj_parser.visit(mark_address(x)))

class EprojDef(TStruct):
  __name__ = 'EprojDef'
  __bank__ = 0x86
  __size__ = 14
  __fields__ = {
    'init_code': CodePtr(0x86),
    'pre_instr' : CodePtr(0x86),
    'instrs' : EprojParserPtr,
    'radius' : uint16_hex,
    'props' : uint16_hex,
    'hit_instrs' : EprojParserPtr,
    'shot_instrs' : EprojParserPtr,
  }
add_global(EprojDef())

HEX6_REGEXP = re.compile('^.*_([0-9A-Fa-f]{6})$') 
HEX4_REGEXP = re.compile('^.*_([0-9A-Fa-f]{4})$') 

def get_name_ea(name, bank = None):
  assert isinstance(name, str), name
  if name in name2ea:
    return name2ea[name]
  elif (m := HEX6_REGEXP.match(name)) != None:
    return int(m.group(1), 16)
  elif bank != None and (m := HEX4_REGEXP.match(name)) != None:
    return bank << 16 | int(m.group(1), 16)

def write_global_object(writer, f, extra = {}):
  old_addr = writer.cur_addr
  if f.name == None:
    writer.error('no name for %s' % f.ident)
  assert f.name != None
  writer.cur_addr = get_name_ea(f.name)
  if writer.cur_addr == None:
    writer.error('address for %s %s not found' % (f.ident, f.name))
  try:
    kGlobalObjects[f.ident].write_actual_binary(writer, f, extra)
  except:
    print('error writing %s %s' % (f.ident, f.name), file = sys.stderr)
    raise
  writer.cur_addr = old_addr

def visit_rooms():
  old_file = push_current_file('rooms')
  def visit_load_stations():
    kLoadStationCounts = [19, 19, 23, 18, 20, 18, 17, 17]
    for i in range(8):
      ea = 0x800000 | ROM.get_word(0x80C4B5 + i * 2)
      visit_location(ea, kGlobalObjects['LoadStations'], {'LoadStations_size': kLoadStationCounts[i]})
      loc = g_locations[ea][0].list
      for f, tp in list(loc):
        ea = f.fields['door'][0]
        if ea != 0 and (t := g_locations.get(ea)) and ea not in g_already_emitted:
          g_already_emitted.add(ea)
          loc.append((t[0], t[1]))
  visit_load_stations()
  visit_location(0x8f91f8, Room(), {})
  pop_current_file(old_file)

def visit_blobs():
  old_file = push_current_file('blobs')
  visit_location(0x8FE7A7, kGlobalObjects['TileSets'], {'TileSets_size': 29})

  kCompressedDataExtra = [
    0xb98000,0xb9a09d,0xb9a09d,0x978DF4,0x978FCD,0x9791C4,0x97938D,0x97953A,
    0x9580D8,0x94E000,0x96FC04,0x9580D8,0x95A5E1,0x95f90e,0x95d089,0x9788cc,
    0x96ff14,0x95e4c2,0x978d12,0x95D713,0x95A82F,0x96FE69,0x96D10A,0x95A82F,
    0x96FE69,0x96D10A,0x978ADB,0x96EC76,0x98bcd6,0x99a56f,0x99d17e,0x988304,
    0x95a82f,0x96fe69,0x98b5c1,0x98b857,0x98baed,0x98bccd,0x97e7de,0x99d65b,
    0x99d932,0x98ED4F,0x999101,0x979803,0x97B957,0x97D7FC,0x97E7DE,0x9796F4,
    0x97F987,0x99DA9F,0x99DAB1,0x99E089,0x99ECC4,0x97EEFF,0xB9FA38,0xB9FE3E,
  ]
  for ea in kCompressedDataExtra:
    visit_location(ea, kGlobalObjects['PackedBlob'], {})

  for ea in [0xA6C4CB, 0xA98F8F, 0xA98FE5, 0xA99003, 0xA9902F, 0xA98FC7]:
    visit_location(ea, kGlobalObjects['SpriteTilesTransfers'], {})

  for ea, size in [(0xB6F000, 512), (0x8EE400, 512)]:
    visit_location(ea, kGlobalObjects['Blob'], {'blob_size': size})

  # StartDmaCopy
  for ea, size in [(0x80988b, 0x0040),(0x8e8000, 0x5600),(0x8ed600, 0x0600),(0x8edc00, 0x0800),
      (0x9ab200, 0x2000),(0x9ad200, 0x2e00),(0xa7a716, 0x0200),
      (0xb68000, 0x4000),(0xb6c000, 0x2000),(0xb6e000, 0x0800),(0xb6e800, 0x0800),]:
    visit_location(ea, kGlobalObjects['Blob'], {'blob_size': size})

  # kPauseMenuMapTilemaps
  for ea, size in [(0xB58000, 0x1000), (0xB59000, 0x1000), (0xB5A000, 0x1000), (0xB5B000, 0x1000),
                   (0xB5C000, 0x1000), (0xB5D000, 0x1000), (0xB5E000, 0x1000),]:
    visit_location(ea, kGlobalObjects['Blob'], {'blob_size': size})

  visit_location(0x9a8200, kGlobalObjects['Blob'], {'blob_size': 1024 * 2})
  visit_location(0x9a8a00, kGlobalObjects['Blob'], {'blob_size': 1024 * 2})
  visit_location(0x9a9200, kGlobalObjects['Blob'], {'blob_size': 1024 * 2})

  for i in range(12):
    visit_location(0x9a9a00 + i * 512, kGlobalObjects['Blob'], {'blob_size': 256 * 2})

  pop_current_file(old_file)

add_global(MakeArray(uint16, 253, 'SamusPoseToSpritemapIndex'))

def visit_samus():
  old_file = push_current_file('samus')
  visit_location(0x919ee2, kGlobalObjects['PoseTransitionTable'], {})
  visit_location(0x91b629, kGlobalObjects['PoseParamsList'], {'PoseParamsList_size': 253})
  kCounts = [32, 30, 32, 17, 17, 17, 17, 20, 10, 16,
              32, 32, 24, 23, 24, 23, 3, 2, 21, 7, 17, 8, 3, 8]
  ea = 0x92cbee
  for count in kCounts:
    visit_location(ea, SamusTileAnimationTileDefsList, {'SamusTileAnimationTileDefsList_size' : count})
    ea += count * 7
  kAnimdefSizes = [96, 9, 9, 2, 2, 1, 1, 1, 1, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 2, 2, 2, 2, 2, 2, 12, 12, 12, 12, 10, 10, 10, 12, 12, 12, 12, 12, 3, 3, 9, 9, 7, 7, 3, 3, 2, 2, 3, 3, 10, 10, 12, 12, 1, 1, 2, 2, 12, 12, 1, 1, 2, 2, 10, 10, 10, 12, 3, 3, 1, 1, 9, 9, 6, 6, 1, 1, 6, 6, 10, 10, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 66, 66, 66, 66, 66, 66, 2, 2, 9, 9, 7, 7, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, 1, 1, 6, 6, 6, 6, 10, 10, 10, 10, 10, 10, 10, 10, 28, 28, 47, 47, 2, 2, 3, 3, 9, 9, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 96, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 9, 9, 1, 1, 2, 2, 2, 2, 2, 2, 66, 66, 9, 9, 1, 1, 1, 1, 1, 1, 1, 1, 6, 3, 3, 3, 3, 3, 3, 10, 3, 6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 15, 15, 5, 5, 6, 6, 5, 5, 3, 3, 3, 3, 10, 2, 2, 2, 2, 2, 2, 2, 2, 15, 32, 6, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
  for i in range(253):
    visit_location(0x920000 | ROM.get_word(0x92D94E + i*2), kGlobalObjects['SamusTileAnimationDefsList'], {'SamusTileAnimationDefsList_size' : kAnimdefSizes[i]})
  visit_location(0x92d91e, kGlobalObjects['SamusTileDefsHalf'], {'SamusTileDefsHalf_size': 13})
  visit_location(0x92d938, kGlobalObjects['SamusTileDefsHalf'], {'SamusTileDefsHalf_size': 11})
  visit_location(0x92d94e, kGlobalObjects['SamusTileAnimationDefsToplevel'], {})
  visit_location(0x92808d, kGlobalObjects['SpriteMapArray'], {'SpriteMapArray_size' : 2096})
  visit_location(0x909F25, kGlobalObjects['SamusSpeedTableArray'], {'SamusSpeedTableArray_size' : 1})
  visit_location(0x909F31, kGlobalObjects['SamusSpeedTableArray'], {'SamusSpeedTableArray_size' : 1})
  visit_location(0x909F3D, kGlobalObjects['SamusSpeedTableArray'], {'SamusSpeedTableArray_size' : 1})
  visit_location(0x909f49, kGlobalObjects['SamusSpeedTableArray'], {'SamusSpeedTableArray_size' : 27})
  visit_location(0x90A08D, kGlobalObjects['SamusSpeedTableArray'], {'SamusSpeedTableArray_size' : 28})
  visit_location(0x90A1DD, kGlobalObjects['SamusSpeedTableArray'], {'SamusSpeedTableArray_size' : 28})

  visit_location(0x929263, kGlobalObjects['SamusPoseToSpritemapIndex'], {})
  visit_location(0x92945D, kGlobalObjects['SamusPoseToSpritemapIndex'], {})


  pop_current_file(old_file)

def visit_projectiles():
  old_file = push_current_file('proj')
  visit_location(0x9383C1, MakeArray(ProjectileDataTable(), 12, 'ProjectileDataTableArray'), {})
  visit_location(0x9383D9, MakeArray(ProjectileDataTable(), 12, 'ProjectileDataTableArray'), {})
  visit_location(0x9383F1, MakeArray(CodePtr(0x93), 9, 'ProjectileDataTableArray'), {})
  visit_location(0x938403, MakeArray(CodePtr(0x93), 8, 'ProjectileDataTableArray'), {})
  visit_location(0x938413, MakeArray(CodePtr(0x93), 12, 'ProjectileDataTableArray'), {})
  visit_location(0x93842B, MakeArray(CodePtr(0x93), 3, 'ProjectileDataTableArray'), {})
  for ea in range(0x93866D, 0x938695, 4):
    visit_location(ea, ProjectileDataTableOnlyOne(), {})
  visit_location(0x9386D7, ProjectileDataTableOnlyOne(), {})
  for ea in range(0x938695, 0x9386D7, 22):
    visit_location(ea, ProjectileDataTable(), {})
  for ea in range(0x938641, 0x93866D, 22):
    visit_location(ea, ProjectileDataTable(), {})
  visit_location(0x93a1a1, kGlobalObjects['SpriteMapArray'], {'SpriteMapArray_size' : 66})
  projectile_parser.process_queue()
  for spritemap in projectile_parser.sprite_maps:
    visit_location(spritemap, SpriteMap(bank = 'auto'), {})
  pop_current_file(old_file)

PalfxParserPtr = CodePtr(0x8d, visitor = lambda x, e: palfx_parser.visit(mark_address(x)))
class Palfx(TStruct):
  __name__ = 'Palfx'
  __bank__ = 0x8d
  __size__ = 4
  __compact__ = True
  __fields__ = {
    'code': CodePtr(0x8d),
    'instrs': PalfxParserPtr,
  }
add_global(Palfx())
add_global(MakeArray(PalfxParserPtr, 16, 'PalfxArray', bank = 0x8d))
          
def visit_palettes():
  old_file = push_current_file('palettes')

  for ea in consts.kPalfxDefs:
    visit_location(ea, Palfx(), {})

  visit_location(0x8de3e0, kGlobalObjects['PalfxArray'], {})
  visit_location(0x8de400, kGlobalObjects['PalfxArray'], {})
  visit_location(0x8de420, kGlobalObjects['PalfxArray'], {})
  
  palfx_parser.process_queue()

  visit_location(0x8CE1E9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CE3E9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CE5E9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CE7E9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CE9E9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CEBE9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CEDE9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x8CEFE9, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  visit_location(0x9a8000, kGlobalObjects['Palette256'], {'Palette256_size' : 256})
  pop_current_file(old_file)

def visit_tilemaps():
  old_file = push_current_file('tilemaps')
  for i in range(20):
    if t := get_word(0x83ABF0 + i*2):
      visit_location(t | 0x8a0000, Blob(), {'blob_size' : 1056 * 2})
  for i in range(6):
    if t := get_word(0x88ADA6 + i*2):
      visit_location(t | 0x8a0000, Blob(), {'blob_size' : 1024 * 2})
  pop_current_file(old_file)

def visit_plms():
  old_file = push_current_file('plm')
  pp = Plm()
  for ea in consts.kPlmHeaderAddrs:
    visit_location(ea, pp, {})
  
  plm_parser.visit(mark_address(0x84D519))
  plm_parser.process_queue()
  for k, v in plm_parser.blobs.items():
    visit_location(k, Blob(), {'blob_size': v})

  pop_current_file(old_file)

def visit_eproj():
  old_file = push_current_file('eproj')
  for ea in consts.kEprojHeaders:
    visit_location(ea, kGlobalObjects['EprojDef'], {})
  for ea in consts.kEprojInstrLists:
    eproj_parser.visit(mark_address(ea))
  eproj_parser.process_queue()
  for spritemap in eproj_parser.sprite_maps:
    visit_location(spritemap, SpriteMap(bank = 'auto'), {})
  pop_current_file(old_file)

class MsgBoxConfig(TStruct):
  __name__ = 'MsgBoxConfig'
  __bank__ = 0x85
  __size__ = 6
  __compact__ = True
  __fields__ = {
    'modify': CodePtr(0x85),
    'draw': CodePtr(0x85),
    'tilemap': CodePtr(0x85),
  }
add_global(StructArray(MsgBoxConfig(), terminator = None, name = 'MessageBoxConfigArray'))

def visit_msgbox():
  old_file = push_current_file('msgbox')
  visit_location(0x85869B, kGlobalObjects['MessageBoxConfigArray'], {'MessageBoxConfigArray_size' : 29})
  arr = sorted(set(0x850000 | ROM.get_word(0x85869B + i * 6 + 4) for i in range(29)))
  for i in range(len(arr)):
    visit_location(arr[i], kGlobalObjects['Tile16'], {'Tile16_size' : (0 if i == (len(arr) - 1) else arr[i + 1] - arr[i]) // 2 })
  pop_current_file(old_file)

AnimtilesParserPtr = CodePtr(0x87, visitor = lambda x, e: animtiles_parser.visit(mark_address(x), e['size']))

class Animtiles(TStruct):
  __name__ = 'Animtiles'
  __bank__ = 0x87
  __size__ = 6
  __compact__ = True
  __fields__ = {
    'instrs': AnimtilesParserPtr,
    'size': uint16_hex,
    'vram_addr': uint16_hex
  }
  def parse_actual_binary(self, ea, extra):
    return super().parse_actual_binary(ea, {'size' : ROM.get_word(ea + 2)})
add_global(Animtiles())


def visit_animtiles():
  old_file = push_current_file('animtiles')
  for ea in consts.kAnimtilesDefs:
    visit_location(ea, Animtiles(), {})
#    ia = get_word(ea) | 0x870000
#    size = get_word(ea + 2)
#    animtiles_parser.visit(mark_address(ia), size)
  animtiles_parser.process_queue()
  for k, v in animtiles_parser.blobs.items():
    visit_location(k, Blob(), {'blob_size': v})

  pop_current_file(old_file)

def visit_misc():
  visit_location(0xA0DAFF, kGlobalObjects['Enemy'], {})
  visit_location(0xA0DF7F, kGlobalObjects['Enemy'], {})
  visit_location(0xA0EF3F, kGlobalObjects['Enemy'], {})
  visit_location(0xA0EFBF, kGlobalObjects['Enemy'], {})
  
  for x in consts.kEnemyPopulation:
    visit_location(x, kGlobalObjects['EnemyPopulation'], {})

def visit_enemy():
  old_file = push_current_file('enemy')
  for k, v in name2ea.items():
    if '_Ilist_' in k and k != 'kRidley_Ilist_DCBA':
      if (v >> 16) != 0xb4:
        enemy_instr_parser.visit(mark_address(v))
  enemy_instr_parser.process_queue()

  for ea in enemy_instr_parser.sprite_maps:
    if 'ExtSprmap' in ea2name[ea]:
      visit_location(ea, ExtSpriteMap(), {})
    else:
      visit_location(ea, SpriteMap(bank = 'auto'), {})

  visit_location(0xADC600, Blob(), {'blob_size' : 0x1800})

  pop_current_file(old_file)

for name, obj in inspect.getmembers(inspect.getmodule(inspect.currentframe()), inspect.isclass):
  if hasattr(obj, 'lazy_load') and callable(getattr(obj, 'lazy_load')):
    obj.lazy_load()

def compile_code():
  try:
    parser = text_lexer.ParserAndWriter(write_global_object, get_name_ea)
    #parser.output = bytearray(0xff for i in range(1024*1024*3))
    #parser.output_is_set = bytearray(0 for i in range(1024*1024*3))
    parser.output = bytearray(ROM.data)
    parser.output_is_set = bytearray(0 for i in range(1024*1024*3))
    for k in glob.glob('defs/*.txt'):
      parser.parse_toplevel(k)
    parser.apply_relocs()
    open('out.bin', 'wb').write(parser.output)
#    open('out_is_set.bin', 'wb').write(parser.output_is_set)
    print(sum(parser.output_is_set))
  except Exception as e:
    print(e.args)
    raise  

def visit_all():
  visit_enemy()
  visit_tilemaps()
  visit_plms()
  visit_rooms()
  visit_blobs()
  visit_samus()
  visit_projectiles()
  visit_palettes()
  visit_eproj()
  visit_msgbox()
  visit_animtiles()
  visit_misc()

def print_things(what, file):
  out = LinePrinter()
  for k, v in sorted(what.items()):
    if k not in g_already_emitted:
      g_already_emitted.add(k)
      v[1].print_actual_text(out, v[0])
  for l in out.lines:
    print(l, file = file)

import os
try:
  os.mkdir('defs')
except FileExistsError:
  pass

if sys.argv[1] == 'decode':
  visit_all()  

  for k, v in g_file_sets.items():
    with open('defs/%s.txt' % (k if k != '' else 'out'), 'w') as f:
      if k == 'plm':
        plm_parser.print(f)
      elif k == 'eproj':
        eproj_parser.print(f)
      elif k == 'proj':
        projectile_parser.print(f)
      elif k == 'animtiles':
        animtiles_parser.print(f)
      elif k == 'palettes':
        palfx_parser.print(f)
      elif k == 'enemy':
        enemy_instr_parser.print(f)
      print_things(v, file = f)
    

if sys.argv[1] == 'compile':
  compile_code()
