package dijkstra_prim;

import java.util.LinkedList;
import java.util.Scanner;

class Graph {
	private int totalVertex;
	// adjMatrix[u][v] = 1 if there is an edge from u to v, otherwise 0
	private int[][] adjMatrix;
	// adjWeight[u][v] -- the weight of the edge from u to v if there is one
	private int[][] adjWeight;
	// if a node is settled (e.g., processed in the Dijkstra's algorithm)
	private Boolean[] ifSettled;

	public Graph() {
		totalVertex = 0;
	}

	public int getTotalVertex() {
		return totalVertex;
	}

	// load graph from standard input
	public void loadAdjMatrix() {
		Scanner in = new Scanner(System.in);
		totalVertex = in.nextInt();
		adjMatrix = new int[totalVertex][totalVertex];
		adjWeight = new int[totalVertex][totalVertex];
		ifSettled = new Boolean[totalVertex];
		int i, j;
		for (i = 0; i < totalVertex; i++) {
			ifSettled[i] = false;
			for (j = 0; j < totalVertex; j++) {
				adjMatrix[i][j] = adjWeight[i][j] = 0;
			}
		}
		int u, v;
		for (i = 0; i < totalVertex; i++) {
			u = in.nextInt();
			int degree = in.nextInt();
			for (j = 0; j < degree; j++) {
				v = in.nextInt();
				adjMatrix[u][v] = 1;
				adjWeight[u][v] = in.nextInt();
			}
		}
		in.close();
	}

	// return weight of the edge from u to v if there is one
	public int getWeight(int u, int v) {
		return adjWeight[u][v];
	}

	// return neighbors of u as a LinkedList
	public LinkedList<Integer> getNeighbors(int u) {
		LinkedList<Integer> tmp = new LinkedList<Integer>();
		for (int v = 0; v < totalVertex; v++) {
			if (adjMatrix[u][v] == 1)
				tmp.add(v);
		}
		return tmp;
	}

	// return unsettled neighbors of u as a LinkedList
	public LinkedList<Integer> getUnsettledNeighbors(int u) {
		LinkedList<Integer> tmp = new LinkedList<Integer>();
		for (int v = 0; v < totalVertex; v++) {
			if ((adjMatrix[u][v] == 1) && (ifSettled[v] == false))
				tmp.add(v);
		}
		return tmp;
	}

	// return the unsettled node that has the smallest dist
	public int getUnsettledNearest(int[] dist) {
		int md = Integer.MAX_VALUE;
		int mv = -1;
		int v;
		for (v = 0; v < totalVertex; v++) {
			if ((ifSettled[v] == false) && (dist[v] < md)) {
				md = dist[v];
				mv = v;
			}
		}
		return mv;
	}

	// implement the following
	public int[] Dijkstra(int s) {
		int[] out = new int[totalVertex];
		for (int i = 0; i < totalVertex; i++) { // Initialize
			out[i] = Integer.MAX_VALUE;
		}
		out[s] = 0;
		int curr = s;
		while (curr != -1 && out[curr] != Integer.MAX_VALUE) {
			ifSettled[curr] = true;
			LinkedList<Integer> neighbors = getUnsettledNeighbors(curr);
			for(Integer i: neighbors) {
				if (out[i] > out[curr] + getWeight(curr, i)) {
					out[i] = out[curr] + getWeight(curr, i);
				}
			}
			
			curr = getUnsettledNearest(out);
		}
		return out;
	}
}

public class DijkstraMain {
	public static void main(String[] argv) {
		Graph g = new Graph();
		g.loadAdjMatrix();
		int[] dist = g.Dijkstra(0);
		for (int i = 0; i < g.getTotalVertex(); i++) {
			System.out.println(i + " " + dist[i]);
		}
	}
}