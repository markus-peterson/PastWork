package graphs_and_algorithms;

import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

class Graph {
	private int totalVertex;
	private LinkedList<LinkedList<Integer>> adjList;
	private LinkedList<Boolean> visited;
	private LinkedList<Integer>preNum, postNum;
	//adjacency list of edges
	public Graph() {
		totalVertex = 0;
		visited = new LinkedList<Boolean>();
		preNum = new LinkedList<Integer>();
		postNum = new LinkedList<Integer>();
	}
	public void loadAdjList() {
		Scanner in = new Scanner(System.in);
		totalVertex = in.nextInt();
		adjList = new LinkedList<LinkedList<Integer>>();
		for(int i = 0; i < totalVertex; i ++) {
			LinkedList<Integer> tmp = new LinkedList<Integer>();
			//int idx1 = in.nextInt() - 1;
			int degree = in.nextInt();
			//System.out.println("mark idx1 = " + idx1 + " degree = " + degree);
			for(int j = 0; j < degree; j ++) {
				int idx2 = in.nextInt() - 1;
				tmp.add(idx2);
			}	
			adjList.add(tmp);
		}
		in.close();
	}
	public int nextUnvisited() {
        for(int i = 0; i < visited.size(); i ++)
            if(visited.get(i) == false) return i;
        return -1;
    }
	public void preWalk() {
        visited.clear();
        for(int i = 0; i < totalVertex; i ++) {
        	visited.add(false);
        	preNum.add(0);
        	postNum.add(0);
        }
    }
	public void walk() {
        //method can be "DFS" or "BFS"
        preWalk();
        time = 1;
        int s;
        while((s = nextUnvisited()) != -1) {
        	DFS(s);
        }
    }
	public boolean ifVisited(int v) { return visited.get(v); }
	public void setVisited(int v) { 
        //set vertex v to be visited
        visited.set(v, true);
    }
	private int time;
	public void DFS(int v) {
	    preNum.set(v, time);
	    time++;
	    //e.g., preNum.add(v) for preVisit enumeration of the nodes
	    setVisited(v);
	    List<Integer> neighbors = adjList.get(v);
	    for(int i = 0; i < neighbors.size(); i ++) {
	        int v1 = neighbors.get(i);
	        if(ifVisited(v1) == false) DFS(v1); //use recursion!
	    }
	    postNum.set(v, time);
	    time++;
	}
	public void displayEnum() {
        //display enumeration of the vertices
        for(int i = 0; i < totalVertex; i ++) {
            System.out.println((i + 1) + " " + preNum.get(i) + " " + postNum.get(i));
        }
    }
}

public class DFS {
	public static void main(String[] args) {
		Graph g = new Graph();
		g.loadAdjList();
		g.walk();
		g.displayEnum();
	}
}