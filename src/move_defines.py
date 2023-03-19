import re, glob


def process(s):
  lines = s.splitlines()

  defines = []
  insert_pos = None

  for i, line in enumerate(lines):
    if line.startswith('#define '):
      lines[i] = None
      defines.append(line.replace('RomPtr(', 'RomFixedPtr('))

    if line.startswith('#include') and lines[i+1].strip()[:8] in ('', '#define ') and len(defines) == 0 and insert_pos == None:
      insert_pos = i + 1
  
  assert insert_pos != None
  lines = lines[:insert_pos] + ['\n'] + defines + lines[insert_pos:]

  return "\n".join(a for a in lines if a != None)


for filename in list(glob.glob('sm_*.c')):
  print(filename)
  s = open(filename, 'r').read()
  s2 = process(s)
  if s != s2:
    open(filename, 'w').write(s2)


