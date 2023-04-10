from decode_common import *

BANK = 0x8D

kInstructionSet = {
    0x8dc595: ('Wait', ''),
    0x8dc599: ('ColorPlus2', ''),
    0x8dc5a2: ('ColorPlus3', ''),
    0x8dc5ab: ('ColorPlus4', ''),
    0x8dc5b4: ('ColorPlus8', ''),
    0x8dc5bd: ('ColorPlus9', ''),
    0x8dc5c6: ('ColorPlus15', ''),
    0x8dc5cf: ('Delete', ''),
    0x8dc5d4: ('SetPreInstr', 'A'),
    0x8dc5dd: ('ClearPreInstr', ''),
    0x8dc61e: ('Goto', 'G'),
    0x8dc639: ('DecTimerGoto', 'G'),
    0x8dc648: ('SetTimer', 'B'),
    0x8dc655: ('SetColorIndex', 'H'),
    0x8dc65e: ('QueueMusic', 'B'),
    0x8dc66a: ('QueueSfx1', 'B'),
    0x8dc673: ('QueueSfx2', 'B'),
    0x8dc67c: ('QueueSfx3', 'B'),
    0x8df1c6: ('SetPalfxIndex', 'B'),
}

kCommandByName = {v[0] : (k, v[1]) for k, v in kInstructionSet.items()}

class PalfxParser(InstrParserCommon):
  instruction_set = kInstructionSet
  instruction_set_term = {0x8DC5CF, 0x8DC61E}
  TAG = 'palfx'

  def __init__(self):
    super().__init__()

  def handle_draw_command(self, ins, ea):
    xs = []
    while (x := get_word(ea + 2 + len(xs) * 2)) & 0x8000 == 0:
       xs.append('%.4x' % x)
    s = f'{ins:2} ' if ins != None else ''
    s = f'  {s}! {" ".join(xs)}'
    if x != 0xc595:
      s += ' !'
    self.print_line(ea, s) 
    return 2 + len(xs) * 2
   
  def process_queue_entry(self, ea):
    assert ea & 0x8000
    is_finish = True
    while ea not in self.visited:
      self.visited.add(ea)
      ins = get_word(ea)
      if ins & 0x8000:
        if ins == 0xc595:
          assert not is_finish
          ea += 2
          is_finish = True
          continue
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
          elif op == 'A':
            addr = (BANK << 16) | get_word(ea)
            assert addr & 0x8000
            r.append(get_ea_name(addr, short_label=True))
            ea += 2            
          else:
            assert 0
        self.print_line(ea_org, f'  {name}({", ".join(r)})') 
        if ins in self.instruction_set_term:
          self.print_line(ea_org + 1, '')
          break
      else:
        if not is_finish:
          ea -= 2
          ins = None
        ea += self.handle_draw_command(ins, ea)
        is_finish = False
