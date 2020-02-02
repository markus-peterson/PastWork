package rand_dna;

import java.util.Random;
import java.util.Scanner;

//Randomly generates and displays a DNA sequence of length n given as the input from stdin.

public class RandomDNA {
	
	private static String listDNA = "ATCG";
	
	public static void randDNA(int n) {
		String s = "";
		Random rnd = new Random();
		for (int i = 0; i < n; i++) {
			s = s + listDNA.charAt(rnd.nextInt(4)) + " ";
		}
		System.out.println(s);
	}
	
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		
		randDNA(in.nextInt());
		
		in.close();
	}
}