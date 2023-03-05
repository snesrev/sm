import re

class FuncParser:
  def __init__(self):
    self.functions = []
    self.lines_prefix = []

  def read(self, filename):
    current_func_name = None
    state = 0
    collect = []
    c = re.compile('^(?:static )?(?:inline )?[a-zA-Z0-9_]+ \\*? *([a-zA-Z0-9_]+)\\(.*\\) {( *//.*)?$')
    for line in open(filename).read().splitlines():
#      print(line)
      line = line.rstrip()
      if m := c.match(line):
        assert state != 1
        if state == 0:
          self.lines_prefix = collect
          collect = []
        current_func_name = m.group(1)
        state = 1
      collect.append(line)

      if line == '}':
        assert state == 1
        self.functions.append((current_func_name, collect))
        collect = []
        state = 2

funcs = {}

for line in open('../funcs.h'):
  if m := re.match('^#define fn([^ ]+) (0x[0-9A-Fa-f]+)', line):
    name, addr = m.group(1), m.group(2)
    funcs[name] = addr

import glob
for fname in glob.glob('../sm_??.cpp'):
  fp = FuncParser()
  fp.read(fname)

  fout = open(fname, 'w')
  for c in fp.lines_prefix:
    print(c, file = fout)

  for a, b in fp.functions:
    if a.endswith('_Async'):
      a = a[:-6]
    cmt = funcs.get(a)
    if cmt == None:
      print('noname ', a)
    for line in b:
      if cmt != None and line.endswith('{'):
        print(line + '  // ' + cmt, file = fout)
        cmt = None
      else:
        print(line, file = fout)
  fout.close()