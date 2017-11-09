package BinarySearchTrees

trait BSTNode {
  def value(): Int

  def left(): BSTNode

  def right(): BSTNode

  def isEmpty(): Boolean
}

object EmptyBSTNode extends BSTNode {
  def isEmpty() = true
  def value() = 0
  def left() = EmptyBSTNode
  def right() = EmptyBSTNode
}

class NonEmptyBSTNode(v: Int, l: BSTNode, r: BSTNode) extends BSTNode {
  def isEmpty() = false
  def value() = v
  def left() = l
  def right() = r
}

/**
  *      a
  *   b    c
  * d  e  f  g
  *
  * b < a, d < b, d < a, e < a, e >= b
  * c >= a, g >= c, g >= a, f >= a, f < c
  *
  */

object BinarySearchTrees {
  def isValid(tree: BSTNode) = {
    def isValidSubTree(subtree: BSTNode, noMin: Boolean, min: Int, noMax: Boolean, max: Int): Boolean = {
      if(subtree.isEmpty) true
      else if((noMin || subtree.value > min) && (noMax || subtree.value <= max)) {
        isValidSubTree(subtree.left, noMin, min, false, subtree.value) &&
        isValidSubTree(subtree.right, false, subtree.value, noMax, max)
      }
      else false
    }

    isValidSubTree(tree, true, tree.value, true, tree.value)
  }
}
