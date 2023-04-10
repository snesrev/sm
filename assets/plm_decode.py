from decode_common import *

BANK = 0x84

kPlmInstructionSet = {
  0x8486b4: ('Sleep', ''),
  0x8486bc: ('Delete', ''),
  0x8486c1: ('PreInstr', 'A'),
  0x8486ca: ('ClearPreInstr', ''),
  0x8486d1: ('UNUSED_CallFunction', 'L'),
  0x8486eb: ('CallFunctionWithArg', 'LH'),
  0x84870b: ('CallFunction', 'L'),
  0x848724: ('Goto', 'G'),
  0x848729: ('UNUSED_sub_848729', 'B'),
  0x84873f: ('DecrementAndGotoIfNonzero', 'G'),
  0x84874e: ('SetTimer', 'B'),
  0x84875a: ('UNUSED_sub_84875A', 'H'),
  0x848764: ('LoadItemPlmGfx', '9BBBBBBBB'),
  0x8487e5: ('CopyFromRamToVram', 'HLH'),
  0x84880e: ('GotoIfBossBitSet', 'BG'),
  0x848821: ('UNUSED_sub_848821', 'B'),
  0x84882d: ('GotoIfEventSet', 'HG'),
  0x84883e: ('SetEvent', 'H'),
  0x848848: ('GotoIfChozoSet', 'G'),
  0x848865: ('SetRoomChozoBit', ''),
  0x84887c: ('GotoIfItemBitSet', 'G'),
  0x848899: ('SetItemBit', ''),
  0x8488b0: ('PickupBeamAndShowMessage', 'HB'),
  0x8488f3: ('PickupEquipmentAndShowMessage', 'HB'),
  0x84891a: ('PickupEquipmentAddGrappleShowMessage', 'H'),
  0x848941: ('PickupEquipmentAddXrayShowMessage', 'H'),
  0x848968: ('CollectHealthEnergyTank', 'H'),
  0x848986: ('CollectHealthReserveTank', 'H'),
  0x8489a9: ('CollectAmmoMissileTank', 'H'),
  0x8489d2: ('CollectAmmoSuperMissileTank', 'H'),
  0x8489fb: ('CollectAmmoPowerBombTank', 'H'),
  0x848a24: ('SetLinkReg', 'G'),
  0x848a2e: ('Call', 'G'),
  0x848a3a: ('Return', ''),
  0x848a40: ('UNUSED_sub_848A40', ''),
  0x848a59: ('UNUSED_sub_848A59', ''),
  0x848a72: ('GotoIfDoorBitSet', 'G'),
  0x848a91: ('IncrementDoorHitCounterAndJGE', 'BG'),
  0x848acd: ('IncrementArgumentAndJGE', 'BG'),
  0x848af1: ('SetBTS', 'B'),
  0x848b05: ('DrawPlmBlock', ''),
  0x848b17: ('DrawPlmBlock_', ''),
  0x848b55: ('ProcessAirScrollUpdate', ''),
  0x848b93: ('ProcessSolidScrollUpdate', ''),
  0x848bd1: ('QueueMusic', 'B'),
  0x848bdd: ('ClearMusicQueueAndQueueTrack', 'B'),
  0x848c07: ('QueueSfx1_Max6', 'B'),
  0x848c10: ('QueueSfx2_Max6', 'B'),
  0x848c19: ('QueueSfx3_Max6', 'B'),
  0x848c22: ('QueueSfx1_Max15', 'B'),
  0x848c2b: ('QueueSfx2_Max15', 'B'),
  0x848c34: ('QueueSfx3_Max15', 'B'),
  0x848c3d: ('QueueSfx1_Max3', 'B'),
  0x848c46: ('QueueSfx2_Max3', 'B'),
  0x848c4f: ('QueueSfx_Max3', 'B'),
  0x848c58: ('QueueSfx1_Max9', 'B'),
  0x848c61: ('QueueSfx2_Max9', 'B'),
  0x848c6a: ('QueueSfx3_Max9', 'B'),
  0x848c73: ('QueueSfx1_Max1', 'B'),
  0x848c7c: ('QueueSfx2_Max1', 'B'),
  0x848c85: ('QueueSfx3_Max1', 'B'),
  0x848c8f: ('ActivateMapStation', ''),
  0x848caf: ('ActivateEnergyStation', ''),
  0x848cd0: ('ActivateMissileStation', ''),
  0x848cf1: ('ActivateSaveStationAndGotoIfNo', 'H'),
  0x848d39: ('UNUSED_sub_848D39', ''),
  0x848d41: ('GotoIfSamusNear', 'BBG'),
  0x848d89: ('UNUSED_sub_848D89', ''),
  0x84ab00: ('MovePlmDownOneBlock', ''),
  0x84ab51: ('Scroll_0_1_Blue', ''),
  0x84ab59: ('MovePlmDownOneBlock_0', ''),
  0x84abd6: ('ABD6', ''),
  0x84ac9d: ('DealDamage_2', ''),
  0x84acb1: ('GiveInvincibility', ''),
  0x84ad43: ('Draw0x38FramesOfRightTreadmill', ''),
  0x84ad58: ('Draw0x38FramesOfLeftTreadmill', ''),
  0x84ae35: ('GotoIfSamusHealthFull', 'G'),
  0x84aebf: ('GotoIfMissilesFull', 'G'),
  0x84b00e: ('PlaceSamusOnSaveStation', ''),
  0x84b024: ('DisplayGameSavedMessageBox', ''),
  0x84b030: ('EnableMovementAndSetSaveStationUsed', ''),
  0x84b9b9: ('SetCrittersEscapedEvent', ''),
  0x84ba6f: ('JumpIfSamusHasNoBombs', 'G'),
  0x84bb25: ('MovePlmRight4Blocks', ''),
  0x84bbdd: ('ClearTrigger', ''),
  0x84bbe1: ('SpawnEproj', 'H'),
  0x84bbf0: ('WakeEprojAtPlmPos', 'H'),
  0x84be3f: ('SetGreyDoorPreInstr', ''),
  0x84cd93: ('SetBtsTo1', ''),
  0x84d155: ('FxBaseYPos_0x2D2', ''),
  0x84d2f9: ('GotoIfRoomArgLess', 'HG'),
  0x84d30b: ('SpawnFourMotherBrainGlass', 'HHHH'),
  0x84d357: ('SpawnTorizoStatueBreaking', 'H'),
  0x84d3c7: ('QueueSong1MusicTrack', ''),
  0x84d3d7: ('TransferWreckedShipChozoSpikesToSlopes', ''),
  0x84d3f4: ('TransferWreckedShipSlopesToChozoSpikes', ''),
  0x84d476: ('UNUSED_sub_84D476', ''),
  0x84d489: ('UNUSED_sub_84D489', ''),
  0x84d4be: ('nullsub_70', ''),
  0x84d525: ('EnableWaterPhysics', ''),
  0x84d52c: ('SpawnN00bTubeCrackEproj', ''),
  0x84d536: ('DiagonalEarthquake', ''),
  0x84d543: ('Spawn10shardsAnd6n00bs', ''),
  0x84d5e6: ('DisableSamusControls', ''),
  0x84d5ee: ('EnableSamusControls', ''),
  0x84d77a: ('ShootEyeDoorProjectileWithProjectileArg', 'H'),
  0x84d790: ('SpawnEyeDoorSweatEproj', 'H'),
  0x84d79f: ('SpawnTwoEyeDoorSmoke', ''),
  0x84d7b6: ('SpawnEyeDoorSmokeProjectile', ''),
  0x84d7c3: ('MoveUpAndMakeBlueDoorFacingRight', ''),
  0x84d7da: ('MoveUpAndMakeBlueDoorFacingLeft', ''),
  0x84db8e: ('DamageDraygonTurret', ''),
  0x84dbb8: ('DamageDraygonTurretFacingDownRight', ''),
  0x84dbf7: ('DamageDraygonTurretFacingUpRight', ''),
  0x84dc36: ('DamageDraygonTurret2', ''),
  0x84dc60: ('DamageDraygonTurretFacingDownLeft', ''),
  0x84dc9f: ('DamageDraygonTurretFacingUpLeft', ''),
  0x84e04f: ('DrawItemFrame0', ''),
  0x84e067: ('DrawItemFrame1', ''),
  0x84e29d: ('ClearChargeBeamCounter', ''),
  0x84e63b: ('E63B', ''),
}

kPlmTerminator = { 0x8486BC, 0x848724, 0x848A3A }

# Sometimes Sleep is used in a way assumes Sleep never wakes up,
# unless we do this we'll decode random instructions.
kPlmTerminatorAtEA = { 0x84B89A, 0x84B8DA, 0x84DB42, 0x84BB03, 0x84B7E9 }

kCommandByName = {v[0] : (k, v[1]) for k, v in kPlmInstructionSet.items()}
#for k, v in kPlmInstructionSet.items():
#  k = k | 0x840000
#  t = ea2name[k]
#  t = t.replace('PlmInstr_', '')
#  print("0x%x: ('%s', %s)," % (k, t, repr(tuple(v[0]))))

#for k, v in kPlmInstructionSet.items():
#  name, args = v
#  tostr = {1 : 'B', 2 : 'H', 3 : 'L'}
#  args = ''.join(tostr[a] for a in args)
#  if name.startswith('Goto') or k == 0x848A24 or k == 0x848A2E:
#    args = args[:-1] + 'G'
#  print("  0x%x: ('%s', '%s')," % (k, name, args))

def plm_header_size(ea):
  return 6 if (ea >= 0x84c842 and ea < 0x84c8ba) or ea in (0x84c8ca, 0x84BAF4) else 4

class PlmParser(InstrParserCommon):
  instruction_set = kPlmInstructionSet
  instruction_set_term = {}
  TAG = 'plm'

  def __init__(self):
    super().__init__()
    self.draw_cmds = {}

  def parse_draw_commands(self, ea):
    assert ea & 0x8000
    if ea in self.draw_cmds:
      return
    last = None
    r = []
    self.draw_cmds[ea] = r
    while True:
      n = get_word(ea)
      assert (n & 0x7f00) == 0
      cmd = ''
      if last != None:
        if last & 0xff != 0: cmd = f'{cmd}{sex8(last&0xff)}x '
        if last >> 8 != 0: cmd = f'{cmd}{sex8(last>>8)}y '
      cmd += 'v ' if n & 0x8000 else 'h '
      cmd += " ".join('%.4x' % get_word(ea + 2 + i * 2) for i in range(n & 0xff))
      r.append(cmd)
      ea += 2 + (n & 0xff) * 2
      last = get_word(ea)
      ea += 2
      if last == 0:
        break

  def print(self, file):
    draw_cmd_printed = set()
    output, queued = [], []
    last_insertion_pos = 0
    def inject_queued():
      if len(queued):
        queued.append('')
        output[last_insertion_pos:last_insertion_pos] = queued
        del queued[:]
    for ea, line in sorted(self.lines, key = lambda x: x[0]):
      if isinstance(line, str):
        if ea in self.label:
          output.append(f'{get_ea_name(ea, short_label = True)}:')
        output.append(line)
        if line == '':
          inject_queued()
          last_insertion_pos = len(output)
      elif line not in draw_cmd_printed:
        draw_cmd_printed.add(line)
        name = get_ea_name(line)
        for i, v in enumerate(self.draw_cmds[line]):
          t = name if i == 0 else (' ' * len(name))
          queued.append(t + ' ! ' + v)
    inject_queued()
    print('<?plm\n', file = file)
    for line in output:
      print(line, file = file)
    print('?>', file = file)
    print('', file = file)
    
  def process_queue_entry(self, ea):
    assert ea & 0x8000
    while True:
      if ea in self.visited:
        break
      self.visited.add(ea)
      ins = get_word(ea)
      if ins & 0x8000:
        ea_org = ea
        ins = 0x840000 | ins
        if ins not in kPlmInstructionSet:
          print(f'// Ins {ins:X} not in iset at {ea:X}')
          self.missing_isets.add(ea & 0xff0000 | ins)
          return
        name, operands = kPlmInstructionSet[ins]
        ea += 2
        r = []
        for op in operands:
          if op == 'B':
            r.append('%d' % get_byte(ea))
            ea += 1
          elif op == 'H':
            r.append('%d' % get_word(ea))
            ea += 2
          elif op == 'L':
            r.append(get_ea_name(get_word(ea) + get_byte(ea+2) * 65536))
            ea += 3
          elif op == 'A':
            addr = 0x840000 | get_word(ea)
            r.append(get_ea_name(addr, short_label=True))
            ea += 2
          elif op == '9':
            addr = 0x890000 | get_word(ea)
            r.append(get_ea_name(addr))
            self.blobs[addr] = 256
            ea += 2
          elif op == 'G':
            addr = 0x840000 | get_word(ea)
            r.append(get_ea_name(addr, short_label=True))
            self.visit(addr)
            ea += 2
          else:
            assert 0
        self.print_line(ea_org, f'  {name}({", ".join(r)})') 
        if ins in kPlmTerminator or ea_org in kPlmTerminatorAtEA:
          self.print_line(ea_org + 1, '')
          break
      else:
        drawp = get_word(ea + 2) | 0x840000
        self.print_line(ea, f'  {ins} ! {get_ea_name(drawp)}') 
        self.print_line(ea, drawp)
        try:
          drawp_end = self.parse_draw_commands(drawp)
        except:
          print(f'Crash while processing draw commants at {ea:X}')
          raise
        ea += 4
