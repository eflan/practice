class Node:
  "Simple node structure for a linked list"

  def __init__(self, data):
    self.next = None
    self.data = data

  def get_data(self):
    return self.data

  def get_next(self):
    return self.next

  def set_next(self, node):
    self.next = node

  def set_data(self, data):
    self.data = data

  def to_string(self):
    return "<" + str(self.data) + ">"


class LinkedList:
  "Simple linked list skeleton structure"

  def __init__(self):
    self.head = None
    self.size = 0

  def set_head(self, node):
    self.head = node

  def get_head(self):
    return self.head

  def _size(self):
    return self.size

  def tail(self):
    current_node = self.head
    while (current_node.get_next()):
      current_node = current_node.get_next()
    return current_node

  def explain(self):
    if not (self.head):
      print "Empty List"
    else:
      current_node = self.head
      while (current_node):
        print current_node.to_string()
        current_node = current_node.get_next()

  def add_node(self, node):
    current_node = self.head
    if not (self.head):
      self.head = node
    else:
      while (current_node.get_next()):
        current_node = current_node.get_next()
      current_node.set_next(node)
    self.size += 1

### 2.1
  def remove_duplicates(self):
    unique_nodes = []
    current_node = self.head
    previous_node = self.head
    while (current_node):
      if current_node.get_data() in unique_nodes:
        previous_node.set_next(current_node.get_next())
        current_node = current_node.get_next()
        self.size -= 1
      else:
        unique_nodes.append(current_node.get_data())
        previous_node = current_node
        current_node = current_node.get_next()

### 2.2
  def remove_kth(self, k):
    lsize = 0
    current_node = self.head
    if not current_node:
      return
    while (current_node):
      current_node = current_node.get_next()
      lsize += 1
    if k >= lsize:
      return
    if k == lsize - 1:
      current_node = self.head.get_next()
      self.head.set_next(None)
      self.set_head(current_node)
      self.size -= 1
    else:
      count = 0
      current_node = self.head
      previous_node = self.head
      while (current_node and count < lsize - 1 - k):
        previous_node = current_node
        current_node = current_node.get_next()
        count += 1
      previous_node.set_next(current_node.get_next())
      current_node.set_next(None)
      self.size -= 1

### 2.3
  def remove(self, node):
    if not node.get_next():
      return
    else:
      node.set_data(node.get_next().get_data())
      next_node = node.get_next()
      node.set_next(next_node.get_next())
      next_node.set_next(None)
      self.size -= 1

### 2.4
  def partition(self, x):
    current_node = self.head
    if not current_node:
      return
    less_than = LinkedList()
    greater_than = LinkedList()
    while (current_node):
      if (current_node.get_data() < x):
        less_than.add_node(current_node)
        next_node = current_node.get_next()
        current_node.set_next(None)
        current_node = next_node
      else:
        greater_than.add_node(current_node)
        next_node = current_node.get_next()
        current_node.set_next(None)
        current_node = next_node
    less_than.add_node(greater_than.get_head())
    return less_than

### 2.6
  def find_cycle(self):
    visited = []
    current = self.head
    if not current:
      return None
    visited = [current.get_data()]
    while (current):
      next_node = current.get_next().get_data()
      if (next_node in visited):
        return current
      current = current.get_next()
      visited.append(current.get_data())
    print "No Cycle"
    return None

### 2.7
  def palindrome(self):
    j = self.size - 1
    current = self.head
    palindrome = True
    i = 0
    while (i < j and palindrome):
      palindrome &= self.check_position(current, j)
      j -= 1
      i += 1
      current = current.get_next()
    return palindrome

  def check_position(self, node, position):
    i = 0
    current = self.head
    while (i < position):
      current = current.get_next()
      i += 1
    return node.get_data() == current.get_data()

### 2.5
def sum_list_numbers(list1, list2):
  number_1 = create_number_from_list(list1)
  number_2 = create_number_from_list(list2)

  return create_list_number(number_1 + number_2)

# list number is in form: ones digit --> tens digit --> ... -->
def create_number_from_list(aList):
  exp = 0
  n = 0
  current = aList.get_head()
  while (current):
    n += current.get_data() * pow(10, exp)
    exp += 1
    current = current.get_next()
  return n

def create_list_number(number):
  ll = LinkedList()
  if (number == 0):
    return ll.add_node(Node(0))
  while (number):
    digit = number % 10
    ll.add_node(Node(digit))
    number /= 10
  return ll

# list number is in opposite form: most significant --> ... --> least significant
def sum_list_numbers_reversed(list1, list2):
  number_1 = create_number_from_list(list1)
  number_2 = create_number_from_list(list2)

  return create_list_number(number_1 + number_2)

def create_number_from_list_reversed(aList):
  n = 0
  current = aList.get_head()
  while (current):
    n = n * 10 + current.get_data()
    current = current.get_next()
  return n

def create_list_number_reverse(number):
  ll = LinkedList()
  if (number == 0):
    return ll.add_node(Node(0))
  current = None
  after = None
  while (number):
    digit = number % 10
    number /= 10
    current = Node(digit)
    if (after):
      current.set_next(after)
      after = current
    else:
      after = current
  ll.add(current)
  return ll


### TESTING
ll = LinkedList()
ll.explain()
print ll.__doc__
nodes = [Node(i % 3) for i in range(1, 10)]
for node in nodes:
  ll.add_node(node)
print str(ll._size()), "nodes"
ll.explain()
print "Remove Duplicates"
ll.remove_duplicates()
print str(ll._size()), "nodes"
ll.explain()
ll.remove_kth(2)
print "Remove k = 2 node"
print str(ll._size()), "nodes"
ll.explain()
print str(ll._size()), "nodes"
print "Add Node(5)"
ll.add_node(Node(5))
print str(ll._size()), "nodes"
ll.explain()
print "Remove Middle Node"
ll.remove(ll.head.get_next())
print str(ll._size()), "nodes"
ll.explain()
nodes = [Node(i % 5) for i in range(1, 10)]
for node in nodes:
  ll.add_node(node)
print "Partition around 3"
ll.explain()
partitioned = ll.partition(3)
print "Partitioned"
partitioned.explain()

print "list number is in form: ones digit --> tens digit --> ... -->\nsum 123 and 777"
l_123 = create_list_number(123)
print "First Number"
l_123.explain()
l_777 = create_list_number(777)
print "Second Number"
l_777.explain()
l_sum = sum_list_numbers(l_123, l_777)
print "Sum"
l_sum.explain()

print "List Number: Most significant to least Significant\nsum 123 and 777"
l_123 = create_list_number(123)
print "First Number"
l_123.explain()
l_777 = create_list_number(777)
print "Second Number"
l_777.explain()
l_sum = sum_list_numbers(l_123, l_777)
print "Sum"
l_sum.explain()

# Testing cycle Finder
l_cycle = LinkedList()
n1 = Node(1)
n2 = Node(2)
n3 = Node(3)
n4 = Node(4)
n1.set_next(n2)
n2.set_next(n3)
n3.set_next(n4)
n4.set_next(n2)

l_cycle.add_node(n1)
print "Cyclical list: n4 --> n2"
cycle_node = l_cycle.find_cycle()
if cycle_node:
  print cycle_node.to_string()

# Test Palindrome
p_list = LinkedList()
n1 = Node(1)
n2 = Node(2)
n3 = Node(2)
n4 = Node(1)
nodes = [n1, n2, n3, n4]
for n in nodes:
  p_list.add_node(n)
print "Palindrome"
p_list.explain()
print p_list.palindrome()

p_list_2 = LinkedList()
n1 = Node(1)
n2 = Node(2)
n3 = Node(4)
n4 = Node(5)
nodes = [n1, n2, n3, n4]
for n in nodes:
  p_list_2.add_node(n)
print "Palindrome"
p_list_2.explain()
print p_list_2.palindrome()
