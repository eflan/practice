def rotate_90(M):
  A = []
  for r in range(len(M) - 1, -1, -1):
    col = []
    for c in range(0, len(M)):
      col.append(M[c][r])
    A.append(col)
  return A

def print_matrix(M):
  for r in range(0, len(M)):
    row = ""
    for c in range(0, len(M)):
      row += str(M[c][r]) + " "
    print row

x = [["a", "b", "c"],["d", "e", "f"],["g", "h", "i"]]

print_matrix(x)
print "Rotate right 90 degrees"
print_matrix(rotate_90(x))


