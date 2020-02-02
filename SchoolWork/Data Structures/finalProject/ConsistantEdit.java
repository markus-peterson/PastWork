package finalProject;

import java.io.*;
import java.util.*;

//  Used to read the input files small.txt and median.txt and sequencially find the 
//      edit distance between two DNA sequences, but this version limits string length.

public class ConsistantEdit {
    static int editDist(String str1, String str2, int m, int n) {
        int table[][] = new int[m + 1][n + 1];
        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0 || j == 0) {
                    table[i][j] = 0;
                } else if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    table[i][j] = table[i - 1][j - 1];
                } else {
                    table[i][j] = 1 + min(table[i    ][j - 1],  // Insert
                                          table[i - 1][j    ],  // Remove
                                          table[i - 1][j - 1]); // Replace
                }
            }
        }
        return table[m][n];
    }
    
    static int min(int x,int y,int z) {
        if (x <= y && x <= z) return x;
        if (y <= x && y <= z) return y;
        else return z;
    }
    
    public static void main(String[] args) throws FileNotFoundException {
        File file = new File("src/finalProject/median.txt"); //Change this file path as needed
        File file2 = new File("src/finalProject/medianOutEdit2.txt"); //Change this file path as needed
        Scanner in = new Scanner(file);
        String sOne = "", sTwo = "", tempOne = "", tempTwo= "";
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
            out.println(editDist(sOne, sTwo, sOne.length(), sTwo.length()));
            sOne = ""; sTwo = ""; tempOne = ""; tempTwo= "";
        }

        out.close();
        in.close();
    }
}
