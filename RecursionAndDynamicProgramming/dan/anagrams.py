words = ["Hello", "I", "am", "ma", "ma", "I", "like", "to", "eat", "tae", "tea", "cake", "and", "steal", "stale", "tales"]

#fill a hashtable, where hash is the letters of word in lexicographical order,
#this will be the key, store the actual word at this key

d = {}
for word in words:
  key = ''.join(sorted(word.lower()))
  if key in d:
    d[key].append(word)
  else:
    d[key] = [word]

for key in d:
  if len(d[key]) > 1:
    print (' '.join(d[key]))

