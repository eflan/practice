# Is a string S comprised of unique characters?

def unique(S):
  d = {}
  for c in S:
    if c in d:
      d[c] += 1
    else:
      d[c] = 1

  unq = True

  for c in d:
    if d[c] > 1:
      unq = False
      break

  return unq


print "abcd --> ", unique("abcd");
print "aacd --> ", unique("aacd");


def unique_2(S):
  unique = True
  for i in range(0, len(S) - 1):
    if not unique:
      break
    for j in range(i + 1, len(S)):
      if S[i] == S[j]:
        unique = False
        break
  return unique


print "abcd --> ", unique_2("abcd");
print "aacd --> ", unique_2("aacd");
