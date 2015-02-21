def zero_out(A, m, n):
  for r in range(0, m):
    for c in range(0, n):
      if A[c][r] == 0:
        zero(A, r, c, m, n)

def zero(A, r, c, m, n):
  for i in range(0, m):
    A[c][i] = 0
  for j in range(0, n):
    A[j][r] = 0


def print_matrix(M):
  for r in range(0, len(M)):
    row = ""
    for c in range(0, len(M)):
      row += str(M[c][r]) + " "
    print row

x = [[1, 1, 1],[1, 1, 1],[1, 1, 0]]
print_matrix(x)
zero_out(x, 3, 3)
print "Zeroed!"
print_matrix(x)


x = [[0, 1, 1],[1, 1, 1],[1, 1, 0]]
print ""
print_matrix(x)
zero_out(x, 3, 3)
print "Zeroed!"
print_matrix(x)
