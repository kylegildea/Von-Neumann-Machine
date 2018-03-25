/*------------------------------------------------------------------
// University of Central Florida
// CGS3269 - Fall 2016
// Program Author: Kyle Gildea

“The program is entirely my own work and I have neither developed my code together with any another person, nor copied program code from any
other person, nor permitted my code to be copied or otherwise used by any other person, nor have I
copied, modified, or otherwise used program code that you have found in any external source, including but
not limited to, online sources”.

//------------------------------------------------------------------*/

#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#define MAXPROGRAMSIZE 1000
#define BUFFERSIZE 500

typedef struct Instruction
{
  int opCode, deviceOrAddress;
  
} Instruction;

typedef struct instructionNode InstructionNode; 
typedef InstructionNode *InstructionNodePtr;

void errorPrint(char *error);
void readFile(char *fileName);
void tinyMachine(Instruction *programMemory);
void fetch(int *programCounter, int *MAR, Instruction *MDR, Instruction *IR, Instruction *programMemory);
void printHeader(int programCounter, int accumulator, int *dataMemory);


int main(int argc, char **argv)
{ 
  readFile(argv[1]); 
  
  return 0;
}

void errorPrint(char *error) 
{ 
  fprintf(stderr, "%s", error);
}

/*Reads in instructions from file*/
void readFile(char *fileName)
{  
  FILE *inputFile = fopen(fileName, "r");
  
  char *error;
  char *line;
  char *token;
  int fieldCounter = 0;
  int i = 10;
  Instruction programMemory[MAXPROGRAMSIZE]; 
  
  //Prints Error if Instruction File Not Found
  if (inputFile == NULL)
  {
    error = malloc(BUFFERSIZE * sizeof(char));
    strcpy(error, "ERROR: could not open file ");
    strcat(error, fileName);
    strcat(error, "\n\n");
    errorPrint(error);
    free(error);
  }
  
  else
  {
    line = malloc(BUFFERSIZE * sizeof(char));
    
    /*reads instructions line by line, tokenizes by whitespace, and if a valid instruction is found
    stores into array of Instructions (programMemory) starting at index 10*/
    while(fgets(line, BUFFERSIZE, inputFile) != NULL)
    {
      if(strcmp(line,"\n"))
      {
        token = strtok(line, " ");  
        
        while(token != NULL)  
         {
           
            if(fieldCounter == 0)
            {
             
              programMemory[i].opCode = atoi(token);
            }
            if(fieldCounter == 1)
            {
              programMemory[i].deviceOrAddress = atoi(token);
            }
           
           fieldCounter++;
           token = strtok(NULL, " ");
         }
         fieldCounter = 0;
         i++;
       }
    }
    //Once instructions are read in, calls simulator
    tinyMachine(programMemory);
  }
}


void tinyMachine(Instruction *programMemory)
{
  int programCounter = 10;//Program Counter starts at 10 per instructions
  int MAR; //Memory Address Register
  Instruction MDR;//Data Register for Instructions
  int accumulator = 0;//Accumulator
  int dataMemory[10] = { 0 }; //Data Register for values
  Instruction IR;
  int userInput;
  
  
  
  for(;;)
  {
    printHeader(programCounter, accumulator, dataMemory);//Prints required info for each pass
    fetch(&programCounter, &MAR, &MDR, &IR, programMemory);//Calls fetch
    
    switch(IR.opCode)//uses opcode to pick function
    {
      case 1:
      {
        //Stores IRAddress in MAR, loads value from dataMemory into Accumulator
        MAR = IR.deviceOrAddress;
        printf("/*loading memory location %d to accumulator*/\n", MAR);
        accumulator = dataMemory[MAR];
        break;
      }
      case 2:
      {
        //Stores IRAddress in MAR, adds value from dataMemory[MAR] into Accumulator stores in accumulator
        MAR = IR.deviceOrAddress;
        printf("/*loading memory location %d to be added to accumulator value %d*/\n", MAR, accumulator);
        accumulator+=dataMemory[MAR];
        break;
      }
      case 3:
      {
       //Stores IRAddress in MAR, stores value from Accumulator in dataMemory[MAR]
        MAR = IR.deviceOrAddress;
        printf("/*storing accumulator in memory location %d\n", MAR);
        dataMemory[MAR] = accumulator;
        break;
      }
      case 4:
      {
        //Stores IRAddress in MAR, subtracts value from dataMemory[MAR] from Accumulator stores in accumulator
        MAR = IR.deviceOrAddress;
        printf("/*loading memory location %d to be subtracted from accumulator value %d*/\n", MAR, accumulator);
        accumulator = accumulator - dataMemory[MAR]; 
        break;
      }
      case 5:
      {
        //Accepts User Value Input
        printf("/*input value*/\n");
        scanf("%d", &accumulator);
        break;
      }
      case 6:
      {
        //Outputs to Screen
        printf("/*outputting accumulator to screen*/\n");
        printf("%d\n\n", accumulator);
        break;
      }
      case 7:
      {
        //Does nothing, breaks loop outside of switch
        printf("/*ending program */\n");
        break;
      }
      case 8:
      {
        //sets program counter to MAR to allow loop
        MAR = IR.deviceOrAddress;
        printf("/*jumping to memory location %d*/\n", MAR);
        programCounter = MAR;
        break;
      }
      case 9:
      {
        //if zero is in accumulator, skips programCounter ahead by one to make loops and stuff
        if(accumulator == 0)
          programCounter++;  
        break;
      }
      default:
      {
        break;
      }                                    
    }
    if(IR.opCode == 7)
      break;                    
  }
}

//Simulates fetch cycle
void fetch(int *programCounter, int *MAR, Instruction *MDR, Instruction *IR, Instruction *programMemory)
{
  *MAR = *programCounter; //Moves program counter to MDR
  
  MDR->opCode = programMemory[(*MAR)].opCode;//Copies OpCode to MDR
  MDR->deviceOrAddress = programMemory[(*MAR)].deviceOrAddress;//Copies device/address to MDR
  *IR = *MDR;//Passes instructions from MDR to IR
  (*programCounter)++;//Increments program counter
}

void printHeader(int programCounter, int accumulator, int *dataMemory)
{
  //prints required header before each pass
  printf("PC = %d | A = %d, MEM = [", programCounter, accumulator);
  
  for(int i = 0; i < 10; i++)
  {
    printf("%d,", *(dataMemory + i));
  }
  printf("]\n\n");
}