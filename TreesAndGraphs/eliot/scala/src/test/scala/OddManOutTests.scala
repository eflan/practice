import org.specs2.mutable._

class OddManOutTests extends Specification {
  import OddManOut._

  "OddManOut" should {
    "return 5 for {1, 2, 3, 4, 5, 2, 3, 4, 1}" in {
      oddManOut(List(1, 2, 3, 4, 5, 2, 3, 4, 1)) must_== 5
    }

    "match oddManOutXOR" in {
      val nums = List(1, 2, 3, 4, 5, 2, 3, 4, 1)
      oddManOut(nums) must_== oddManOutXOR(nums)
    }
  }
}
