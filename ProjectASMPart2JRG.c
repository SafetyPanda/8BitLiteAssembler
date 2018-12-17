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

//MIDDLE REGISTERS
#define AXR 0
#define BXR 8
#define CXR 16
#define DXR 24

//FULLY ACTIVATED BITS
#define HIGHBITS 224
#define MIDDLEBITS 24
#define LOWBITS 7
#define JUMPBITS 14

///////////////
/// COMMANDS //
///////////////
#define HALT 5
#define MOVREG 192 // IE AX TO DX or AX 100
#define ADD 160
#define MOVMEM  224
#define CMP 96
#define PUT 7
#define GET 6

//JUMP COMMANDS
#define JE 8
#define JNE 9
#define JB 10
#define JBE 11
#define JA 12
#define JAE 13
#define JMP 14


#define FUN 4
#define BRK 3

/////////////////////////
// Low Order Bit Stuff //
////////////////////////
#define MEMADDRESS 6
#define CONSTANT 7
#define BXADDRESS 4
#define BXPLUS 5


//FLAGS
#define ABOVE 1
#define EQUAL 0
#define BELOW -1

// Lazy Defines

#define NUMBERS line[0] == '0' || line[0] == '1' || line[0] == '2' || line[0] == '3' || line[0] == '4' || line[0] == '5' || line[0] == '6' || line[0] == '7' || line[0] == '8' || line[0] == '9'
#define REGISTERS line[1] == 'x' || line[0] == 'b' || line[0] == 'c' || line[0] == 'd'
#define NENUMBERS line[0] != '0' && line[0] != '1' && line[0] != '2' && line[0] != '3' && line[0] != '4' && line[0] != '5' && line[0] != '6' && line[0] != '7' && line[0] != '8' && line[0] != '9'
#define NEREGISTERS (line[1] != 'x') && line[0] != 'b' && line[0] != 'c' && line[0] != 'd'

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
int stackPtr = MAX;

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
int parseCommand(int instruction, int operand);
int compareInts(int one, int two);
void restoreStackValues();
void saveStackValues();

// COMMAND FUNCTIONS
void move();
void add();
void compare();
void put();
void get();
void jump();
void brk();
void function();

int main()
{
	printMemoryDump();
	fillMemory();
	int killme;
	scanf("%d", &killme);
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
	int command;
	int jmpCommand;
	//scanf("%d", &jmpCommand);

	while (anotherCommand) {
      //  scanf("%d", &jmpCommand);
        printf("Address Location: %d\n", address);
        printf("Address Value: %d\n", memory[address]);
        command = parseCommand(memory[address], 1);
        printf("COMMAND:%d\n\n", command);
        jmpCommand = parseCommand(memory[address],4);

        if (command == MOVREG || command == MOVMEM) //MOV Commands
        {
            move();
        }
        else if(command == ADD)
        {
            add();
        }
        else if(command == CMP)
        {
            compare();
        }
        else if(jmpCommand <= JMP && jmpCommand >= JE)
        {
            jump();
        }
        else if(memory[address] == PUT)
        {
            put();
        }
        else if(memory[address] == GET)
        {
            get();
        }
		else if(memory[address] == HALT)
		{
			return;
		}
		else if(memory[address] == FUN)
        {
		    printf("in here\n");
		    function();
        }
		else if(memory[address] == BRK )
        {
		    restoreStackValues();
        }
		else
        {
		    address++;
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
    if (line[0] != 'h' && line[0]!= 'p' && line[0]!= 'g' && line[0] != '\n' && line[0]!= '[' && NENUMBERS && NEREGISTERS)
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
    } //Execute if line is halt else
    if (line[0] == 'h' || line[0] == 'p' || line[0] == 'g' || line[0] == '[' || NUMBERS || REGISTERS)
    {
        strcpy(command, line);
        oper1[0] = '\0';
        oper2[0] = '\0';
    }
    if(line[0] == '\n')
    {
        command[0] = '0';
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
	int movmemAddress;
	
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
    printf("OPER1[0] = %c\n", oper1[0]);
    printf("OPER2[0] = %c\n", oper2[0]);

    if (command[0] == 'h')  //halt
	{
		memory[address] = HALT;
		address++;
	}
	if(command [0] == 'm' || command [0] == 'a' || command [0] == 'c')
    {
        if (command[0] == 'm')
	    {
            machineCode = MOVREG;
		    if (oper1[0] == '[') //Move into address location
            {

                if (oper1[1] != 'a' && oper1[1] != 'b' && oper1[1] != 'c' && oper1[1] != 'd')
                {
                    machineCode = MOVMEM;
                    convertToNumber(oper1, 1, &oper2num);
                    movmemAddress = oper2num;

                    machineCode += MEMADDRESS;
                    memory[address] = machineCode;
                    memory[address] += (whichReg(oper2[0]) << 3);

                    int regNum = whichReg(oper2[0]);
                    printf("REGNUM: %d\n", regNum);
                    address++;
                    memory[address] = movmemAddress;

                    return;
                }
                else if(oper1[3] != '+')
                {
                    machineCode += BXADDRESS;
                    memory[address] = machineCode;
                    memory[address] += (whichReg(oper2[0]) << 3);
                    address++;

                    return;
                }
                else
                {
                    machineCode += BXPLUS;
                    machineCode += (whichReg(oper2[0]) << 3);
                    memory[address] = machineCode;

                    convertToNumber(oper1, 4, &oper2num);
                    address++;
                    printf("OPER2NUM:%d\n", oper2num);
                    memory[address] = oper2num;
                    address++;

                    return;
                }
            }
		}
	    else if (command[0] == 'a') //Add two Registers, Total placed in first one
	    {
		    machineCode = ADD;
	    }

	    else if(command[0] == 'c')
	    {
		    machineCode = CMP;
	    }

        if(command[0] != 'm' && oper2[0] == '[')
        {
            if(oper2[1] != 'b')
            {
                machineCode += MEMADDRESS;

                convertToNumber(oper2, 1, &oper2num);
                movmemAddress = oper2num;

                memory[address] = machineCode;
                memory[address] += (whichReg(oper1[0]) << 3);

                address++;

                memory[address] = movmemAddress;
                address++;
            }
            else if (oper2[3] != '+')
            {
                machineCode += BXADDRESS;
                machineCode += (whichReg(oper1[0]) << 3);
                memory[address] = machineCode;

            }
            else
            {
                printf("OPER2:%s\n", oper2);
                machineCode += BXPLUS;
                machineCode += (whichReg(oper1[0]) << 3);
                memory[address] = machineCode;

                convertToNumber(oper2, 4, &oper2num);
                address++;
                printf("OPER2NUM:%d\n", oper2num);
                memory[address] = oper2num;
            }
            address++;
        }
        else if ((int)oper2[0] < 65 || (int)oper2[0] > 122)
        {

            machineCode += (whichReg(oper1[0]) << 3);

            printf("oper1:%s\n",oper1);
            machineCode += CONSTANT;

            memory[address] = machineCode;
            address++;
            convertToNumber(oper2, 0, &oper2num);

            memory[address] = oper2num;
            address++;
            printf("MACHINECODE:%d\n",machineCode);
        }
        else if(oper2[0] == 'a'|| oper2[0] == 'b' || oper2[0] == 'c' || oper2[0] == 'd')
        {
            machineCode += (whichReg(oper1[0]) << 3);
            machineCode += whichReg(oper2[0]);
            memory[address] = machineCode;
            address++;
        }
    }
	else if(command[0] == 'j') //jumps
	{
		int junk = 0;

	    if(command[1] == 'e')
		{
			machineCode = JE;
		}
		else if(command[1] == 'n')
		{
			machineCode = JNE;
		}
		else if(command[1] == 'b')
		{
			if(command[2] == 'e')
			{
				machineCode = JBE;
			}
			else
			{
				machineCode = JB;
			}
		}
		else if(command[1] == 'a')
		{
			if(command[2] == 'e')
			{
				machineCode = JAE;
			}
			else
			{
				machineCode = JA;
			}
		}
		else if(command[1] == 'm')
		{
			machineCode = JMP;
		}
        memory[address] = machineCode;
        address++;

        convertToNumber(oper1, 1, &oper2num);
        memory[address] = oper2num;
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
	else if(command[0] == 'f')
    {
	    machineCode = FUN;
        memory[address] = machineCode;


        convertToNumber(oper1, 1, &oper2num);
	    address++;
	    memory[address] = oper2num; //ADDRESS OF FUNCTION

	    address++;
        convertToNumber(oper2, 0, &oper2num);

        int parameterCount = oper2num;
        memory[address] = parameterCount; //AMOUNT OF PARAMETERS
	    address++;

        for(int i = 0; i < parameterCount; i++)
        {
            fgets(line, LINE_SIZE, fin);

            if(line[0] == '[')
            {
                if(line[1] != 'b')
                {
                    machineCode = MEMADDRESS;

                    convertToNumber(line, 1, &oper2num);
                    movmemAddress = oper2num;

                    memory[address] = machineCode;
                    address++;

                    memory[address] = movmemAddress;
                    address++;
                }
                else if (line[3] != '+')
                {
                    machineCode = BXADDRESS;
                    memory[address] = machineCode;

                }
                else
                {
                    machineCode = BXPLUS;
                    memory[address] = machineCode;

                    convertToNumber(line, 4, &oper2num);
                    address++;
                    memory[address] = oper2num;
                }
                address++;
            }
            else if ((int)line[0] < 65 || (int)line[0] > 122)
            {
                machineCode = CONSTANT;

                memory[address] = machineCode;
                address++;
                convertToNumber(line, 0, &oper2num);

                memory[address] = oper2num;
                address++;
            }
            else if(line[0] == 'a'|| line[0] == 'b' || line[0] == 'c' || line[0] == 'd')
            {
                machineCode = whichReg(line[0]);
                memory[address] = machineCode;
                address++;
                memory[address] = machineCode;
                address++;
            }
        }
    }
	else if(command[0] == 'b')
    {
	    machineCode = BRK;
	    memory[address] = machineCode;
	    address++;
    }
	else
    {
        convertToNumber(command, 0, &oper2num);
        printf("command:%d\n",command);
        memory[address] = oper2num;
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



/////////////////////////
/// Command Functions ///
/////////////////////////

void move()
{
    int middle = parseCommand(memory[address],2);
    int end = parseCommand(memory[address],3);
    int begin = parseCommand(memory[address],1);
    int junk;
    printf("VALUE OF BEGIN: %d\n",begin);
    printf("VALUE OF END: %d\n", end);
    int memValue;

    if (begin == MOVMEM)
    {
        address++;
        memValue = memory[address];

        switch(middle)
        {
            case AXR:
            {
                memory[memValue] = regis.AX;
                break;
            }
            case CXR:
            {
                memory[memValue] = regis.CX;
                break;
            }
            case DXR:
            {
                memory[memValue] = regis.DX;
                break;
            }
            default:
            {
                memory[memValue] = regis.BX;
                break;
            }
        }
    }

    else if(end == CONSTANT)
    {
        switch(middle) // MOV CONSTANT
        {
            case AXR:
            {
                address++;
                regis.AX = memory[address];
                break;
            }
            case BXR:
            {
                address++;
                regis.BX = memory[address];
                break;
            }
            case CXR:
            {
                address++;
                regis.CX = memory[address];
                break;
            }
            case DXR:
            {
                address++;
                regis.DX = memory[address];
                break;
            }
        }
    }

    else if(end == MEMADDRESS) //MOV FROM MEMADDRESS
    {
        address++;
        memValue = memory[address];
        printf("MEMVALUE:%d\n", memValue);
        switch(middle)
        {
            case AXR:
            {
                regis.AX = memory[memValue];
                break;
            }
            case BXR:
            {

                regis.BX = memory[memValue];
                break;
            }
            case CXR:
            {

                regis.CX = memory[memValue];
                break;
            }
            case DXR:
            {

                regis.DX = memory[memValue];
                break;
            }
        }
    }

    else if(end == BXADDRESS) //BX ADDRESS
    {
        memValue = regis.BX;

        switch(middle)
        {
            case AXR:
            {
                memory[memValue] = regis.AX;
                break;
            }
            case BXR:
            {

                memory[memValue] = regis.BX;
                break;
            }
            case CXR:
            {

                memory[memValue] = regis.CX;
                break;
            }
            case DXR:
            {

                memory[memValue] = regis.AX;
                break;
            }
        }
    }
    else if(end == BXPLUS) //BX PLUS
    {
        memValue = regis.BX;
        address++;
        memValue += memory[address];

        switch(middle)
        {
            case AXR:
            {
                memory[memValue] = regis.AX;
                break;
            }
            case BXR:
            {

                memory[memValue] = regis.BX;
                break;
            }
            case CXR:
            {

                memory[memValue] = regis.CX;
                break;
            }
            case DXR:
            {

                memory[memValue] = regis.DX;
                break;
            }
        }
    }
    else if(end < 4)
    {
        switch(middle)
        {
            case AXR:
            {
                if(end == BXREG)
                {
                    regis.AX = regis.BX;
                }
                else if(end == CXREG)
                {
                    regis.AX = regis.CX;
                }
                else if(end == DXREG)
                {
                    regis.AX = regis.DX;
                }
                break;
            }
            case BXR:
            {
                if(end == AXREG)
                {
                    regis.BX = regis.AX;
                }
                else if(end == CXREG)
                {
                    regis.BX = regis.CX;
                }
                else if(end == DXREG)
                {
                    regis.BX = regis.DX;
                }
                break;
            }
            case CXR:
            {
                if(end == AXREG)
                {
                    regis.CX = regis.AX;
                }
                else if(end == BXREG)
                {
                    regis.CX = regis.BX;
                }
                else if(end == DXREG)
                {
                    regis.CX = regis.DX;
                }
                break;
            }
            case DXR:
            {
                if(end == AXREG)
                {
                    regis.DX = regis.AX;
                }
                else if(end == BXREG)
                {
                    regis.DX = regis.BX;
                }
                else if(end == CXREG)
                {
                    regis.DX = regis.CX;
                }
                break;
            }
        }
    }
    address++;
}

void compare()
{
    int middle = parseCommand(memory[address],2);
    int end = parseCommand(memory[address],3);
    int memValue;

    if(end == CONSTANT)
    {
        switch(middle)
        {
            case AXR:
            {
                address++;
                regis.flag = compareInts(regis.AX,memory[address]);
                break;
            }
            case BXR:
            {
                address++;
                regis.flag = compareInts(regis.BX,memory[address]);
                break;
            }
            case CXR:
            {
                address++;
                regis.flag = compareInts(regis.CX,memory[address]);
                break;
            }
            case DXR:
            {
                address++;
                regis.flag = compareInts(regis.DX,memory[address]);
                break;
            }
        }
    }

    if(end == MEMADDRESS) //MOV FROM MEMADDRESS
    {
        address++;
        memValue = memory[address];
        printf("MEMVALUE:%d\n", memValue);
        switch(middle)
        {
            case AXR:
            {
                regis.flag = compareInts(regis.AX,memory[memValue]);
                break;
            }
            case BXR:
            {
                regis.flag = compareInts(regis.BX,memory[memValue]);
                break;
            }
            case CXR:
            {
                regis.flag = compareInts(regis.CX,memory[memValue]);
                break;
            }
            case DXR:
            {
                regis.flag = compareInts(regis.DX,memory[memValue]);
                break;
            }
        }
    }

    if(end == BXADDRESS) //BX ADDRESS
    {
        memValue = regis.BX;

        switch(middle)
        {
            case AXR:
            {
                regis.flag = compareInts(regis.AX,memory[memValue]);;
                break;
            }
            case BXR:
            {

                regis.flag = compareInts(regis.BX,memory[memValue]);;
                break;
            }
            case CXR:
            {

                regis.flag = compareInts(regis.CX,memory[memValue]);
                break;
            }
            case DXR:
            {

                regis.flag = compareInts(regis.DX,memory[memValue]);
                break;
            }
        }
    }
    if(end == BXPLUS) //BX PLUS
    {
        memValue = regis.BX;
        address++;
        memValue += memory[address];
        printf("MEMVALUE: %d\n", memValue);


        switch(middle)
        {
            case AXR:
            {
                regis.flag = compareInts(regis.AX,memory[memValue]);
                break;
            }
            case BXR:
            {

                regis.flag = compareInts(regis.BX,memory[memValue]);
                break;
            }
            case CXR:
            {

                regis.flag = compareInts(regis.CX,memory[memValue]);
                break;
            }
            case DXR:
            {

                regis.flag = compareInts(regis.DX,memory[memValue]);
                break;
            }
        }
    }
    if(end < 4) // ADD FROM REG
    {
        switch(middle)
        {
            case AXR:
            {
                if(end == BXREG)
                {
                    regis.flag = compareInts(regis.AX,regis.BX);
                }
                else if(end == CXREG)
                {
                    regis.flag = compareInts(regis.AX,regis.CX);
                }
                else if(end == DXREG)
                {
                    regis.flag = compareInts(regis.AX,regis.DX);
                }
                break;
            }
            case BXR:
            {
                if(end == AXREG)
                {
                    regis.flag = compareInts(regis.BX,regis.AX);
                }
                else if(end == CXREG)
                {
                    regis.flag = compareInts(regis.BX,regis.CX);
                }
                else if(end == DXREG)
                {
                    regis.flag = compareInts(regis.BX,regis.DX);
                }
                break;
            }
            case CXR:
            {
                if(end == AXREG)
                {
                    regis.flag = compareInts(regis.CX,regis.AX);
                }
                else if(end == BXREG)
                {
                    regis.flag = compareInts(regis.CX,regis.BX);
                }
                else if(end == DXREG)
                {
                    regis.flag = compareInts(regis.CX,regis.DX);
                }
                break;
            }
            case DXR:
            {
                if(end == AXREG)
                {
                    regis.flag = compareInts(regis.DX,regis.AX);
                }
                else if(end == BXREG)
                {
                    regis.flag = compareInts(regis.DX,regis.BX);
                }
                else if(end == CXREG)
                {
                    regis.flag = compareInts(regis.DX,regis.CX);
                }
                break;
            }
        }
    }
    address;
}

void function()
{
	int functionStart;
	int tempAddress;
	int command;
	int memoryValue;

    saveStackValues();

	address++;
	functionStart = memory[address];

	printf("%d\n",functionStart);

	address++;
	tempAddress = (functionStart - memory[address]);
    printf("%d\n",tempAddress);

	address++;

	while (tempAddress != functionStart)
    {
	    command = parseCommand(memory[address],3);
	    switch (command)
        {
            case CONSTANT:
            {
                address++;
                memory[tempAddress] = memory[address];
                break;
            }
            case BXADDRESS:
            {
                address++;
                memory[tempAddress] = regis.BX;
            }
            case BXPLUS:
            {
                address++;
                memoryValue = regis.BX;
                memoryValue+= memory[address];
                memory[tempAddress] = memoryValue;
            }
            default:
            {
                address++;
                if(command == AXREG)
                {
                    memory[tempAddress] =regis.AX;
                }
                if(command == BXREG)
                {
                    memory[tempAddress] =regis.BX;
                }
                if(command == CXREG)
                {
                    memory[tempAddress] =regis.CX;
                }
                if(command == DXREG)
                {
                    memory[tempAddress] =regis.DX;
                }
            }
        }
	    tempAddress++;
	    address++;
    }
    address = functionStart;
}

void jump()
{
    int begin = parseCommand(memory[address],4);
    int addressValue;

    address++;
    addressValue = memory[address];
    switch(begin)
    {
        case JMP:
        {
            address = addressValue;
            break;
        }
        case JE:
        {
            if(regis.flag == EQUAL)
            {
                address = addressValue;
            }
            break;
        }
        case JNE:
        {
            if(regis.flag == ABOVE || regis.flag == BELOW)
            {
                address = addressValue;
            }
            break;
        }
        case JB:
        {
            if(regis.flag == BELOW)
            {
                address = addressValue;
            }
            break;
        }
        case JBE:
        {
            if(regis.flag == BELOW || regis.flag == EQUAL)
            {
                address == addressValue;
            }
            break;
        }
        case JAE:
        {
            if(regis.flag == ABOVE || regis.flag == EQUAL)
            {
                address == addressValue;
            }
            break;
        }
        case JA:
        {
            if(regis.flag == ABOVE)
            {
                address == addressValue;
            }
            break;
        }
//        default:
//        {
//            address++;
//            break;
//        }
    }
}

void add()
{
    int middle = parseCommand(memory[address],2);
    int end = parseCommand(memory[address],3);
    int memValue;

    printf("VALUE OF MIDDLE: %d\n",middle);
    printf("VALUE OF END: %d\n", end);

    if(end == CONSTANT)
    {
        switch(middle)
        {
            case AXR:
            {
                address++;
                regis.AX += memory[address];
                break;
            }
            case BXR:
            {
                address++;
                regis.BX += memory[address];
                break;
            }
            case CXR:
            {
                address++;
                regis.CX += memory[address];
                break;
            }
            case DXR:
            {
                address++;
                regis.DX += memory[address];
                break;
            }
        }
    }

    if(end == MEMADDRESS) //MOV FROM MEMADDRESS
    {
        address++;
        memValue = memory[address];
        printf("MEMVALUE:%d\n", memValue);
        switch(middle)
        {
            case AXR:
            {
                regis.AX += memory[memValue];
                break;
            }
            case BXR:
            {
                regis.BX += memory[memValue];
                break;
            }
            case CXR:
            {
                regis.CX += memory[memValue];
                break;
            }
            case DXR:
            {
                regis.DX += memory[memValue];
                break;
            }
        }
    }

    if(end == BXADDRESS) //BX ADDRESS
    {
        memValue = regis.BX;

        switch(middle)
        {
            case AXR:
            {
                regis.AX += memory[memValue];
                break;
            }
            case BXR:
            {
                regis.BX += memory[memValue];
                break;
            }
            case CXR:
            {
                regis.CX += memory[memValue];
                break;
            }
            case DXR:
            {
                regis.DX += memory[memValue];
                break;
            }
        }
    }
    if(end == BXPLUS) //BX PLUS
    {
        memValue = regis.BX;
        address++;
        memValue += memory[address];
        printf("MEMVALUE: %d\n", memValue);


        switch(middle)
        {
            case AXR:
            {
                regis.AX += memory[memValue];
                break;
            }
            case BXR:
            {
                regis.BX += memory[memValue];
                break;
            }
            case CXR:
            {
                regis.CX += memory[memValue];
                break;
            }
            case DXR:
            {
                regis.DX += memory[memValue];
                break;
            }
        }
    }
    if(end < 4) // ADD FROM REG
    {
        switch(middle)
        {
            case AXR:
            {
                if(end == AXREG)
                {
                    regis.AX += regis.AX;
                }
                else if(end == BXREG)
                {
                    regis.AX += regis.BX;
                }
                else if(end == CXREG)
                {
                    regis.AX += regis.CX;
                }
                else if(end == DXREG)
                {
                    regis.AX += regis.DX;
                }
                break;
            }
            case BXR:
            {
                if(end == AXREG)
                {
                    regis.BX += regis.AX;
                }
                else if(end == BXREG)
                {
                    regis.BX += regis.BX;
                }
                else if(end == CXREG)
                {
                    regis.BX += regis.CX;
                }
                else if(end == DXREG)
                {
                    regis.BX += regis.DX;
                }
                break;
            }
            case CXR:
            {
                if(end == AXREG)
                {
                    regis.CX += regis.AX;
                }
                else if(end == BXREG)
                {
                    regis.CX += regis.BX;
                }
                else if(end == CXREG)
                {
                    regis.CX += regis.CX;
                }
                else if(end == DXREG)
                {
                    regis.CX += regis.DX;
                }
                break;
            }
            case DXR:
            {
                if(end == AXREG)
                {
                    regis.DX = regis.AX;
                }
                else if(end == BXREG)
                {
                    regis.DX = regis.BX;
                }
                else if(end == CXREG)
                {
                    regis.DX = regis.CX;
                }
                else if(end == DXREG)
                {
                    regis.DX = regis.DX;
                }
                break;
            }
        }
    }
    address++;
}

void put()
{
    printf("Value of AX: %d\n", regis.AX);
    address++;
}

void get()
{
    printf("Enter a value: ");
    scanf("%d", &regis.AX);
    address++;
}

void brk()
{
    restoreStackValues();
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
	printf("Register Letter in Which Reg: %c\n",regLetter);
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

/*********************   printMemoryDumpHex   ********************
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


int parseCommand(int instruction, int operand)
{
	int command; //Value of High Middle Or Low

	if(operand == 1)
	{
		command = (instruction & HIGHBITS);
		return command;
	}
	else if(operand == 2)
	{
		command = (instruction & MIDDLEBITS);
		return command;
	}
	else if(operand == 3)
	{
		command = (instruction & LOWBITS);
		return command;
	}
	else if(operand == 4) //FOR JUMP
    {
	    command = (instruction & JUMPBITS);
	    return command;
    }
}

int compareInts(int one, int two)
{

	if (one > two)
	{
		return 1;
	}
	else if (one == two)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void saveStackValues()
{
	stackPtr--;
	memory[stackPtr] = regis.AX;
	stackPtr--;
	memory[stackPtr] = regis.BX;
	stackPtr--;
	memory[stackPtr] = regis.CX;
	stackPtr--;
	memory[stackPtr] = regis.DX;
	stackPtr--;
	memory[stackPtr] = regis.flag;
	stackPtr--;

	memory[stackPtr] = (memory[address] + ((memory[address + 2] * 2 )-1)) ; //RETURN VALUE PUTS YOU AT END OF FUNCTION CALL
}

void restoreStackValues()
{
	address = memory[stackPtr];
	stackPtr++;
	regis.flag = memory[stackPtr];
	stackPtr++;

	regis.DX = memory[stackPtr];
	stackPtr++;
	regis.CX = memory[stackPtr];
	stackPtr++;
	regis.BX = memory[stackPtr];
	stackPtr++;
	regis.AX = memory[stackPtr];
	stackPtr++;
}
