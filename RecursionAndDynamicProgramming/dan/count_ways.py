steps_to_ways = {}

def count_ways(n, d):
  if (n < 0):
    return 0
  elif (n == 0):
    return 1
  elif n in d:
    return d[n]
  else:
    ways = count_ways(n - 1, d) + count_ways(n - 2, d) + count_ways(n - 3, d)
    d[n] = ways
    return ways

for i in range(0, 10):
  print "How manys to traverse %d steps? = %d" % (i, count_ways(i, steps_to_ways))

