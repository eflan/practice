def encode(s):
  length = len(s)
  if length == 0 or length == 1:
    return s
  encoded = ""
  current_char = s[0]
  count = 1
  for i in range(1, length):
    if s[i] == current_char:
      count += 1
    else:
      encoded += current_char + str(count)
      current_char = s[i]
      count = 1

  encoded += current_char + str(count)

  return encoded if len(encoded) < length else s


print "\"\" --> %s" % encode("")
print "aabbcc --> %s" % encode("aabbcc")
print "a --> %s" % encode("a")
print "aabbbcccd --> %s" % encode("aabbbcccd")
