=========================== p5 ======================
======= Make file written as per TA instructions
=====================================================

All files related to p5 is present in this directory.

>> To compile the code run:
$make all
 or 
$make
This will create mymutex.o mycond.o mytest.o and mytest (executable)
The output file created is mytest in case of "all" or "test". 

>> The test file is last file given under the "FILES" variable in the 
make file. To change the test file, change this value and 
run make or use:

$gcc -g newTestFile.c -o outputFileName mymutex.o mycond.o mythread.a -I. -L.


>> To clean: 
$make clean


NOTE: 
1) Since we are not allowed to change mythread.h, the test file
should inclue "mycond.h" and "mymutex.h" along with "mythread.h" for 
compilation.
2) The test file countains one test case for mutex. For condion variables,
p2 has been used. For this, p2 input is to be given. The filename is hardcoded
as p5TestInput.txt and is present in the same directory. 

Please make sure that this text file is present in the working directory where
the executable is going to be run.


=============
For queries:
Arun Sriraman 262 385 9101
Shyam Prasad 732 397 2831
Vineet 803 347 2458

