package finalProject;

import java.io.*;
import java.util.*;

//  Used to read the input files small.txt and median.txt and sequencially find the 
//      longest common sub sequence between two DNA sequences, but this version limits string length.

public class ConsistantLCS {
    static int lcs(char[] X, char[] Y, int m, int n) {
        int[][] table = new int[m + 1][n + 1];
        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0 || j == 0) {
                    table[i][j] = 0;
                } else if (X[i - 1] == Y[j - 1]) {
                    table[i][j] = table[i - 1][j - 1] + 1;
                } else {
                    table[i][j] = max(table[i - 1][j], table[i][j - 1]);
                }
            }
        }
        return table[m][n];
    }

    static int max(int x, int y) {
        if (x > y) {
            return x;
        } else {
            return y;
        }
    }
    
    public static void main(String[] args) throws FileNotFoundException {
        File file = new File("src/finalProject/median.txt"); //Change this file path as needed
        File file2 = new File("src/finalProject/medianOutLCS2.txt"); //Change this file path as needed
        Scanner in = new Scanner(file);
        String sOne = "", sTwo = "", tempOne = "", tempTwo= "";
        char[] one, two;
        PrintWriter out = new PrintWriter(file2);
        while(in.hasNextLine()) {
            while(in.hasNextLine()) {
                tempOne = in.nextLine();
                if (tempOne.charAt(0) != '>') {
                    sOne = sOne + tempOne;
                } else if (tempOne.charAt(0) == '>' && sOne.length() > 0) {
                    break;
                }
            }
            while(in.hasNextLine()) {
                tempTwo = in.nextLine();
                if (tempTwo.charAt(0) != '>') {
                    sTwo = sTwo + tempTwo;
                } else if (tempTwo.charAt(0) == '>' && sTwo.length() > 0) {
                    break;
                }
            }
            if(sOne.length() >= 128 && sTwo.length() >= 128) {
            	sOne = sOne.substring(0, 128);
            	sTwo = sTwo.substring(0, 128);
            }
            one = sOne.toCharArray();
            two = sTwo.toCharArray();
            out.println(lcs(one, two, one.length, two.length));
			sOne = ""; sTwo = ""; tempOne = ""; tempTwo= "";
        }

        out.close();
        in.close();
    }
}

