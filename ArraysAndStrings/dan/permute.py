def permutation(s1, s2):
  if len(s1) != len(s2):
    return False
  s1_d = char_count(s1)
  s2_d = char_count(s2)

  for c in s2_d:
    if c not in s1_d:
      return False
    if s2_d[c] != s1_d[c]:
      return False

  return True


def char_count(s):
  d = {}
  for c in s:
    if c in d:
      d[c] += 1
    else:
      d[c] = 1
  return d


print "abcd, dbca --> ", permutation("abcd", "dbca")
print "abcd, aaaa --> ", permutation("abcd", "aaaa")
