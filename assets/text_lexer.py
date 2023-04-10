import re
import plm_decode
import projectile_decode
import eproj_decode
import animtiles_decode
import palfx_decode
import enemy_instr_decode

NUMBER_RE = re.compile(r'(-?)(0x[0-9a-fA-F]+|\d+)')
HEX_NUMBER4_RE = re.compile(r'[0-9a-fA-F]{4}')
IDENT_RE = re.compile(r'[a-zA-Z_][a-zA-Z0-9_]*')
HEX_NUMBER_RE = re.compile(r'[0-9a-fA-F]+')

class Lexer:
  def __init__(self):
    self.expectation = []

  def set_file(self, filename):
    self.token, self.token_arg = None, None
    self.has_token = False
    self.mode = ''
    self.text = open(filename).read()
    self.text_pos = 0
    self.lineno = 1
    self.start_lineno = 1
    self.start_line = 0
    self.filename = filename

  def next_char(self):
    if self.text_pos >= len(self.text):
      return ''
    c = self.text[self.text_pos]
    self.text_pos += 1
    return c

  def fill(self):
    self.start_lineno = self.lineno
    while True:
      c = self.next_char()
      if c != ' ' and c != '\t' and c != '\n':
        break
      if c == '\n':
        self.lineno += 1
        self.start_line = self.text_pos

    if c in ('', '{', '}', ':', ',', '(', ')', '!', '@', '$', '='):
      return c, True

    if self.mode == 'tile4':
      if (m := HEX_NUMBER4_RE.match(self.text, self.text_pos - 1)) != None:
        self.text_pos = m.end()
        return 'tile4', int(m.group(0), 16)
      
    if self.mode == 'hex':
      if (m := HEX_NUMBER_RE.match(self.text, self.text_pos - 1)) != None:
        self.text_pos = m.end()
        return 'hex_data', m.group(0)
    else:
      if c >= '0' and c <= '9' or c == '-':
        m = NUMBER_RE.match(self.text, self.text_pos - 1)
        self.text_pos = m.end()
        return 'number', int(m.group(0), 0)

    if c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' or c == '_':
      m = IDENT_RE.match(self.text, self.text_pos - 1)
      self.text_pos = m.end()
      return 'ident', m.group(0)

    if c == '<' and self.text.startswith('?', self.text_pos):
      self.text_pos += 1
      return 'code_start', True
      
    if c == '?' and self.text.startswith('>', self.text_pos):
      self.text_pos += 1
      return 'code_end', True
    
    if c == '"':
      l = 0
      while (c := self.next_char()) not in ('', '"'):
        l += 1
      return 'string', self.text[self.text_pos - l - 1 : self.text_pos - 1]
    self.syntax_error(f"Unexpected character '{c}'")

  def is_next_or_newline(self, symbol):
    if rv := self.is_next(symbol):
      return rv
    return True if self.lineno != self.start_lineno else None

  def is_next(self, expected):
    if not self.has_token:
      self.token, self.token_arg = self.fill()
      self.has_token = True
      del self.expectation[:]
    self.expectation.append(expected)
    if self.token != expected:
      return None
    self.has_token = False
    return self.token_arg
  
  def peek_next(self, expected):
    if not self.has_token:
      self.token, self.token_arg = self.fill()
      self.has_token = True
      del self.expectation[:]
    return self.token == expected

  def expect(self, expected):
    if rv := self.is_next(expected):
      return rv
    self.syntax_error()

  def syntax_error(self, msg = 'Error'):
    expects = ', '.join(f"'{a}'" for a in self.expectation)
    tok = self.token_arg if self.token=='ident' else self.token
    self.error(f"{msg} at '{tok}'. Expecting: {expects}")
    raise Exception()

  def error(self, msg):
    msg = f'{self.filename}:{self.lineno}: {msg}'
    raise Exception(msg)

class Parser(Lexer):
  def parse_function_args(self):
    if self.is_next(')'):
      return ()
    args = []
    while True:
      if (number := self.is_next('number')) != None:
        args.append(number)
      elif (ident := self.is_next('ident')) != None:
        args.append(ident)
      else:
        self.syntax_error()
      if self.is_next(')'):
        return args
      self.expect(',')

  def parse_draw_def(self):
    result = []
    x, y = 0, 0
    while True:
      if (num := self.is_next('number')) != None:
        ident = self.is_next('ident')
        if ident == 'x':
          x = num
          continue
        elif ident == 'y':
          y = num
          continue
      elif (ident := self.is_next('ident')) != None:
        if ident in ('h', 'v'):
          self.mode = 'tile4'
          r = []
          result.append((ident, x, y, r))
          while True:
            if (num := self.is_next('tile4')) != None:
              r.append(num)
            elif self.is_next('!') != None:
              self.mode = ''
              x, y = 0, 0
              break
            elif self.is_next_or_newline(';'):
              self.mode = ''
              return result
            else:
              self.syntax_error()
          continue
      self.syntax_error()

  def parse_code_common(self, tag, module, spr_bank = None):
    if (ident := self.is_next('ident')) != None:
      if self.is_next(':'): # label
        self.add_label(ident, module)
        return
      if self.is_next('('): # command
        args = self.parse_function_args()
        self.add_call(ident, module, args)
        return
    elif spr_bank and (number := self.is_next('number')) != None:
      self.expect('!')
      ident = self.expect('ident')
      self.write_word(number)
      if spr_bank == 'auto': spr_bank = self.get_ea() >> 16
      self.write_word(self.get_addr_in_bank(ident, spr_bank))
      return
    self.syntax_error()

  def parse_plm_code(self):
    if (ident := self.is_next('ident')) != None:
      if self.is_next(':'): # label
        self.add_label(ident, plm_decode)
      elif self.is_next('('): # command
        args = self.parse_function_args()
        self.add_call(ident, plm_decode, args)
      elif self.is_next('!'):
        self.add_draw_def(ident, self.parse_draw_def())
      else:
        self.syntax_error()
    elif (number := self.is_next('number')) != None:
      self.expect('!')
      self.add_draw_cmd(number, self.expect('ident'))
    else:
      self.syntax_error()

  def parse_proj_code(self):
    self.parse_code_common('proj', projectile_decode, None)

  def parse_eproj_code(self):
    self.parse_code_common('eproj', eproj_decode, 0x8d)

  def parse_animtiles_code(self):
    self.parse_code_common('animtiles', animtiles_decode, 0x87)
  
  def parse_enemy_code(self):
    self.parse_code_common('enemy', enemy_instr_decode, 'auto')

  def parse_palfx_code(self):
    module = palfx_decode
    if (ident := self.is_next('ident')) != None:
      if self.is_next(':'): # label
        self.add_label(ident, module)
        return
      if self.is_next('('): # command
        args = self.parse_function_args()
        self.add_call(ident, module, args)
        return
    else:
      if (number := self.is_next('number')) != None:
        self.write_word(number)
      self.expect('!')
      self.mode = 'tile4'
      while (v := self.is_next('tile4')) != None:
        assert v & 0x8000 == 0
        self.write_word(v)
      self.mode = ''
      if not self.is_next('!'):
        self.write_word(0xc595) # Wait
      return
    self.syntax_error()

  def parse_toplevel(self, filename):
    print('Parsing', filename)
    self.set_file(filename)
    kCodeParsers = {
      'plm': self.parse_plm_code,
      'proj': self.parse_proj_code,
      'eproj': self.parse_eproj_code,
      'animtiles': self.parse_animtiles_code,
      'palfx' : self.parse_palfx_code,
      'enemy' : self.parse_enemy_code,
    }
    while not self.is_next(''):
      if self.is_next('code_start'):
        parser = kCodeParsers.get(self.expect('ident'))
        if parser == None:
          self.syntax_error()
        while not self.is_next('code_end'):
          parser()
      else:
        ident = self.expect('ident')
        name = self.expect('ident')
        if self.is_next('{'):
          self.parse_fields(ident, name, None)
        elif self.is_next('='):
          self.parse_assignment_value(ident, name)
        else:
          self.syntax_error()

  def parse_fields(self, fields_ident, fields_name, args):
    self.begin_field(fields_ident, fields_name, args)
    has_comma = True
    allow_list_values = True
    while not self.is_next('}'):
      if not has_comma:
        self.syntax_error()
      if (ident := self.is_next('ident')) != None:
        if self.is_next(',') or self.peek_next('}'):
          if not allow_list_values:
            self.error(f"Error: keyword arguments need to be first, found '{ident}'")
          self.add_field_value(None if ident == 'null' else ident)
          has_comma = True
          continue
        args = self.parse_function_args() if self.is_next('(') else None
        name = self.is_next('ident')
        if self.is_next('{'):
          self.parse_fields(ident, name, args)
          has_comma, allow_list_values = True, False
          continue
        if args == None and name != None and self.is_next('='):
          self.parse_assignment_value(ident, name)
          continue
        if name == None and args == None and self.is_next(':'):
          self.add_field_key_value(ident, self.parse_field_value())
          has_comma, allow_list_values = self.is_next_or_newline(',') != None, False
          continue
      elif (number := self.is_next('number')) != None:
        if not allow_list_values:
          self.error(f"Error: keyword arguments need to be first, found '{number}'")
        self.add_field_value(number)
        has_comma = self.is_next_or_newline(',') != None
        continue
      self.syntax_error()
    self.end_field()
  
  def parse_assignment_value(self, ident, name):
    self.expect('$')
    if self.expect('ident') != 'hex':
      self.error("expecting 'hex'")
    self.expect('(')
    self.mode = 'hex'
    hex_data = []
    while d := self.is_next('hex_data'):
      hex_data.append(d)
    self.expect(')')
    self.mode = ''
    class HexField:
      def __init__(self):
        self.value = bytes.fromhex("".join(hex_data))
        self.ident = ident
        self.name = name
    self.write_global_object(self, HexField())

  def parse_field_value(self):
    if (number := self.is_next('number')) != None:
      return number
    elif (string := self.is_next('string')) != None:
      return string
    elif (ident := self.is_next('ident')) != None:
      return None if ident == 'null' else ident
    self.syntax_error()

class WriterFields:
  def __init__(self, ident, name, args):
    self.ident = ident
    self.name = name
    self.args = args
    self.list = []
    self.dict = {}

class ParserAndWriter(Parser):
  def __init__(self, write_global_object, get_name_ea):
    super().__init__()
    self.write_global_object = write_global_object
    self.get_name_ea = get_name_ea
    self.cur_addr = 0
    self.output = bytearray(0x300000)
    self.output_is_set = bytearray(0x300000)
    self.relocs = []
    self.label_addr = {}
    self.stack = []

  def check_int_range(self, v, mi, ma):
    if not isinstance(v, int):
      self.error(f'Value {v} is not an integer')
    if v < mi or v > ma:
      self.error(f'Value {v} out of range [{mi}, {ma}]')

  def get_addr(self, name):
    result = self.get_name_ea(name)
    if result == None:
      self.error(f'Unknown address {name}')
    return result

  def get_addr_in_bank(self, name, bank):
    result = self.get_addr(name)
    if bank != None and (result >> 16) != bank:
      self.error(f'Address {name} is not in bank {bank:x}')
    return result & 0xffff

  def add_label(self, lbl, module):
    addr = self.get_name_ea(lbl, bank = module.BANK)
    if addr != None:
      self.cur_addr = addr
    self.label_addr[lbl] = self.cur_addr

  def add_call(self, cmd, module, args):
    cmdvalue, argspec = module.kCommandByName[cmd]
    if len(args) != len(argspec):
      self.error(f'Wrong number of arguments to {cmd}, got {len(args)}, expecting {len(argspec)}')
    if cmdvalue != None:
      if module.BANK != None and (cmdvalue >> 16) != module.BANK:
        self.error(f'Not in bank {module.BANK:X}: {v}')
      self.write_word(cmdvalue & 0xffff)
    for i in range(len(args)):
      a, v = argspec[i], args[i]
      if a == 'B':
        self.write_byte(v)
      elif a in ('H', 'X', 'I'):
        self.write_word(v & 0xffff)
      elif a == 'L': # long address
        self.write_long(self.get_addr(v))
      elif a == 'A': # address in bank same bank
        self.write_word(self.get_addr_in_bank(v, module.BANK))
      elif a == '9': # address in bank 89
        self.write_word(self.get_addr_in_bank(v, 0x89))
      elif a == 'a': # address in bank a0
        self.write_word(self.get_addr_in_bank(v, 0xa0))
      elif a == '6': # address in bank a0
        self.write_word(self.get_addr_in_bank(v, 0xa6))
      elif a == 'S': # spritemap in bank 8d
        self.write_word(self.get_addr_in_bank(v, 0x8d))
      elif a == 'G': # goto
        self.add_reloc(v, module.BANK)
      else:
        raise Exception(f'Unknown command character: {a}')
  
  def add_reloc(self, target, bank):
    assert isinstance(target, str), target
    v = self.label_addr.get(target)
    if v == None:
      v = self.get_name_ea(target)
    if v != None:
      if bank != None and (v >> 16) != bank:
        self.error(f"Label '{target}/{v:x}' referenced from {self.cur_addr:x} is not in bank {bank:x}")
      self.write_word(v & 0xffff)
    else:
      self.relocs.append((self.cur_addr, target, bank))
      self.cur_addr += 2

  def apply_relocs(self):
    for addr, target, bank in self.relocs:
      v = self.label_addr.get(target)
      if v == None:
        v = self.get_name_ea(target)
      if v == None:
        self.error(f"Unknown label '{target}' referenced from {addr:x}")
      if (v >> 16) != bank:
        self.error(f"Label '{target}' referenced from {addr:x} is not in bank {bank:x}")
      self.cur_addr = addr
      self.write_word(v & 0xffff)

  def add_draw_cmd(self, number, cmd):
    self.write_word(number)
    self.write_word(self.get_addr_in_bank(cmd, 0x84))

  def add_draw_def(self, ident, draw_def):
    backup = self.cur_addr
    self.cur_addr = 0x840000 | self.get_addr_in_bank(ident, 0x84)
    for i, line in enumerate(draw_def):
      if i != 0:
        self.write_word((line[1] & 0xff) | (line[2] & 0xff) << 8)
      self.write_word((line[0] == 'v') * 0x8000 | len(line[3]))
      for a in line[3]:
        self.write_word(a)
    self.write_word(0)
    self.cur_addr = backup

  def begin_field(self, ident, name, args):
    wf = WriterFields(ident, name, args)
    self.stack.append(wf)

  def add_field_value(self, value):
    self.stack[-1].list.append(value)

  def add_field_key_value(self, key, value):
    self.stack[-1].dict[key] = value

  def end_field(self):
    wf = self.stack.pop()
    if len(self.stack):
      self.add_field_value(wf)
    else:
      self.write_global_object(self, wf)

  def write_byte(self, v):
    assert self.cur_addr != None

    self.check_int_range(v, 0, 255)
    addr = self.cur_addr
    assert addr & 0x8000
    ea = (((addr >> 16) << 15) | (addr & 0x7fff)) & 0x3fffff
    if self.output_is_set[ea] and self.output[ea] != v:
      self.error('Address 0x%x already set to a different value (0x%x vs 0x%x)' % (self.cur_addr, self.output[ea], v))
    self.output[ea], self.output_is_set[ea] = v, 1
    self.cur_addr += 1

  def write_word(self, v):
    self.check_int_range(v, -32768, 65535)
    self.write_byte(v & 0xff)
    self.write_byte((v >> 8) & 0xff)

  def write_long(self, v):
    self.write_byte(v & 0xff)
    self.write_byte((v >> 8) & 0xff)
    self.write_byte(v >> 16)

  def write_bytes(self, v):
    for b in v:
      self.write_byte(b)

  def write_bytes_with_wrap(self, v):
    for b in v:
      if (self.cur_addr & 0xffff) == 0:
        self.cur_addr += 0x8000
      self.write_byte(b)

  def write_symbol_word(self, v, bank):
    if v == 0 or v == None:
      self.write_word(0)
    else:
      self.add_reloc(v, bank)

  def write_symbol_long(self, v):
    if not isinstance(v, int):
      ea = self.get_name_ea(v)
      if ea == None:
        self.error(f'Unknown address {v}')
      v = ea
    self.write_long(v)

  def get_ea(self):
    return self.cur_addr
