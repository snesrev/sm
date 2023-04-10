import pickle
from decode_common import *

BANK = 0x86

kEprojInstructionSet = {
  0x868154: ('Delete', ''),
  0x868159: ('Sleep', ''),
  0x868161: ('SetPreInstr_', 'A'),
  0x86816a: ('ClearPreInstr', ''),
  0x868171: ('CallFunc', 'L'),
  0x86818b: ('CallFuncWithParam', 'LH'),
  0x8681ab: ('Goto', 'G'),
  0x8681c6: ('DecTimerAndGotoIfNonZero', 'G'),
  0x8681d5: ('SetTimer', 'H'),
  0x8681de: ('nullsub_82', ''),
  0x8681df: ('MoveRandomlyWithinRadius', 'BBBB'),
  0x868230: ('SetProjectileProperties', 'X'),
  0x86823c: ('ClearProjectileProperties', 'X'),
  0x868248: ('EnableCollisionWithSamusProj', ''),
  0x868252: ('DisableCollisionWithSamusProj', ''),
  0x86825c: ('DisableCollisionWithSamus', ''),
  0x868266: ('EnableCollisionWithSamus', ''),
  0x868270: ('SetToNotDieOnContact', ''),
  0x86827a: ('SetToDieOnContact', ''),
  0x868284: ('SetLowPriority', ''),
  0x86828e: ('SetHighPriority', ''),
  0x868298: ('SetXyRadius', 'BB'),
  0x8682a1: ('SetXyRadiusZero', ''),
  0x8682a5: ('CalculateDirectionTowardsSamus', ''),
  0x8682d5: ('WriteColorsToPalette', 'HHH'),
  0x8682fd: ('QueueMusic', 'B'),
  0x868309: ('QueueSfx1_Max6', 'B'),
  0x868312: ('QueueSfx2_Max6', 'B'),
  0x86831b: ('QueueSfx3_Max6', 'B'),
  0x868324: ('QueueSfx1_Max15', 'B'),
  0x86832d: ('QueueSfx2_Max15', 'B'),
  0x868336: ('QueueSfx3_Max15', 'B'),
  0x86833f: ('QueueSfx1_Max3', 'B'),
  0x868348: ('QueueSfx2_Max3', 'B'),
  0x868351: ('QueueSfx3_Max3', 'B'),
  0x86835a: ('QueueSfx1_Max9', 'B'),
  0x868363: ('QueueSfx2_Max9', 'B'),
  0x86836c: ('QueueSfx3_Max9', 'B'),
  0x868375: ('QueueSfx1_Max1', 'B'),
  0x86837e: ('QueueSfx2_Max1', 'B'),
  0x868387: ('QueueSfx3_Max1', 'B'),
  0x868c68: ('SpawnEnemyDropsWithDraygonsEyeDrops', ''),
  0x868cf6: ('EnemyProjInstr_SetPreInstrA', ''),
  0x868d99: ('EprojInstr_868D99', ''),
  0x869270: ('EprojInstr_9270', ''),
  0x8695ba: ('EprojInstr_95BA', ''),
  0x8695ed: ('EprojInstr_95ED', ''),
  0x869620: ('EprojInstr_9620', ''),
  0x86980e: ('EprojInstr_980E', ''),
  0x86a050: ('SetPreInstrAndRun', 'A'),
  0x86a3be: ('EprojInstr_A3BE', ''),
  0x86ab8a: ('SpawnEnemyDrops', 'aa'),
  0x86af36: ('ResetXYpos1', ''),
  0x86b269: ('SetVelTowardsSamus1', ''),
  0x86b7ea: ('SpawnTourianStatueUnlockingParticle', ''),
  0x86b7f5: ('Earthquake', ''),
  0x86b818: ('SpawnTourianStatueUnlockingParticleTail', ''),
  0x86c173: ('SwitchJump', ''),
  0x86c1b4: ('UserPalette0', ''),
  0x86caee: ('MotherBrainPurpleBreathIsActive', ''),
  0x86c42e: ('sub_86C42E', ''),
  0x86c8d0: ('Add12ToY', ''),
  0x86d1c7: ('EprojInstr_D1C7', ''),
  0x86d1ce: ('EprojInstr_D1CE', ''),
  0x86d5f2: ('SetN00bTubeShardX', 'SS'),
  0x86d5e1: ('AssignNewN00bTubeShardVelocity', ''),
  0x86d69a: ('SetXvelRandom', ''),
  0x86dc5a: ('EprojInstr_DC5A', ''),
  0x86dc61: ('SpawnEnemyDrops_0', ''),
  0x86dfea: ('EprojInstr_DFEA', ''),
  0x86e533: ('SetYVel', 'I'),
  0x86ee8b: ('QueueSfx2_9', ''),
  0x86b13e: ('sub_86B13E', ''),
  0x86b272: ('SetVelTowardsSamus2', ''),
  0x86b3b8: ('GotoIfFunc1', 'G'),
  0x86d15c: ('EprojInstr_D15C', ''),
  0x86d1b6: ('EprojInstr_D1B6', ''),
  0x86d62a: ('EprojInstr_D62A', 'H'),
  0x86dc77: ('SpawnSporesEproj', ''),
  0x86ef10: ('HandleRespawningEnemy', ''),
  0x86eeaf: ('EprojInstr_EEAF', ''),
  0x86eea3: ('QueueSfx2_B', ''),
  0x86ee97: ('QueueSfx2_24', ''),
  0x86ece3: ('EprojInstr_ECE3', 'H'),
  0x86ed17: ('EprojInstr_ED17', 'H'),
  0x86ad92: ('GotoDependingOnXDirection', 'HG'),
  0x869475: ('DisableCollisionsWithSamus', ''),
  0x86a456: ('GotoWithProbability25', 'G'),
  0x86af92: ('MoveY_Minus4', ''),
  0x86b841: ('AddToYpos', 'H'),
}

kCommandByName = {v[0] : (k, v[1]) for k, v in kEprojInstructionSet.items()}


kEprojTerminator = {0x868154, 0x868159, 0x8681AB, 0x86c173}
kEprojTerminatorEA = {0x86B818}

if 0:
  name2ea = pickle.loads(open('names.pickle', 'rb').read())
  name2ea = {k.replace('?', '') : v for k, v in name2ea.items()}
  ea2name = {v:k for k,v in name2ea.items()}
  for k, v in kEprojInstructionSet.items():
    k = k | 0x860000
    t = ea2name[k]
    name = t.replace('EprojInstr_', '')
    args, _ = v
    tostr = {1 : 'B', 2 : 'H', 3 : 'L'}
    args = ''.join(tostr[a] for a in args)
    if name.startswith('Goto'):
      args = args[:-1] + 'G'
    print("  0x%x: ('%s', '%s')," % (k, name, args))

class EprojParser(InstrParserCommon):
  forbidden_addrs = {0x868a39}

  def __init__(self):
    super().__init__()
    self.sprite_maps = set()

  def print(self, file):
    print('<?eproj\n', file = file)
    for ea, line in sorted(self.lines, key = lambda x: x[0]):
      if ea in self.label:
        print(f'{get_ea_name(ea, short_label = True)}:', file = file)
      print(line, file = file)
    print('?>', file = file)
    print('', file = file)

  def process_queue_entry(self, ea):
    assert ea & 0x8000
    while True:
      if ea in self.visited or ea in kEprojTerminatorEA:
        break

      self.visited.add(ea)
      ins = get_word(ea)
      if ins & 0x8000:
        ea_org = ea
        ins = 0x860000 | ins
        if ins not in kEprojInstructionSet:
          print(f'// Ins {ins:X} not in iset at {ea:X}')
          self.missing_isets.add(ea & 0xff0000 | ins)
          return
        name, operands = kEprojInstructionSet[ins]
        ea += 2
        r = []
        for op in operands:
          if op == 'B':
            r.append('%d' % get_byte(ea))
            ea += 1
          elif op == 'H':
            r.append('%d' % get_word(ea))
            ea += 2
          elif op == 'I':
            r.append('%d' % sex16(get_word(ea)))
            ea += 2
          elif op == 'X':
            r.append('0x%x' % get_word(ea))
            ea += 2
          elif op == 'L':
            r.append(get_ea_name(get_word(ea) + get_byte(ea+2) * 65536))
            ea += 3
          elif op == 'G':
            addr = 0x860000 | get_word(ea)
            assert addr & 0x8000
            r.append(get_ea_name(addr, short_label=True))
            self.visit(addr)
            ea += 2
          elif op == 'A':
            addr = 0x860000 | get_word(ea)
            assert addr & 0x8000
            r.append(get_ea_name(addr, short_label=True))
            ea += 2
          elif op == 'a':
            addr = 0xa00000 | get_word(ea)
            assert addr & 0x8000
            r.append(get_ea_name(addr))
            ea += 2
          elif op == 'S':
            addr = 0x8D0000 | get_word(ea)
            assert addr & 0x8000
            r.append(get_ea_name(addr))
            self.sprite_maps.add(addr)
            ea += 2
          else:
            assert 0
        self.print_line(ea_org, f'  {name}({", ".join(r)})') 
        if ins in kEprojTerminator:
          self.print_line(ea_org + 1, '')
          break
      else:
        drawp = get_word(ea + 2) | 0x8D0000
        assert drawp & 0x8000, hex(ea)
        assert drawp != 0x868000
        self.sprite_maps.add(drawp)
        self.print_line(ea, f'  {ins} ! {get_ea_name(drawp)}') 
        ea += 4
