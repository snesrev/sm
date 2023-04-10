import pickle

def load_names():
  xs = [a.split(' ') for a in open('names.txt', 'r').read().splitlines()]
  return {b : int(a, 16) for a, b in xs}

name2ea = load_names()
name2ea = {k.replace('?', '') : v for k, v in name2ea.items()}
ea2name = {v:k for k,v in name2ea.items()}

g_marked_addresses = set()

def mark_address(ea):
  g_marked_addresses.add(ea)
  return ea

def get_ea_name(v, short_label = False, unknown_prefix = 'unk'):
  if v == None: return None
  if v == 0: return 'null'
  r = ea2name.get(v)
  if r == None: 
    if short_label and v not in g_marked_addresses:
      r = 'lbl_%X' % (v)
    else:
      r = '%s_%X' % (unknown_prefix, v)
  return r

class Rom:
  def __init__(self):
    self.data = open('../sm.smc', 'rb').read()

  def map(self, addr):
    assert addr & 0x8000
    return (((addr >> 16) << 15) | (addr & 0x7fff)) & 0x3fffff

  def get_byte(self, addr):
    return self.data[self.map(addr)]

  def get_word(self, addr):
    addr = self.map(addr)
    return self.data[addr] + self.data[addr + 1] * 256

  def get_long(self, addr):
    addr = self.map(addr)
    return self.data[addr] + self.data[addr + 1] * 256 + self.data[addr + 2] * 65536

  def get_bytes(self, addr, n):
    addr = self.map(addr)
    return self.data[addr:addr+n]

ROM = Rom()
get_byte = ROM.get_byte
get_word = ROM.get_word


def translate_old_layout():
  for k, v in kInstructionSet.items():
      k = k | 0x8D0000
      t = ea2name[k]
      name = t.replace('PalInstr_', '')
      args, _ = v
      tostr = {1 : 'B', 2 : 'H', 3 : 'L'}
      args = ''.join(tostr[a] for a in args)
      if name.startswith('Goto'):
          args = args[:-1] + 'G'
      print("  0x%x: ('%s', '%s')," % (k, name, args))

def sex8(v):
  return v if v < 0x80 else v - 0x100
def sex16(v):
  return v if v < 0x8000 else v - 0x10000

class InstrParserCommon:
  forbidden_addrs = set()
  def __init__(self):
    self.visited = set()
    self.missing_isets = set()
    self.queue = []
    self.queue_pos = 0
    self.label = set()
    self.lines = []
    self.blobs = {}

  def print_line(self, ea, line):
    self.lines.append((ea, line))

  def visit(self, ea):
    assert ea & 0x8000
    if ea in self.forbidden_addrs:
      return
    self.label.add(ea)
    if ea not in self.visited:
      self.queue.append(ea)
  
  def process_queue(self, final = True):
    while self.queue_pos < len(self.queue):
      if (ea := self.queue[self.queue_pos]) not in self.visited:
        self.process_queue_entry(ea)
      self.queue_pos += 1
    if final:
      self.queue = None

  def print(self, file):
    print('<?%s\n' % self.TAG, file = file)
    for ea, line in sorted(self.lines, key = lambda x: x[0]):
      if ea in self.label:
        print(f'{get_ea_name(ea, short_label = True)}:', file = file)
      print(line, file = file)
    print('?>', file = file)
    print('', file = file)

def decompress(rom, ea):
    ea = rom.map(ea)
    ea_start = ea
    data = rom.data
    rv = bytearray()
    while True:
        b = data[ea]; ea += 1
        if b == 0xff:
            return rv, ea - ea_start
        if (b & 0xe0) == 0xe0:
            cmd = (8 * b) & 0xe0
            size = ((b & 3) << 8 | data[ea]) + 1; ea += 1
        else:
            cmd = b & 0xe0
            size = (b & 0x1f) + 1
        if cmd & 0x80:
            want_xor = 0xff if cmd & 0x20 else 0x00
            if cmd >= 0xc0:
                src_pos = len(rv) - data[ea]; ea += 1
            else:
                src_pos = data[ea] | data[ea + 1] * 256; ea += 2
            for i in range(size):
                rv.append(rv[src_pos + i] ^ want_xor)
        elif cmd == 0x20:
            b = data[ea]; ea += 1
            for i in range(size):
                rv.append(b)
        elif cmd == 0x40:
            b, b2 = data[ea], data[ea + 1]; ea += 2
            while size:
                rv.append(b)
                if size <= 1:
                    break
                rv.append(b2)
                size -= 2
        elif cmd == 0x60:
            b = data[ea]; ea += 1
            for i in range(size):
                rv.append(b)
                b = (b + 1) & 0xff
        else:
            for i in range(size):
                rv.append(data[ea]); ea += 1


def get_compressed_size(rom, ea):
    ea = rom.map(ea)
    ea_start = ea
    data = rom.data
    while True:
        b = data[ea]; ea += 1
        if b == 0xff:
            return ea - ea_start
        if (b & 0xe0) == 0xe0:
            cmd = (8 * b) & 0xe0
            size = ((b & 3) << 8 | data[ea]) + 1; ea += 1
        else:
            cmd = b & 0xe0
            size = (b & 0x1f) + 1
        if cmd & 0x80:
            if cmd >= 0xc0:
                ea += 1
            else:
                ea += 2
        elif cmd == 0x20:
            ea += 1
        elif cmd == 0x40:
            ea += 2
        elif cmd == 0x60:
            ea += 1
        else:
            ea += size

        

