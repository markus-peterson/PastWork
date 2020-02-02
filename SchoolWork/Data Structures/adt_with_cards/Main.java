package adt_with_cards;

import java.util.ArrayList;
import java.util.Scanner;
import java.util.Stack;

public class Main {
	
	public static int RPN(ArrayList<String> in) {
		int output = 0;
		String ops = "+-/*";
		Stack<String> nums = new Stack<>();
		
		for (String s : in) {
			if (!ops.contains(s)) {
				nums.push(s);
			} else {
				int b = Integer.parseInt(nums.pop());
				int a = Integer.parseInt(nums.pop());
				int op = ops.indexOf(s);
				if (op == 0) {
					nums.push(String.valueOf(a + b));
				} else if (op == 1) {
					nums.push(String.valueOf(a - b));
				} else if (op == 2) {
					nums.push(String.valueOf(a / b));
				} else if (op == 3) {
					nums.push(String.valueOf(a * b));
				}
				
			}
		}
		output = Integer.parseInt(nums.pop());
		
		return output;
	}
	
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		ArrayList<String> input = new ArrayList<>();
		
		while (in.hasNext()) {
			input.add(in.next().trim());
		}
		System.out.println(RPN(input));
		
		in.close();
	}
}
