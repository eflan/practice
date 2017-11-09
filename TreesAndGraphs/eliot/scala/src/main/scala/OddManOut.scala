import scala.collection.immutable.HashSet

object OddManOut {
  def oddManOut(numbers: List[Int]): Int= {
    def oddManOutHelper(nums: List[Int], unmatched: HashSet[Int]) : Int = {
      if(nums.isEmpty) {
        if(unmatched.size == 1) unmatched.iterator.next()
        else throw new IllegalArgumentException("List does not contain an odd man out")
      }
      else if(unmatched.contains(nums.head)) {
        oddManOutHelper(nums.tail, unmatched - nums.head)
      }
      else {
        oddManOutHelper(nums.tail, unmatched + nums.head)
      }
    }

    oddManOutHelper(numbers, HashSet())
  }

  def oddManOutXOR(numbers: List[Int]) = numbers.foldRight(0)((n, m) => n ^ m)
}
