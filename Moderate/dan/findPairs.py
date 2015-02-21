## find all the pairs that sum to a target

def find_pairs(numbers, target):
  d = {}
  for number in numbers:
    if number in d:
      d[number] += 1
    else:
      d[number] = 1

  for number in numbers:
    diff = target - number
    if diff in d:
      if diff == number and d[diff] == 1:
        break
      else:
        print "(", number, ", ", diff, ")"
        d[diff] -= 1
        d[number] -= 1
        if (diff == number):
          del d[number]
        else:
          if d[diff] == 0:
            del d[diff]
          if d[number] == 0:
            del d[number]


x = [ 1, 2, 3, 4, 5, 3, 3 ]
target = 6

find_pairs(x, 6)



