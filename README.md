# Linear-system-solver

Using C, I created a program which parses a dataset of linear systems, solves them, properly reduces the fraction answers, and finally sorts all computed answers. These linear systems consist of two variables and two equations where all numbers included are fractions (negative numbers included).
<br><br>
For example:
```
ax + by = e
cx - dy = f 
Where a, b, c, d, e, and f are all fractions in the form of n/d (numerators and denominators can be one or two digits)
```
<br><br>
Any submitted equations are parsed and validated before being solved.

The initial steps of the program consist of file handling; accessing the data from a text file and prepping an output file for all the solved linear systems. The linear systems are then parsed from the provided dataset and the fractions are then further parsed into their own FractionType structure. The constants and variables are accessed, keeping in mind any parenthesis. The linear systems are solved while consistently reducing the numerators and denominators to eliminate the chances of overflow throughout the multiplication processes. The finalized linear systems are sorted respective to their answer sizes and output to the prepared text file.
