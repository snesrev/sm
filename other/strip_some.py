import re
import glob

all_data = ''
for fname in glob.glob('../sm_??.cpp'):
  all_data += open(fname).read()

assert 'Eproj_NorfairLavaquakeRocks_Func1' in all_data


for line in open('../funcs.h').read().splitlines():
  if m := re.match('^#define fn([^ ]+) (0x[0-9A-Fa-f]+)', line):
    name, addr = m.group(1), m.group(2)
    test1 = ('fn' + name)
    test2 = f'FUNC16({name})'
    if (test1 in all_data) or (test2) in all_data:
      print(line)
