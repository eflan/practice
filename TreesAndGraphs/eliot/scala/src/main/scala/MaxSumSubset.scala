object MaxSumSubset {
  def maxSumSubArray(nums: Array[Int]) = {
    def maxSumSubArrayHelper(index: Int, sumToIndex: Int, minSubSumToIndex: Int, maxSubSum: Int): Int = {
      if(index >= nums.length) {
        maxSubSum
      }
      else {
        val newSumToIndex = sumToIndex + nums(index)

        var newMinSubSumToIndex = minSubSumToIndex
        if(newSumToIndex < minSubSumToIndex) {
          newMinSubSumToIndex = newSumToIndex
        }

        val newSubSum = newSumToIndex - newMinSubSumToIndex
        if(newSubSum > maxSubSum) {
          maxSumSubArrayHelper(index + 1, newSumToIndex, newMinSubSumToIndex, newSubSum)
        }
        else {
          maxSumSubArrayHelper(index + 1, newSumToIndex, newMinSubSumToIndex, maxSubSum)
        }
      }
    }

    maxSumSubArrayHelper(1, nums(0), 0, nums(0))
  }
}
