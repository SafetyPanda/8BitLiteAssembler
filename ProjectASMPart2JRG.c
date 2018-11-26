//need to create the functions run the array notation.
#define _CRT_SECURE_NO_WARNINGS  // lets us use depricated code

#ifdef _unix
#define fopen_s(pFile, filename, mode) ((*(pFile))=fOpen(filename), (mode)))==NULL
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//char ASM_FILE_NAME[ ] = "project1avicki.asm";
char ASM_FILE_NAME[ ] = "/home/safetypanda/Documents/CodingProjects/assemblyproject/project1avicki.asm";
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
#define ADD 160
#define MOVMEM  224
#define CMP 96
#define PUT 7
#define GET 6
#define JMP 14
#define JE 8
#define JNE 9
#define JB 10
#define JBE 11
#define JAE 13
#define JA 12


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

    address = 0;
	bool anotherCommand = TRUE;
	int registerNumber;
	int registerAddFrom = 0;
	int addressValue;

	while (anotherCommand)
	{

		printf("Address Location: %d\n", address);
		if ((memory[address] & 224) == MOVREG) //MOV Constant value into a register
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
        else if((memory[address] & 224) == MOVMEM) //MOVING TO ANOTHER MEMORY ADDRESS
        {
			registerNumber = memory[address] & 24;

			if (registerNumber == 0)
			{
				address++;
				addressValue = memory[address];

				memory[addressValue] = regis.AX;
			}
			else if (registerNumber == 8)
			{
				address++;
				addressValue = memory[address];

				memory[addressValue] = regis.BX;
			}
			else if (registerNumber == 16)
			{
				address++;
				addressValue = memory[address];

				memory[addressValue] = regis.CX;			}
			else
			{
				address++;
				addressValue = memory[address];

				memory[addressValue] = regis.DX;
			}
			printf("Value of Memory Address: %d is: %d\n",addressValue, memory[addressValue]);
        }
        else if((memory[address] & 224) == ADD) // ADD TWO REGISTERS TOGETHER.
        {
			registerNumber = memory[address] & 24;
			registerAddFrom = memory[address] & 3;

			if (registerNumber == 0)
			{
				if (registerAddFrom == 0)
				{
					regis.AX += regis.AX;
				}
				else if (registerAddFrom == 1)
				{
					regis.AX += regis.BX;
				}
				else if (registerAddFrom == 2)
				{
					regis.AX += regis.CX;
				}
				else
				{
					regis.AX += regis.DX;
				}

			}
			else if (registerNumber == 8)
			{
				if (registerAddFrom == 0)
				{
					regis.BX += regis.AX;
				}
				else if (registerAddFrom == 1)
				{
					regis.BX += regis.BX;
				}
				else if (registerAddFrom == 2)
				{
					regis.BX += regis.CX;
				}
				else
				{
					regis.BX += regis.DX;
				}
			}
			else if (registerNumber == 16)
			{
				if (registerAddFrom == 0)
				{
					regis.CX += regis.AX;
				}
				else if (registerAddFrom == 1)
				{
					regis.CX += regis.BX;
				}
				else if (registerAddFrom == 2)
				{
					regis.CX += regis.CX;
				}
				else
				{
					regis.CX += regis.DX;
				}
			}
			else
			{
				if (registerAddFrom == 0)
				{
					regis.DX += regis.AX;
				}
				else if (registerAddFrom == 1)
				{
					regis.DX += regis.BX;
				}
				else if (registerAddFrom == 2)
				{
					regis.DX += regis.CX;
				}
				else
				{
					regis.DX += regis.DX;
				}
			}
			address++;
        }

        else if((memory[address] & 224) == CMP) //COMPARE REGISTER WITH REGISTER, ADDRESS, CONSTANT
		{

			printf("IN COMPARE\n\n");
        	registerNumber = memory[address] & 24;
			registerAddFrom = memory[address] & 3;
			printf("Register add from: %d \n", registerAddFrom);
			printf("Address Value: %d", memory[address]);


			if((memory[address] & 100) == 100) //Check to see if I'm looking at an address
			{
				address++;
				addressValue = memory[address];
				printf("%d address value\n\n", addressValue);

				if(registerNumber == AXREG)
				{
					if(regis.AX > memory[addressValue])
					{
						regis.flag = 1;
					}
					else if(regis.AX < memory[addressValue])
					{
						regis.flag = -1;
					}
					else if(regis.AX == memory[addressValue])
					{
						regis.flag = 0;
					}
					else
					{
						regis.flag = 99;
					}
				}
				else if(registerNumber == BXREG)
				{
					if(regis.BX > memory[addressValue])
					{
						regis.flag = 1;
					}
					else if(regis.BX < memory[addressValue])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
				else if(registerNumber == CXREG)
				{
					if(regis.CX > memory[addressValue])
					{
						regis.flag = 1;
					}
					else if(regis.CX < memory[addressValue])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
				else if(registerNumber == DXREG)
				{
					if(regis.DX > memory[addressValue])
					{
						regis.flag = 1;
					}
					else if(regis.DX < memory[addressValue])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
			}
			else if((memory[address] & 96) == 96) // comparing with constant
			{
				printf("comparing with constant\n");
				address++;
				if(registerNumber == AXREG)
				{
					if(regis.AX > memory[address])
					{
						regis.flag = 1;
					}
					else if(regis.AX < memory[address])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
				else if(registerNumber == BXREG)
				{
					if(regis.BX > memory[address])
					{
						regis.flag = 1;
					}
					else if(regis.BX < memory[address])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
				else if(registerNumber == CXREG)
				{
					if(regis.CX > memory[address])
					{
						regis.flag = 1;
					}
					else if(regis.CX < memory[address])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
				else
				{
					if(regis.DX > memory[address])
					{
						regis.flag = 1;
					}
					else if(regis.DX < memory[address])
					{
						regis.flag = -1;
					}
					else
					{
						regis.flag = 0;
					}
				}
			}
			else if(registerAddFrom < 3) // Comparing Two registers
			{
				printf("Comparing two registers");
				if (registerNumber == AXREG)
				{
					if (registerAddFrom == 0)
					{
						if(regis.AX > regis.AX)
						{
							regis.flag = 1;
						}
						else if(regis.AX < regis.AX)
						{
							regis.flag = -1;
						}
						else if(regis.AX == regis.AX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 1)
					{
						printf("in here");
						if(regis.AX > regis.BX)
						{

							regis.flag = 1;
						}
						else if(regis.AX < regis.BX)
						{
							regis.flag = -1;
						}
						else if(regis.AX == regis.BX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 2)
					{
						if(regis.AX > regis.CX)
						{
							regis.flag = 1;
						}
						else if(regis.AX < regis.CX)
						{
							regis.flag = -1;
						}
						else if(regis.AX == regis.CX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 3)
					{
						if(regis.AX > regis.DX)
						{
							regis.flag = 1;
						}
						else if(regis.AX < regis.DX)
						{
							regis.flag = -1;
						}
						else if(regis.AX == regis.DX)
						{
							regis.flag = 0;
						}
					}
				}

				else if (registerNumber == BXREG)
				{
					if (registerAddFrom == 0)
					{
						if(regis.BX > regis.AX)
						{
							regis.flag = 1;
						}
						else if(regis.BX < regis.AX)
						{
							regis.flag = -1;
						}
						else if(regis.BX == regis.AX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 1)
					{
						printf("in here");
						if(regis.BX > regis.BX)
						{
							regis.flag = 1;
						}
						else if(regis.BX < regis.BX)
						{
							regis.flag = -1;
						}
						else if(regis.BX == regis.BX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 2)
					{
						if(regis.BX > regis.CX)
						{
							regis.flag = 1;
						}
						else if(regis.BX < regis.CX)
						{
							regis.flag = -1;
						}
						else if(regis.BX == regis.CX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 3)
					{
						if(regis.BX > regis.DX)
						{
							regis.flag = 1;
						}
						else if(regis.BX < regis.DX)
						{
							regis.flag = -1;
						}
						else if(regis.BX == regis.DX)
						{
							regis.flag = 0;
						}
					}
				}
				else if (registerNumber == CXREG)
				{
					if (registerAddFrom == 0)
					{
						if(regis.CX > regis.AX)
						{
							regis.flag = 1;
						}
						else if(regis.CX < regis.AX)
						{
							regis.flag = -1;
						}
						else if(regis.CX == regis.AX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 1)
					{
						printf("in here");
						if(regis.CX > regis.BX)
						{
							regis.flag = 1;
						}
						else if(regis.CX < regis.BX)
						{
							regis.flag = -1;
						}
						else if(regis.CX == regis.BX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 2)
					{
						if(regis.CX > regis.CX)
						{
							regis.flag = 1;
						}
						else if(regis.CX < regis.CX)
						{
							regis.flag = -1;
						}
						else if(regis.CX == regis.CX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 3)
					{
						if(regis.CX > regis.DX)
						{
							regis.flag = 1;
						}
						else if(regis.CX < regis.DX)
						{
							regis.flag = -1;
						}
						else if(regis.CX == regis.DX)
						{
							regis.flag = 0;
						}
					}
				}
				else if (registerNumber == DXREG)
				{
					if (registerAddFrom == 0)
					{
						if(regis.DX > regis.AX)
						{
							regis.flag = 1;
						}
						else if(regis.DX < regis.AX)
						{
							regis.flag = -1;
						}
						else if(regis.DX == regis.AX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 1)
					{
						if(regis.DX > regis.BX)
						{
							regis.flag = 1;
						}
						else if(regis.DX < regis.BX)
						{
							regis.flag = -1;
						}
						else if(regis.DX == regis.BX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 2)
					{
						if(regis.DX > regis.CX)
						{
							regis.flag = 1;
						}
						else if(regis.AX < regis.CX)
						{
							regis.flag = -1;
						}
						else if(regis.DX == regis.CX)
						{
							regis.flag = 0;
						}
					}
					else if (registerAddFrom == 3)
					{
						if(regis.DX > regis.DX)
						{
							regis.flag = 1;
						}
						else if(regis.DX < regis.DX)
						{
							regis.flag = -1;
						}
						else if(regis.DX == regis.DX)
						{
							regis.flag = 0;
						}
					}
				}
			}
			address++;
		}
		else if ((memory[address] & 14) == JMP)
		{
			printf("here");
			address++;
			address = memory[address];
			printf("%d",address);
		}
		else if ((memory[address] & 5) == 5)
		{
			return;
		}
		printMemoryDump();
	}

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
            printf("line: %s\nlineSize: %d\nindex: %d\nindex2: %d\n", line, lineSize, index, index2);    //FOR DEBUGGING

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
		if (oper2[0] == '[')
		{
			machineCode = MOVMEM;
			machineCode += (whichReg(oper1[0]) << 3);

			machineCode += 4; //Adding address flag

			memory[address] = machineCode;
			address++;

			convertToNumber(oper2, 1, &oper2num);
			memory[address] = oper2num;
			address++;
		}
		if ((int)oper2[0] < 65 || (int)oper2[0] > 122)
		{
			machineCode = MOVREG;
			machineCode += (whichReg(oper1[0]) << 3);

			memory[address] = machineCode;
			address++;

			//printf("BEFORE CONVERT TO NUMBER\n");
			convertToNumber(oper2, 0, &oper2num);
			//printf("MADE IT PAST CONVERT TO NUMBER\n");
            
			memory[address] = oper2num;
            //printf("MADE IT PAST OPER2NUM\n");
			address++;
		}
	}
	else if (command[0] == 'a') //Add two Registers, Total placed in first one
	{
		machineCode = ADD;
		machineCode += (whichReg(oper1[0] << 3));
		machineCode += (whichReg(oper2[0]) + 1);
		memory[address] = machineCode;

		address++;
	}

	else if(command[0] == 'c')
	{
		machineCode = CMP;
		machineCode += (whichReg(oper1[0]) << 3);
		if (oper2[0] == '[') //compare to an address
		{
			machineCode += 4; //Adding address flag
			memory[address] = machineCode;
			address++;

			convertToNumber(oper2, 1, &oper2num);
			memory[address] = oper2num;
			address++;
		}
		else if(oper2[0] == 'a' || oper2[0] == 'b' || oper2[0] == 'c' || oper2[0] == 'd') //compare with a register
		{
			machineCode += (whichReg(oper2[0]));
			memory[address] = machineCode;
			address++;
		}
		else // Compare with a constant
		{
			memory[address] = machineCode;
			address++;

			convertToNumber(oper2, 0, &oper2num);
			memory[address] = oper2num;

			address++;
		}
	}
	else if(command[0] == 'j') //jumps
	{
		if(command[1] == 'e')
		{
			machineCode = JE;
			memory[address] = machineCode;
			address++;

			convertToNumber(oper1, 1, &oper2num);
			memory[address] = oper2num;
		}
		else if(command[1] == 'n')
		{
			machineCode = JNE;
			memory[address] = machineCode;
			address++;

			convertToNumber(oper1, 1, &oper2num);
			memory[address] = oper2num;
		}
		else if(command[1] == 'b')
		{
			if(command[2] == 'e')
			{
				machineCode = JBE;
				memory[address] = machineCode;
				address++;

				convertToNumber(oper1, 1, &oper2num);
				memory[address] = oper2num;
			}
			else
			{
				machineCode = JB;
				memory[address] = machineCode;
				address++;

				convertToNumber(oper1, 1, &oper2num);
				memory[address] = oper2num;
			}
		}
		else if(command[1] == 'a')
		{
			if(command[2] == 'e')
			{
				machineCode = JAE;
				memory[address] = machineCode;
				address++;

				convertToNumber(oper1, 1, &oper2num);
				memory[address] = oper2num;
			}
			else
			{
				machineCode = JA;
				memory[address] = machineCode;
				address++;

				convertToNumber(oper1, 1, &oper2num);
				memory[address] = oper2num;
			}
		}
		else if(command[1] == 'm')
		{
			machineCode = JMP;
			memory[address] = machineCode;
			address++;

			convertToNumber(oper1, 1, &oper2num);
			memory[address] = oper2num;


		}
		address++;
	}
	else if(command[0] == 'p') //Puts
	{
		machineCode = PUT;
		memory[address] = machineCode;
		address++;
	}
	else if(command[0] == 'g')//Gets
	{
		machineCode = GET;
		memory[address] = machineCode;
		address++;
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
