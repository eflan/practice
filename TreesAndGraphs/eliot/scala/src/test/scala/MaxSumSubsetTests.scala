import org.specs2.mutable._

class MaxSumSubsetTests extends Specification {
  import MaxSumSubset._

  "maxSumSubArray" should {
    "return 12 for [1, -3, 5, -2, 9, -8, -6, 4]" in {
      maxSumSubArray(Array(1, -3, 5, -2, 9, -8, -6, 4)) must_== 12
    }
  }
}
