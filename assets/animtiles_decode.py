from decode_common import *

BANK = 0x87

kInstructionSet = {
  0x8780b2: ('Delete', ''),
  0x8780b7: ('Goto', 'G'),
  0x87813f: ('GotoIfEventHappened', 'HG'),
  0x878150: ('SetEventHappened', 'H'),
  0x8781ba: ('WaitUntilAreaBossDead_DoubleRet', ''),
  0x878303: ('GotoIfBossBitSetInArea', 'BBG'),
  0x878320: ('SpawnTourianStatueEyeGlow', 'H'),
  0x87832f: ('SpawnTourianStatueSoul', 'H'),
  0x87833e: ('GotoIfTourianStatueBusy', 'G'),
  0x878349: ('TourianStatueSetState', 'H'),
  0x878352: ('TourianStatueClearState', 'H'),
  0x87835b: ('Clear3PaletteColors', 'H'),
  0x878372: ('SpawnPalfxObj', 'H'),
  0x87837f: ('Write8PaletteColors', 'H'),
}

kCommandByName = {v[0] : (k, v[1]) for k, v in kInstructionSet.items()}

class AnimtilesParser(InstrParserCommon):
  instruction_set = kInstructionSet
  instruction_set_term = {0x8780b7, 0x8780b2}
  TAG = 'animtiles'

  def __init__(self):
    super().__init__()
    self.current_array_size = None

  def handle_draw_command(self, ins, ea):
    drawp = get_word(ea + 2) | (BANK << 16)
    assert self.current_array_size != None
    self.blobs[drawp] = self.current_array_size
    self.print_line(ea, f'  {ins} ! {get_ea_name(drawp)}') 
    return 4
  
  def visit(self, ea, cur_size = None):
    if cur_size == None:
      super().visit(ea)  
      return
    self.current_array_size = cur_size
    super().visit(ea)
    self.process_queue(final = False)
    
  def process_queue_entry(self, ea):
    assert ea & 0x8000
    while ea not in self.visited:
      self.visited.add(ea)
      ins = get_word(ea)
      if ins & 0x8000:
        ea_org = ea
        ins = (BANK << 16) | ins
        if ins not in self.instruction_set:
          raise Exception(f'Ins {ins:X} not in iset at {ea:X}')
        name, operands = self.instruction_set[ins]
        ea += 2
        r = []
        for op in operands:
          if op == 'B':
            r.append('%d' % get_byte(ea))
            ea += 1
          elif op == 'H':
            r.append('%d' % get_word(ea))
            ea += 2          
          elif op == 'G':
            addr = (BANK << 16) | get_word(ea)
            r.append(get_ea_name(addr, short_label=True))
            self.visit(addr)
            ea += 2
          else:
            assert 0
        self.print_line(ea_org, f'  {name}({", ".join(r)})') 
        if ins in self.instruction_set_term:
          self.print_line(ea_org + 1, '')
          break
      else:
        ea += self.handle_draw_command(ins, ea)
