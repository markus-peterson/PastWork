------------------
 Authors
------------------

Students in Indiana University's CSCI-C343 course, section 1609.

Markus Peterson

Robert Lucas

------------------
 Running our Code
------------------

To run the program, you must first make sure the path strings for the two file objects in the main method of the given java file we made is correct and calls the right file.
The input code is writen to take the format of the provided small.txt and median.txt, so if you use another file, have a line between desired sequences with ">" as the first character.

----------
 Findings
----------

This program studies the relationship between LCS and Edit Distance.
The input is a line depicting the DNA string to be analyzed followed by a return and the entire string with no spaces, ended with a return.
This program works by comparing the first DNA sequence with the second, the third with the forth, and so on.
With this in mind, it would be best to keep inputs in multiples of 2.
Our program outputs two lines, one of which is the LCS and the other is the Edit Distance.

Within the repository you will find our code, sample inputs, and graphs created based on the sample inputs.

**Code:**

- inEdit.java -> Our file for edit distance between strings.
- ConsistantEdit.java -> Our file for edit distance between strings, where the input has been modified as described below.
- inLCS.java -> Our file for LCS between strings.
- ConsistantLCS.java -> Our file for LCS between strings, where the input has been modified as described below.

**Text Files:**

- median.txt -> Medium file input.
- small.txt -> Small file input.
- MedianOutEdit.txt -> Medium file output for Edit Distance.
- MedianOutEdit2.txt -> Medium file output for Edit Distance after input was made the same length.
- MedianOutLCS.txt -> Medium file output for LCS.
- MedianOutLCS2.txt -> Medium file output for LCS after input was made the same length.
- SmallOutEdit.txt -> Small file output for Edit Distance.
- SmallOutLCS.txt -> Small file output for LCS.

**Graphs:**

- Results.xlxs

Our finding for the small.txt data set was that the LCS and Edit Distance were directly inversely proportional to each other.
The larger data set seems to be asymptotic in nature.

We went further to compute the LCS and Edit Distance after altering the data to make the inputs the same string length of the smaller of the two inputs.
By keeping the length of all inputs the same, this distinctly showed the relationship between to two to be inversely proportional.
We did this because the larger sequences were skewing the results since both values were increasing, not because of there relationship, but because of the change in length of the inputs.

We found that by forcing the inputs into the same length we found a more consistent relationship between the two outputs.
