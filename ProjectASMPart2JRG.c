//need to create the functions run the array notation.
#define _CRT_SECURE_NO_WARNINGS  // lets us use depricated code

#ifdef _unix
#define fopen_s(pFile, filename, mode) ((*(pFile))=fOpen(filename), (mode)))==NULL
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char ASM_FILE_NAME[ ] = "project1avicki.asm";
//char ASM_FILE_NAME[ ] = "/mnt/c/Users/clock/Downloads/project1avicki.asm";
#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL
#endif

#define MAX 150			// strlen of simulators memory
#define COL 7			// number of columns for output
#define LINE_SIZE 20	// For c-strings

//REGISTERS
#define AXREG 0
#define BXREG 1
#define CXREG 2
#define DXREG 3

//commands
#define HALT 5
#define MOVREG 192

//boolean
#define TRUE 1
#define FALSE 0
typedef int bool;

enum paramType { reg, mem, constant, arrayBx, arrayBxPlus, none };

typedef short int Memory;

struct Registers
{
	int AX;
	int BX;
	int CX;
	int DX;
	int instrAddr;
	int flag;
}regis;
Memory memory[MAX] = { 0 };
int address;


void runCode();	// Executes the machine code	****NEEDS WORK***
void splitCommand(char line[], char command[], char oper1[], char oper2[]);	// splits line of asm into it's three parts	****NEEDS WORK***
void convertToMachineCode(FILE *fin);	// Converts a single line of ASM to machine code	***NEEDS WORK***
void fillMemory();		// Converts the entire ASM file and stores it in memory
void printMemoryDump();	// Prints memeory with commands represented as integes

// Helper functions.
void convertToNumber(char line[], int start, int *value);	// converts a sub-string to an int
int whichReg(char regLetter);			// Returns the number of the letter registar
void changeToLowerCase(char line[]);	// Changes each character to lower case
void printMemoryDumpHex();				// Prints memory in hexedecimal

//RON's PERSONAL FUNCTIONS
void parseRegister();
int parseCommands(int parsedCommand);



int main()
{
	printMemoryDump();
	fillMemory();
    printf("BEFORE RUN CODE\n");
	runCode();
	printMemoryDump();
	printf("\n");
	system( "pause" );
	return 0;
}




/********************   runCode   ***********************
Executes the machine code

Needs to be written
-----------------------------------------------------------*/
void runCode()
{
    //printf("\nIN RUNCODE");	
    address = 0;
	bool anotherCommand = TRUE;
	int registerNumber;

	while (anotherCommand)
	{

		if ((memory[address] & 224) == 192)
		{
			registerNumber = memory[address] & 24;

			if (registerNumber == 0)
			{
				address++;
				regis.AX = memory[address];
			}
			else if (registerNumber == 8)
			{
				address++;
				regis.BX = memory[address];
			}
			else if (registerNumber == 16)
			{
				address++;
				regis.CX = memory[address];
			}
			else
			{
				address++;
				regis.DX = memory[address];

			}
		}
        //else if(memory[address] & 224) == ) //MOVING TO ANOTHER MEMORY ADDRESS
        //{
        //}

        // else if(memory[address] & 224 == ) // ADDTO REGISTERS TOGETHER.
        //{
        //}

		else if ((memory[address] & 5) == 5)
		{
			return;
		}

		address++;
	}
}

// Author: Ron Gillman
// MethodName: parseCommands
// Parameters: NONE
// Return: None (PASSES BY REFERENCE)
// Description: parses commands out of memory[address] using bitwise and
int parseCommands(int parsedCommand)
{  
   return 1;
}


// Author: Ron Gillman
// MethodName: parseRegister
// Parameters: NONE
// Return: No
// Description: bitwise &'s to get a registerNumber
void parseRegister()
{
    address = 0;
	int registerNumber = 0;
    
    printf("REG NUMBER %d\n", registerNumber);
    if ((memory[address] & 224) == 192)
    {

        registerNumber = memory[address] & 24;        

        if (registerNumber == 0)
	    {
		    address++;
		    regis.AX = memory[address];
	    }
	    else if (registerNumber == 8)
	    {
	    	address++;
	    	regis.BX = memory[address];
	    }
	    else if (registerNumber == 16)
	    {
	    	address++;
	    	regis.CX = memory[address];
	    }
	    else
	    {
	    	address++;
	   	    regis.DX = memory[address];
	    }
    }    
   // address++;
}


/********************   splitCommand   ***********************
splits line of asm into it's three parts

Needs work
-----------------------------------------------------------*/
void splitCommand(char line[], char command[], char oper1[], char oper2[])
{
    int lineSize = 0; //Get size of line 
    for (int i = 0; line[i] != NULL; i++) 
    { 
        lineSize++; 
    } 
    lineSize -= 1; //Execute if not the halt command 

    if (line[0] != 'h') 
    { 
        //Get command char 
        char *spacePtr = strchr(line, ' ');        
        //spacePtr = strchr(line, ' ');  //the pointer where the first space occurs
        int index = (int)(spacePtr - line);  //the difference between the start point and the space
        //printf("index = %d \n", index);
        strncpy(command, line, index + 1);    // move the command into the command variable
        command[index + 1] = '\0';            // add the string stopper

        if (index < lineSize)
        {
            //Get oper1 and
            int index2;
            int offset = 0;
            for (int i = (index + 1); line[i] != ' '; i++)
            {
                oper1[offset] = line[i];
                index2 = i;
                offset++;
            }
            //Add null terminator
            oper1[offset] = '\0';

            //Get oper2 if applicable
            if (offset < lineSize)
            {
                offset = 0;
                for (int i = index2 + 1; line[i] != NULL; i++)
                {
                    oper2[offset] = line[i + 1];
                    offset++;
                }
                oper2[offset] = '\0';
            }
            //printf("line: %s\nlineSize: %d\nindex: %d\nindex2: %d\n", line, lineSize, index, index2);    //FOR DEBUGGING

        }
        //printf("command: %s\n", command); 
        printf("oper1: %s\n", oper1); 
        printf("oper2: %s\n", oper2); //FOR DEBUGGING 
    } //Execute if line is halt else 
    if (line[0] == 'h') 
    { 
        strncpy(command, line, lineSize); 
        oper1[0] = '\0'; 
        oper2[0] = '\0'; 
    }
}

/********************   convertToMachineCode   ***********************
Converts a single line of ASM to machine code

Needs work
---------------------------------------------------------------------*/
void convertToMachineCode(FILE *fin)
{
    char line[LINE_SIZE];		// full command
	char command[LINE_SIZE];	// the asm commmand
	char oper1[LINE_SIZE], oper2[LINE_SIZE];	// the two operands could be empty
	int machineCode = 0;			// One line of converted asm code from the file
	int oper2num;
	
    fgets(line, LINE_SIZE, fin);
    
	if (feof(fin))
	{
        printf("IN END OF FILE\n");
		return;
	}
	
    printf("LINE: %s", line);    

    changeToLowerCase(line);

	splitCommand(line, command, oper1, oper2);
    printf ("COMMAND = %s", command);
	printf("MADE IT PAST SPLIT COMMAND\n");
    printf("COMMAND[0] = %c\n", command[0]);	
    if (command[0] == 'h')  //halt
	{
        printf("IN HALT\n");
		memory[address] = HALT;
		address++;
	}
	else if (command[0] == 'm')  //move into a register
	{
		machineCode = MOVREG;
		machineCode += (whichReg(oper1[0]) << 3);
		memory[address] = machineCode;
		address++;

		if ((int)oper2[0] < 65 || (int)oper2[0] > 122)
		{
			//printf("BEFORE CONVERT TO NUMBER\n");
			convertToNumber(oper2, 0, &oper2num);
			//printf("MADE IT PAST CONVERT TO NUMBER\n");
            
			memory[address] = oper2num;
            //printf("MADE IT PAST OPER2NUM\n");
			address++;
		}
	}
	printf("\n");
	printMemoryDump();


}

/********************   fillMemory   ***********************

changes the assembly code to machine code and places the
commands into the memory.
-----------------------------------------------------------*/
void fillMemory()
{
	address = 0;
	FILE *fin;		// File pointer for reading in the assembly code.

	//recommend changeing so you can type in file name
	fopen_s(&fin, ASM_FILE_NAME, "r");
	if (fin == NULL)
	{
		printf("Error, file didn't open\n\nExiting program...\n\n");
		system("pause");
		exit(1);
	}

	for (int i = 0; i < MAX && !feof(fin); i++)
	{
		convertToMachineCode(fin);
	}
    printf("FILLED MEMORY\n");
}

/****************************   printMemoryDump   ********************************
prints memory by number
MAX is the amount of elements in the memory array (Vicki used 100)
COL is the number of columns that are to be displayed (Vicki used 7; was originally called COLUMNS)
---------------------------------------------------------------------------------*/
void printMemoryDump()
{
	int numRows = MAX / COL + 1;	//number of rows that will print
	int carryOver = MAX % COL;		//number of columns on the bottom row
	int location;   //the current location being called
	for (int row = 0; row < numRows; row++)
	{
		location = row;
		//print(setw( 3 ) << location << "." << setw( 5 ) << memory[location];
		for (int column = 0; location < MAX&&column < COL; column++)
		{
			if (!(numRows - 1 == row && carryOver - 1 < column))
			{
				printf("%5d.%5d", location, memory[location]);
				location += (numRows - (carryOver - 1 < column));
			}
		}
		printf("\n");
	}

	printf("\n");
	printf("AX:%d\t", regis.AX);
	printf("BX:%d\t", regis.BX);
	printf("CX:%d\t", regis.CX);
	printf("DX:%d\t", regis.DX);
	printf("\n\n");
	printf("Instruction: %d\n", regis.instrAddr);
	printf("Flag: %d", regis.flag);

	printf("\n\n");
}



//-----------------------------------------------------------------------------
//**************   Helper functions   *****************************************
//-----------------------------------------------------------------------------


/*****************************   ConvertToNumber   ******************************
takes in a line (string) and converts the characters that they represent.
*	line - is the string of assembly code to convert
*	start - is the location where the line is being coverted,
		it starts at the beginning of number and it passed
		back at the next location
*	value - is the integer value of the digits in the code
---------------------------------------------------------------------------------*/
void convertToNumber(char line[], int start, int *value)
{
	char number[16];
	int negative = FALSE;
	int i = 0;
	printf("Number Is : %d\n", line[start]); 
	if (line[start] != NULL && line[start] == '-')
	{
		++start;
		negative = TRUE;
	}
	while (i < 16 && strlen(line) > start && isdigit(line[start]))
	{
		number[i] = line[start];
		i++;
		++start;
	}
	printf("Made it past While condition\n");
	number[i] = '\0';
	*value = atoi(number);
	if (negative == TRUE)
	{
		*value = -*value;// *-1;
	}
}

/*********************   whichReg   *************************
changes the letter of the registar to a number.
------------------------------------------------------------*/
int whichReg(char regLetter)
{
	if (regLetter == 'a')
	{
		return AXREG;
	}
	else if (regLetter == 'b')
	{
		return BXREG;
	}
	else if (regLetter == 'c')
	{
		return CXREG;
	}
	else if (regLetter == 'd')
	{
		return DXREG;
	}
	return -1;  //something went wrong if -1 is returned
}

/*********************   changeToLowerCase   ********************
Changes each character to lower case
----------------------------------------------------------------*/
void changeToLowerCase(char line[])
{
	int index = 0;
	while (index < strlen(line))
	{
		line[index] = tolower(line[index]);
		index++;
	}
}

/*********************   changeToLowerCase   ********************
Prints memory in hexedecimal
----------------------------------------------------------------*/
void printMemoryDumpHex()
{
	int numRows = MAX / COL + 1;	//number of rows that will print
	int carryOver = MAX % COL;		//number of columns on the bottom row
	int location;   //the current location being called
	for (int row = 0; row < numRows; row++)
	{
		location = row;
		//print(setw( 3 ) << location << "." << setw( 5 ) << memory[location];
		for (int column = 0; location < MAX && column < COL; column++)
		{
			if (!(numRows - 1 == row && carryOver - 1 < column))
			{
				printf("%5d.%3x", location, memory[location]);
				location += (numRows - (carryOver - 1 < column));
			}
		}
		printf("\n");
	}

	printf("\n");
	printf("AX:%d\t", regis.AX);
	printf("BX:%d\t", regis.BX);
	printf("CX:%d\t", regis.CX);
	printf("DX:%d\t", regis.DX);
	printf("\n\n");
	printf("Instruction: %d\n", regis.instrAddr);
	printf("Flag: %d", regis.flag);

	printf("\n\n");
}
