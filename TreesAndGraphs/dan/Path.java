/**
 * Does a path exist between point A and point B in a di-graph?
 */

import java.util.Stack;
import java.util.ArrayList;

public class Path
{

  public static boolean route(Graph graph, Node start, Node finish, Stack<Node> path)
  {
    boolean isPath = false;
    
    start.visit();
    
    if (start == finish)
    {
      path.push(start);
      return (isPath = true);
    }
    else
    {
      for (Node node : start.getAdjacent())
      {
        if (!node.visited())
          isPath = route(graph, node, finish, path);
        if (isPath)
        {
          path.push(node);
          break;
        }
      }
      return isPath;
    }
  }

  public static void main(String[] args)
  {
    Graph graph = new Graph();
    Stack<Node> path = new Stack<Node>();
    ArrayList<Node> nodes = new ArrayList<Node>();

    for (int i = 0; i < 5; i++)
    {
      String data = Character.toString((char) ('A' + i));
      nodes.add(new Node(data));
    }

    for (Node node : nodes)
      System.out.print(node + " ");
    System.out.println();

    //0-A, 1-B, 2-C, 3-D, 4-E
    nodes.get(0).addAdjacent(nodes.get(1)); //A --> B
    nodes.get(0).addAdjacent(nodes.get(2)); //A --> C
    nodes.get(1).addAdjacent(nodes.get(2)); //B --> C
    nodes.get(2).addAdjacent(nodes.get(0)); //C --> A
    nodes.get(2).addAdjacent(nodes.get(3)); //C --> D
    nodes.get(3).addAdjacent(nodes.get(4)); //D --> E;

    graph.setNodes(nodes);
    graph.printGraph();

    int start = 2;
    int finish = 4;

    System.out.println("Does a path from " + nodes.get(start) + " to " + nodes.get(finish) + " exists in the graph?");
    System.out.println((route(graph, nodes.get(start), nodes.get(finish), path) ? "Yes" : "No"));
  }

}// end Path
