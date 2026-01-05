# Assembly-Linker

This project began as part of my Assembly course, based on the text book C and C++ Under the Hood, Second Edition.
The assignment was to create a Linker, based off the assembly compiler we use in class.

## Overview
Develop a Linker for a custom assembly language. The program takes in multiple obejct (.o) files (generated through the LCC compiler we use in calss) and output an executable file (.e)

## Usage
Compile:&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;gcc l.c -o l

Generate Object files:&emsp;lcc startup.a

Run Programs:&emsp;&emsp;&emsp;&emsp;l startup.o m1.o m2.o

## Future Plans
Expand to take in multiple assembly source code (.a) files, generate the object (.o) files enternaly, and output one executable (.e) files.
Combine this with previous Assembler and Interperter projects to allow for easy running of assembly source code in one command.
