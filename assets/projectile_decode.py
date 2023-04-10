from decode_common import *

BANK = 0x93

kProjInstructionSet = {
  0x93822F: ('Delete', ''),
  0x938239: ('Goto', 'G'),
}

kCommandByName = {v[0] : (k, v[1]) for k, v in kProjInstructionSet.items()}
kCommandByName['Draw'] = (None, 'HABBH')

kProjTerminator = { 0x938239, 0x93822F}

class ProjParser(InstrParserCommon):
  instruction_set = kProjInstructionSet
  instruction_set_term = kProjTerminator
  TAG = 'proj'

  def __init__(self):
    super().__init__()
    self.sprite_maps = set()

  def handle_draw_command(self, ins, ea):
    drawp = get_word(ea + 2) | (BANK << 16)
    self.sprite_maps.add(drawp)
    self.print_line(ea, f'  Draw({ins}, {get_ea_name(drawp)}, {get_byte(ea + 4)}, {get_byte(ea + 5)}, {get_word(ea + 6)})') 
    return 8

  def process_queue_entry(self, ea):
    assert ea & 0x8000
    while True:
      if ea in self.visited:
        break
      self.visited.add(ea)
      ins = get_word(ea)
      if ins & 0x8000:
        ea_org = ea
        ins = (BANK << 16) | ins
        if ins not in self.instruction_set:
          print(f'// Ins {ins:X} not in iset at {ea:X}')
          return
        name, operands = self.instruction_set[ins]
        ea += 2
        r = []
        for op in operands:
          if op == 'G':
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
