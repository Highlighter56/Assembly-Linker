// lshell.c
// Patrick Hughes
#include <stdio.h>    // for I/O functions
#include <stdlib.h>   // for exit()
#include <string.h>   // for string functions
#include <time.h>     // for time functions

int i, j;

unsigned short temp, inst, addr;
char buf[300];					// so we can read in lines of assembly

FILE *infile;
FILE *outfile;
char c, *p, letter;

unsigned short mca[65536];		// mca = machine code array	- stores our assembly program
int mcaindex;					// mcaindex keeps track of what line we are currently looking at

// These are our tables (acutaly just arrays)
unsigned short start;			// start is a variable instead of a table becasue thers only 1
int gotstart;					// this is representative of a boolean

unsigned short Gadd[1000];		// G Adress Table
char *Gptr[1000];				// G Pointer Table
int Gindex;						// G Index - to keep track of the size

unsigned short Eadd[1000];		// E Table
char *Eptr[1000];
int Eindex;

unsigned short eadd[1000];		// e Table
char * eptr[1000];
int eindex;

unsigned short Aadd[1000];		// A Table
int Amodadd[1000];				// A Modulus Table - this is different from the other tables becuase A represents local varalbes and its slightly different
int Aindex;

unsigned short Vadd[1000];		// V Table
char *Vptr[1000];
int Vindex;

time_t timer;

int main(int argc,char *argv[]) {
	if (argc < 2) {
		printf("Wrong number of command line arguments\n");
		printf("Usage: l <obj module name1> <obj module name2> ... \n");
		exit(1);
	}

   // display your name, command line args, time
   time(&timer);      // get time
   printf("Patrick Hughes  %s %s   %s", argv[0], argv[1], asctime(localtime(&timer)));

   //================================================================
   // Step 1:
   // For each module, store header entries into tables with adjusted
   // addresses and store machine code in mca (the machine code array).

	for (i = 1; i < argc; i++) {
		infile = fopen(argv[i], "rb");
		if (!infile) {
			printf("Cannot open %s\n", argv[i]);
			exit(1);
		}
		printf("Linking %s\n", argv[i]);
		letter = fgetc(infile);
		if (letter != 'o') {
			printf("Not a linkable file\n");
			exit(1);
		}
		while (1) {
			letter = fgetc(infile);
			if (letter == 'C')									// C - Completeness : This the end of a header
				break;
			else
				if (letter == 'S')	{							// S - Calculates the start location of the file
					if (fread(&addr, 2, 1, infile) != 1) {		// addr unsigned short
						printf("Invalid S entry\n");
						exit(1);
					}
				if (gotstart) {
					printf("More than one entry point\n");
					exit(1);
				}
				gotstart = 1;                  	 				// indicate S entry processed
				start = addr + mcaindex;        				// save adjusted address
			} else if (letter == 'G') {							// G - Global Variables
				if (fread(&addr, 2, 1, infile) != 1) {
					printf("Invalid G entry\n");
					exit(1);
				}
				Gadd[Gindex] = addr + mcaindex; 				// save adjusted address
				j = 0;
				do {                              				// get string in G entry
					letter = fgetc(infile);
					buf[j++] = letter;
				} while (letter != '\0');
				j = 0;
				while (j < Gindex) {   							// check for multiple definitions of the same global variable from different files
					if (!strcmp(buf, Gptr[j])) {
						printf("Multiple defs of global var %s\n", buf);
						exit(1);
					} else {
						j++;
					}
				}
				Gptr[Gindex++] = strdup(buf);   				// save string
			} else
			// The code for each of these tables will be similar
			if (letter == 'E') {								// E -
				// code missing here
			} else if (letter == 'e') {							// e -
				// code missing here
			} else if (letter == 'V') {							// V -
				// code missing here
			} else if (letter == 'A') {							// A is Different!
				// code missing here
			} else {
				printf("Invalid header entry %c in %s\n", letter, argv[i]);
				exit(1);
			}
		}

		//================================================================
		// Step 1.b: Add machine code to machine code array
		while(fread(&inst, 2, 1, infile)) {
			mca[mcaindex++] = inst;
		}
		fclose(infile);
	}

	// Code that Professor recomended to implement for checking purposes, before moving on to step 2
	// Check page 168 in textbook for an example of what were printing  
	// Possibly add manual prints for the tables
	for(i=0; i<mcaindex; i++) {
		printf("%04x\t%04x\t", i, mca[i]);
	}

	//================================================================
	// Step 2: Adjust external references

	// handle E references
	for (i = 0; i < Eindex; i++) {
		for (j = 0; j < Gindex; j++) {
			if(!strcmp(Eptr[i], Gptr[j]))
				break;
			}
		if (j >= Gindex) {
			printf("%s is an undefined external reference", Eptr[i]);
			exit(1);
		}
		mca[Eadd[i]] = (mca[Eadd[i]] & 0xf800) |
						((mca[Eadd[i]] + Gadd[j] - Eadd[i] - 1) & 0x7ff);
	}
	// Add code snipets are similar, just need to adjust the variables and the mask for different pcoffset sizes  
	// handle e entries
	for (i = 0; i < eindex; i++) {
		// code missing here
	}

	// handle V entries
	for (i = 0; i < Vindex; i++) {
		// code missing here
	}

	//================================================================
	// Step 3: Handle A entries

	for (i = 0; i < Aindex; i++) {
		// Code missing here. Only 1 line of code needed to handle each A entry
	}
	//================================================================
	// Step 4: Write out executable file

	outfile = fopen("link.e", "wb");
	if (!outfile) {
		printf("Cannot open output file link.e\n");
		exit(1);
	}

	// Write out file signature
	fwrite("o", 1, 1, outfile);
	
	printf("Creating executable file link.e\n");
	// Write out start entry if there is one
	if (gotstart) {
		fwrite("S", 1, 1, outfile);
		fwrite(&start, 2, 1, outfile);
	}
	// Write out G entries
	for (i = 0; i < Gindex; i++) {
		fwrite("G", 1, 1, outfile);
		fwrite(Gadd + i, 2, 1, outfile);
		fprintf(outfile, "%s", Gptr[i]);
		fwrite("", 1, 1, outfile);
	}
	// Write out V entries as A entries
	for (i = 0; i < Vindex; i++) {
		//  Code missing here:
		//  Write out V entries as A entries.
	}
	// Write out A entries
	for (i = 0; i < Aindex; i++) {                        
		// Code missing here:
		// Write out A entries.
	}
	// Terminate header
	fwrite("C", 1, 1, outfile);

	// Write out code
	for (i = 0; i < mcaindex; i++) {
		fwrite(mca + i, 2, 1, outfile);
	}
	fclose(outfile);
}