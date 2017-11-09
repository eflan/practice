import BinarySearchTrees._
import org.specs2.mutable._

class BinarySearchTreesTests extends Specification {
  import BinarySearchTrees._

  "isValidBST" should {
    "return true for empty tree" in {
      isValid(EmptyBSTNode) must beTrue
    }

    "return true for unit tree" in {
      isValid(new NonEmptyBSTNode(0, EmptyBSTNode, EmptyBSTNode)) must beTrue
    }

    "return true for valid tree" in {
      /**
        *       7
        *   4     11
        * 2     8  21
        *   3
        */
      val tree = new NonEmptyBSTNode(7,
        new NonEmptyBSTNode(4,
          new NonEmptyBSTNode(2,
            EmptyBSTNode,
            new NonEmptyBSTNode(3, EmptyBSTNode, EmptyBSTNode)),
          EmptyBSTNode),
        new NonEmptyBSTNode(11,
          new NonEmptyBSTNode(8, EmptyBSTNode, EmptyBSTNode),
          new NonEmptyBSTNode(21, EmptyBSTNode, EmptyBSTNode)))

      isValid(tree) must beTrue
    }
 
    "return false for invalid tree" in {
      /**
        *       7
        *   4     11
        * 2  3   8  21
        */
      val tree = new NonEmptyBSTNode(7,
        new NonEmptyBSTNode(4,
          new NonEmptyBSTNode(2, EmptyBSTNode, EmptyBSTNode),
          new NonEmptyBSTNode(3, EmptyBSTNode, EmptyBSTNode)),
        new NonEmptyBSTNode(11,
          new NonEmptyBSTNode(8, EmptyBSTNode, EmptyBSTNode),
          new NonEmptyBSTNode(21, EmptyBSTNode, EmptyBSTNode)))

      isValid(tree) must beFalse
    }
  }
}
