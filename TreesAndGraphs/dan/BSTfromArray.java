/**
 * Make a BST from a sorted array of ints
 */

public class BSTfromArray
{
  public static BNode convertArrayToBST(int[] array, int lowIndex, int highIndex)
  {
    if (lowIndex > highIndex)
      return null;

    int midIndex = (lowIndex + highIndex) / 2;

    BNode root = new BNode(array[midIndex]);
    BNode left = convertArrayToBST(array, lowIndex, midIndex - 1);
    BNode right = convertArrayToBST(array, midIndex + 1, highIndex);

    root.setLeft(left);
    root.setRight(right);

    return root;
  }

  public static void main(String[] args)
  {
    int array[] = { 1, 7, 11, 44, 47, 60, 77};
    System.out.println("The array");
    for (int i : array)
      System.out.print(i + " ");

    BNode root = BSTfromArray.convertArrayToBST(array, 0, array.length - 1);
    BTree btree = new BTree(root);
    System.out.println("The Binary Tree");
    btree.inorderTraversal();
    System.out.println("Root: " + btree.root());
  }

}//end BSTfromArray
