package graphs_and_algorithms;

import java.util.*;

class Graph {
	private int totalVertex;
	private LinkedList<LinkedList<Integer>> adjList;
	private LinkedList<Boolean> visited;
	private ArrayList<Integer>dist;
	public Graph() {
		totalVertex = 0;
		visited = new LinkedList<Boolean>();
		dist = new ArrayList<Integer>();
	}
	public void loadAdjList() {
		Scanner in = new Scanner(System.in);
		totalVertex = in.nextInt();
		adjList = new LinkedList<LinkedList<Integer>>();
		for(int i = 0; i < totalVertex; i ++) {
			LinkedList<Integer> tmp = new LinkedList<Integer>();
			in.nextInt();
			int degree = in.nextInt();
			for(int j = 0; j < degree; j ++) {
				int idx2 = in.nextInt() - 1;
				tmp.add(idx2);
			}	
			adjList.add(tmp);
		}
		in.close();
	}
	public int nextUnvisited() {
        for(int i = 0; i < visited.size(); i ++) {
            if(visited.get(i) == false) {
            	return i;
            }
        }
        return -1;
    }
	public void preWalk() {
        visited.clear();
        dist.clear();
        for(int i = 0; i < totalVertex; i ++) {
        	visited.add(false);
        	dist.add(0);
        }
    }
	public void walk() {
        preWalk();
        BFS(0);
    }
	public boolean ifVisited(int v) {
		if (v >= 0 && v < totalVertex)
			return visited.get(v);
		else
			return true;
	}
	public void setVisited(int v) { visited.set(v, true); }
	
	public void BFS(int v) {
		int distance = 0; // Starting distance
		LinkedList<Integer> queue = new LinkedList<Integer>();
		setVisited(v); // Set the first index to be visited as it will not be set later
		queue.add(v); 
		dist.set(v, distance); // set the distance of the first vertex to be 0
		while (queue.size() != 0) {
			v = queue.poll(); // Pulls the most recently added item of the queue
			Iterator<Integer> i = adjList.get(v).listIterator(); // Gets that vertex's neighbors
			while (i.hasNext()) { // For each of the neighbors,
				int n = i.next(); // Get the next neighbor
				if (!visited.get(n)) { // and if it has not been visited yet
					visited.set(n, true); // indicate that it now has been visited
					dist.set(n, dist.get(v) + 1); // and set its distance to be one more than the parent vertexes distance
					queue.add(n); // finally add it to the queue to check its neighbors later
				}
			}
		}
	}
	
	public void displayEnum() {
		for (int i = 0; i < totalVertex; i++) {
			if (visited.get(i) == false) { // if any vertex in the list has not been visited,
				System.out.print((i + 1) + " " + -1 + "\n"); // print a -1
			} else {
				System.out.print((i + 1) + " " + dist.get(i) + "\n"); // if they have been visited, print the distance that is saved in the distance list
			}
		}
    }
}

public class ShortestPathBFS {
	public static void main(String[] args) {
		Graph g = new Graph();
		g.loadAdjList();
		g.walk();
		g.displayEnum();
	}
}