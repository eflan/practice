package ArraysAndStrings

  object Strings {
    /**
      * 1. Implement an algorithm to determine if a string has all unique characters. What if you cannot use any additional data structures?
      */
    def allCharsUnique(s: String) = s.length == s.toCharArray.toList.map((c: Char) => (c, true)).toMap.keys.size

    /**
      *  2. Implement a function void reverse(char *str) in C/C++ which reverses a null terminated string.
      */

    /**
      * 3. Given two strings, write a method to determine if one is a permutation of the other.
      */
    def isPermutation(a: String, b: String) = {
      def countChars(s: String, counts: Map[Char, Int]): Map[Char, Int] = {
        if(s.isEmpty) counts
        else {
          countChars(s.tail, counts.updated(s.head, counts.getOrElse(s.head, 0) + 1))
        }
      }

      countChars(a, Map()) == countChars(b, Map())
    }

    /**
      * 4. Write a method to replace all spaces in a string with '%20'. You may assume that the string has sufficient space at the end of the string to hold the additional characters, and that you are given the 'true' length of the string. (Note: if implementing in Java, please use a character array so that you can perform this operation in place.) Example -- Input: "Mr John Smith    ", 13 Output: "Mr%20John%20Smith"
      */

    /**
      * 5. Implement a method to perform basic string compression using the counts of repeated characters. For example, the string aabcccccaaa would become a2b1c5a3. If the "compressed" string would not become smaller than the original string, your method should return the original string. You can assume the string has only upper and lower case letters (a-z).
      */

    def compress(originalString: String) = {
      def compressHelper(strToCompress: String, currentChar: Char, count: Int, compressed: String): String = {
        if (originalString.length <= compressed.length) originalString
        else if(strToCompress.isEmpty) compressed + count
        else if (strToCompress.head != currentChar) {
          compressHelper(strToCompress.tail, strToCompress.head, 1, compressed + count + strToCompress.head)
        }
        else compressHelper(strToCompress.tail, strToCompress.head, count + 1, compressed)
      }

      if(originalString.isEmpty) originalString
      else {
        compressHelper(originalString.tail, originalString.head, 1, originalString.head.toString)
      }
    }

    /**
      *  6. Given an image represented by an NxN matrix, where each pixel in the image is 4 bytes, write a method to roate the image by 90 degrees. Can you do this in place?
      *  7. Write an algorithm such that if an element in an MxN matrix is 0, its entire row and column are set to 0.
      *  8. Assume you have a method isSubString() that checks if one word is a substring of another. Given two strings, s1 and s2, write code to check is s2 is a rotation of s1 using only one call to isSubstring(). (e.g. "waterbottle" is a rotation of "erbottlewat".
      */

    /**
      * Given a pattern and a string input - find if the string follows the same pattern and return true or false.
      * Examples:
      * Pattern : "abab", input: "redblueredblue" should return true.
      * Pattern: "aaaa", input: "asdasdasdasd" should return true.
      * Pattern: "aabb", input: "xyzabcxzyabc" should return false.
      */
    def matchesRegex(pattern: String, input: String): Boolean = {
      def matches(patternIndex: Int, inputIndex: Int, patternsFound: Map[Char, String]): Boolean = {
        if (patternIndex == pattern.length && inputIndex == input.length) true
        else if (patternIndex == pattern.length || inputIndex == input.length) false
        else {
          // next char in pattern
          val c = pattern(patternIndex)

          // check if we've already matched this char in the pattern to a potential pattern string
          patternsFound.get(c) match {
            // yes, found an existing match so check if pattern string matches current position in input string
            case Some(p: String) =>
              // input string too short or pattern string doesn't match
              if (inputIndex + p.length > input.length || input.substring(inputIndex, inputIndex + p.length) != p) {
                false
              }
              // pattern string matches, move on to the next character in pattern
              else {
                matches(patternIndex + 1, inputIndex + p.length, patternsFound)
              }
            // no exiting match for pattern char
            case None => {
              // check increasingly longer substrings of input string as the pattern string for this char in the pattern
              for (i <- (inputIndex to input.length - 1)) {
                if (matches(patternIndex + 1, i + 1, patternsFound.updated(c, input.substring(inputIndex, i + 1))))
                  return true
              }

              // all possible substrings failed to match the pattern string
              false
            }
          }
        }
      }

      matches(0, 0, Map())
    }
  }