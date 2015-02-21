## find the maximum sum of sequence of positive and negative values

def find_max_sum(numbers):
  max_sum = 0
  _sum = 0

  for number in numbers:
    _sum += number
    if (max_sum < _sum):
      max_sum = _sum
    else:
      if (_sum < 0):
        _sum = 0
  return max_sum

x = [ 2, 3, -8, -1, 2, 4, -2, 3 ]

print x
print "Max Sum: %d" % find_max_sum(x)
