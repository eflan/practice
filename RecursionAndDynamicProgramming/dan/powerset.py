def powerset(S, pSet):
  if not S or len(S) == 0:
    if set() not in pSet:
      pSet.append(set())
    return
  if (S not in pSet):
    pSet.append(S)
  for element in S:
    copy = S.copy()
    copy.remove(element)
    powerset(copy, pSet)

S = {'a', 'b', 'c'}
s = [];

powerset(S, s)

print len(s), " elements in poweset for", S
s.sort(key=len)
for subset in s:
  print subset
