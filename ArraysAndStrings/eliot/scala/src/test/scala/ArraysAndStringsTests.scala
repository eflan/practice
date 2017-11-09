import ArraysAndStrings._
import org.specs2.mutable._

class ArraysAndStringsTests extends Specification {
  import Strings._

  "allCharsUnique" should {
    "return true for empty string" in {
      allCharsUnique("") must beTrue
    }

    "return false for 'abba'" in {
      allCharsUnique("abba") must beFalse
    }

    "return true for 'abcd123'" in {
      allCharsUnique("abcd123") must beTrue
    }
  }

  "isPermutation" should {
    "return false for 'abc' and 'abcd'" in {
      isPermutation("abc", "abcd") must beFalse
    }

    "return true for 'abc' and 'cab'" in {
      isPermutation("abc", "abcd") must beFalse
    }
  }

  "compress" should {
    "compress aabcccccaaa to a2b1c5a3" in {
      compress("aabcccccaaa") must_== "a2b1c5a3"
    }

    "compress aabbc to aabbc" in {
      compress("aabbc") must_== "aabbc"
    }

    "compress empty string to empty string" in {
      compress("") must_== ""
    }
  }

  "matchesRegex" should {
    """Pattern : "abab", input: "redblueredblue" should return true.""" in {
      matchesRegex("abab", "redblueredblue") must beTrue
    }

    """Pattern: "aaaa", input: "asdasdasdasd" should return true.""" in {
      matchesRegex("aaaa", "asdasdasdasd") must beTrue
    }

    """Pattern: "aabb", input: "xyzabcxzyabc" should return false.""" in {
      matchesRegex("aabb", "xyzabcxzyabc") must beFalse
    }

    """Pattern: "aabb", input: "" should return false.""" in {
      matchesRegex("aabb", "") must beFalse
    }

    """Pattern: "", input: "foo" should return false.""" in {
      matchesRegex("", "foo") must beFalse
    }

    """Pattern: "abab", input: "aba" should return false.""" in {
      matchesRegex("", "foo") must beFalse
    }

    """Pattern: "abab", input: "abab" should return true.""" in {
      matchesRegex("abab", "abab") must beTrue
    }
  }
}