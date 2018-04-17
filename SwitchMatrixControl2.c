#include "cbw.h"
#include <cvirte.h>	
#include <userint.h>
#include <rs232.h>
#include <utility.h>
#include <formatio.h>
#include <string.h>
#include "SwitchMatrixControl2.h"
#include <ansi_c.h>

#define GPIOBOARD 0
// I don't know if it is actually legal to nest defines like this.
// It seems to build but might not work as expected
//#define BOARDPORT FIRSTPORTA
//#define STATUSPORT FIRSTPORTB
//#define ROWPORT FIRSTPORTCL
//#define CLOCKPORT FIRSTPORTCH

// These are the same as the commented out defines above, but not nested.
// It is probably safer, but not as clear
#define BOARDPORT  10
#define STATUSPORT 11
#define ROWPORT    12
#define CLOCKPORT  13

#define OPENCLOCK 0b01
#define CLOSECLOCK  0b10

#define SWITCHINGTIME  	0.050
#define CLOCKTIME	   	0.010
#define ADDRESSTIME		0.010
#define MULTITIME		0.020

char *fileread(char name[], char access[]);
int getInputNew(char FileInput[], int *pointer, char **line);

int initSwitchMatrix (struct SwitchMatrixConfig2_type *SwitchMatrixConfig2, char ProbeCardConfig[])
{
	// Set all ports as outputs
	cbDConfigPort(GPIOBOARD, BOARDPORT, DIGITALOUT);
	cbDConfigPort(GPIOBOARD, ROWPORT, DIGITALOUT);
	cbDConfigPort(GPIOBOARD, STATUSPORT, DIGITALOUT);			  
	cbDConfigPort(GPIOBOARD, CLOCKPORT, DIGITALOUT);
	
	//Initialize switch matrix
	//FILE *file = NULL;
    char *record = NULL, access[3], *line = NULL, *InputFile=NULL, probeCardInitPins[MaxRelays][3];
    char fileName[1024];
    int i, counter, filepointer,numProbecardConnections,lineLength;
	struct RelayAddress_type MatrixRelays[MaxRelays];
	
	for (int i = 0;i < MaxRelays; i++) {
		probeCardInitPins[i][0] = 0;
	}
    
    for (counter=0; counter<MaxRelays; counter++) {  				//Initialize arrays to the unused values
        SwitchMatrixConfig2->BoardAddresses[counter] = Unused;
		SwitchMatrixConfig2->RelayAddress[counter].relayRow = Unused;
		SwitchMatrixConfig2->RelayAddress[counter].rowIndex = Unused;
		
		for (int i = 0;i < MaxInputs; i++) {
			SwitchMatrixConfig2->Connections[i][counter] = (unsigned char) DisConnect;
		}
    }
    
    //Start by reading in the switch matrix configuration
    //Open File and read its contents into InputFile
    strcpy(access,"r");
    strcpy(fileName, "SwitchMatrixConfig2.csv");
    InputFile = fileread(fileName, access);
    
    filepointer = 0;
    counter = 0;
    lineLength=getInputNew(InputFile, &filepointer, &line); 		//Read the first Excel line containing card addresses
    record = strtok(line,","); 										//breaks string "line" into a series of tokens using the delimiter ",".
    record = strtok(NULL,","); 										//Get next field to start board addresses
    
	// Read in the card addresses
    while(record != NULL) {
        SwitchMatrixConfig2->BoardAddresses[counter] = atoi(record);
        counter++;
        record = strtok(NULL,","); 									//Get next field
    }
    SwitchMatrixConfig2->numBoards = counter;
    
    lineLength=getInputNew(InputFile, &filepointer, &line); 		//Skips the next Excel line with Column Labels
    
    counter = 0;
    while((lineLength=getInputNew(InputFile, &filepointer, &line))!=0)
    {
        // Check for end of file
        if (lineLength == EOF)
            break;
        
        record = strtok(line,","); 									//breaks string "line" into a series of tokens using the delimiter ",".
        //Reading pin assignments
        record = strtok(NULL,","); 									//skip probe card pin number and get card connector assignment
        strcpy(SwitchMatrixConfig2->CardConnectPins[counter],record);
        record = strtok(NULL,","); 									//Get relay row number corresponding to card connector assignment
        MatrixRelays[counter].relayRow = atoi(record);
		record = strtok(NULL,","); 									//Get row index number corresponding to card connector assignment 
		MatrixRelays[counter].rowIndex = atoi(record);				
        counter++;
    }
    numProbecardConnections = counter; 								//save the number of connections to the probe card
    
    // Free data from switch matrix configuration file
    free(InputFile);
    InputFile = NULL;
    free(line);
    line = NULL;
    
    //Now read in probe card configuration
    strcpy(SwitchMatrixConfig2->ProbeCardSetupFile, ProbeCardConfig);
    //Open File and read its contents into InputFile
    strcpy(access,"r");
    InputFile = fileread(ProbeCardConfig, access);
    
    filepointer = 0;
    lineLength=getInputNew(InputFile, &filepointer, &line); 	//Skips the first Excel line with Column Labels
    
    counter = 0;
    while((lineLength=getInputNew(InputFile, &filepointer, &line))!=0)
    {
        // Check for end of file
        if (lineLength == EOF)
            break;
        
        record = strtok(line,","); 							   //breaks string "line" into a series of tokens using the delimiter ",".
        
		//Reading pin assignments
        record = strtok(NULL,","); 				               //skip probe card pin number(Should be in order starting at 1), get probe card connector pin
        strcpy(probeCardInitPins[counter],record);
        counter++;
    }
    SwitchMatrixConfig2->numProbePins = counter; 			   //Save the number of pins used in the probe card
    
    // Free data from probe card file
    free(InputFile);
    InputFile = NULL;
    free(line);
    line = NULL;
    
    //Now match probe card pins to corresponding relay number
    for (counter=0; counter<SwitchMatrixConfig2->numProbePins; counter++) { 		//loop over the probe pins						%
        for (i=0; i<numProbecardConnections; i++) { 								//Loop over the possible connections, looking for a match
            if (!strcmp(probeCardInitPins[counter],SwitchMatrixConfig2->CardConnectPins[i])) {
                SwitchMatrixConfig2->RelayAddress[counter] = MatrixRelays[i]; 		//save relay number corresponding to probe pin
                break;
            }							 
        }		
    }						  
  							   
	return 0;				   
}							   

void switchMatrix(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2, int inputNumber, int pinNumber, int action)
{
	int rowNumber = SwitchMatrixConfig2->RelayAddress[pinNumber-1].relayRow;
	int rowIndex = SwitchMatrixConfig2->RelayAddress[pinNumber-1].rowIndex;
	SwitchMatrixConfig2->Connections[inputNumber-1][pinNumber-1] = action;
	
	switch (action) {
		case Connect:
			closeRelay(SwitchMatrixConfig2->BoardAddresses[inputNumber-1], rowNumber, rowIndex);
			break;
		case DisConnect:
			openRelay(SwitchMatrixConfig2->BoardAddresses[inputNumber-1], rowNumber, rowIndex);
			break;
	}
}

void setRelaysBBM(int boardAddress, int rowAddress, int *setMask)
{
	int resetMask[8];
	for(int i = 0;i < 8; i++) {
		resetMask[i] = !setMask[i];
	}
	
	openRelays(boardAddress, rowAddress, resetMask); 
	closeRelays(boardAddress, rowAddress, setMask);
}

void setRelaysMBB(int boardAddress, int rowAddress, int *setMask)
{
	int resetMask[8];
	for(int i = 0;i < 8; i++) {
		resetMask[i] = !setMask[i];
	}
	
	closeRelays(boardAddress, rowAddress, setMask);
	openRelays(boardAddress, rowAddress, resetMask);
}

uint8_t convertRowMask(int *rowMask)
{
	uint8_t	rowMaskNumber = 0;
	
	//convert rowMask array into a number rowMaskNumber. Conversion assumes array was binary
	for(int i=0; i<8; i++){
		rowMaskNumber += rowMask[i] * (1 << i);
	}
	return rowMaskNumber;
}

// boardAddress: 8 bit address of the board with relays to change
// rowAddress: 3 bit address of the row with relays to change
// rowMask: 8 element array of binary values. The relays with
//          value '1' will be set or reset according to `clockBit`
void changeRelays(int boardAddress, int rowAddress, int *rowMask, int clockBit)
{
	uint8_t rowMaskNumber = convertRowMask(rowMask);
	
	printf("Setting relays...\n");

	printf("Board Address: %d\n", boardAddress);
	printf("Row Address: %d\n", rowAddress);
	printf("Row Mask %d\n", rowMaskNumber);
	
	//Address the board and row		
	cbDOut(GPIOBOARD, BOARDPORT, boardAddress);
	cbDOut(GPIOBOARD, ROWPORT, rowAddress);
	
	// Flag flip flops to change
	cbDOut(GPIOBOARD, STATUSPORT, rowMaskNumber);
	Delay(ADDRESSTIME);
	
	//Turn on masked flip flops
	cbDOut(GPIOBOARD, CLOCKPORT, clockBit);
	Delay(CLOCKTIME);
	cbDOut(GPIOBOARD, CLOCKPORT, 0);
	
	// Switching Delay
	Delay(SWITCHINGTIME - CLOCKTIME);
	
	// Turn off all flip flops in the row
	cbDOut(GPIOBOARD, STATUSPORT, 0);
	Delay(ADDRESSTIME);
	
	//flip clock
	cbDOut(GPIOBOARD, CLOCKPORT, clockBit);
	Delay(CLOCKTIME);
	cbDOut(GPIOBOARD, CLOCKPORT, 0);
}

void closeRelay(int boardAddress, int rowAddress, int rowIndex)
{
	int rowMask[8] = {0,0,0,0,0,0,0,0};
	rowMask[rowIndex] = 1;
	
	closeRelays(boardAddress, rowAddress, rowMask);
}

void closeRelays(int boardAddress, int rowAddress, int *rowMask)
{
	changeRelays(boardAddress, rowAddress, rowMask, CLOSECLOCK);
}

void openRelay(int boardAddress, int rowAddress, int rowIndex)
{
	int rowMask[8] = {0,0,0,0,0,0,0,0};
	rowMask[rowIndex] = 1;
	
	openRelays(boardAddress, rowAddress, rowMask);
}

void openRelays(int boardAddress, int rowAddress, int *rowMask)
{
	changeRelays(boardAddress, rowAddress, rowMask, OPENCLOCK);
}


// TODO: Update for new control scheme
void quickReset(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2) 
{
	fullReset(SwitchMatrixConfig2);
}
/*
void quickReset(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2)
{
	int zeroRowStatus[8] = {DisConnect,DisConnect,DisConnect,DisConnect,DisConnect,DisConnect,DisConnect,DisConnect};
	int connectionInRow[8] = {DisConnect,DisConnect,DisConnect,DisConnect,DisConnect,DisConnect,DisConnect,DisConnect};
	int rowNumber;
	
	for (int i = 0; i < MaxInputs; i++) {													//Loop through boards
		for (int j = 0; j < MaxRelays; j++) {												//Loop through relays
				if (SwitchMatrixConfig2->Connections[i][j]) {								//Check if relay is connected
				   rowNumber = SwitchMatrixConfig2->RelayAddress[j].relayRow; 				//Find row number of the relay
				   connectionInRow[rowNumber] = Connect;									//Note that the row has a connection
				}
		}
		
		for (int k = 0; k<8; k++) {
			if (connectionInRow[k] == Connect) {
				setRelays(SwitchMatrixConfig2->BoardAddresses[i], k, zeroRowStatus);		 //Disconnect row of relays
			}
		}
	}	
}
*/


void fullReset(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2)
{
	// Open all relays
	//printf("Resetting all relays\n");
	//int allRelaysMask[8] = {1, 1, 1, 1, 1, 1, 1, 1};
	int relayMask[4][8] = {
		{1, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 1}
		};
	
    for (int i=0; i<SwitchMatrixConfig2->numBoards; i++) {								//loop through connected boards
        for (int j=0; j<8; j++){														//loop through rows
			for (int k=0;k<4;k++) {
				openRelays(SwitchMatrixConfig2->BoardAddresses[i], j, relayMask[k]);		//Disconnect row of relays
				Delay(MULTITIME);
			}
        }
    }
	
    for (int i=0; i<MaxRelays; i++) {  						//Initialize relay status array to the unused values
		for (int j = 0;j < MaxInputs;j++) {
			SwitchMatrixConfig2->Connections[j][i] = DisConnect;
		}
    }
}


// This reset assumes nothing about the state of the flip flops
void paranoidReset(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2)
{
	// Open all relays
	//printf("Resetting all relays\n");
	//int allRelaysMask[8] = {1, 1, 1, 1, 1, 1, 1, 1};
	int relayMask[4][8] = {
		{1, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 1}
		};

	for (int i=0; i<SwitchMatrixConfig2->numBoards; i++) {								//loop through connected boards
		int boardAddress = SwitchMatrixConfig2->BoardAddresses[i];
	    for (int rowAddress=0; rowAddress<8; rowAddress++){								//loop through rows
			for (int k=0;k<4;k++) {
				int *rowMask = relayMask[k];
			
				uint8_t rowMaskNumber = convertRowMask(rowMask);
				
				printf("Resetting relays (paranoid)...\n");

				printf("Board Address: %d\n", boardAddress);
				printf("Row Address: %d\n", rowAddress);
				printf("Row Mask %d\n", rowMaskNumber);

				//Address the board and row		
				cbDOut(GPIOBOARD, BOARDPORT, boardAddress);
				cbDOut(GPIOBOARD, ROWPORT, rowAddress);


				// Set "set" flip flops low
				cbDOut(GPIOBOARD, STATUSPORT, 0);
				Delay(ADDRESSTIME);

				//flip clock
				cbDOut(GPIOBOARD, CLOCKPORT, CLOSECLOCK);
				Delay(CLOCKTIME);
				cbDOut(GPIOBOARD, CLOCKPORT, 0);
				
				// Switching Delay
				Delay(SWITCHINGTIME - CLOCKTIME);
				
				// Set "reset" flip flop high
				// Flag flip flops to change
				cbDOut(GPIOBOARD, STATUSPORT, rowMaskNumber);
				Delay(ADDRESSTIME);

				//Turn on masked flip flops
				cbDOut(GPIOBOARD, CLOCKPORT, OPENCLOCK);
				Delay(CLOCKTIME);
				cbDOut(GPIOBOARD, CLOCKPORT, 0);
				
				// Switching Delay
				Delay(SWITCHINGTIME - CLOCKTIME);
				
				// Set "reset" flip flop low
				cbDOut(GPIOBOARD, STATUSPORT, 0);
				Delay(ADDRESSTIME);

				//flip clock
				cbDOut(GPIOBOARD, CLOCKPORT, OPENCLOCK);
				Delay(CLOCKTIME);
				cbDOut(GPIOBOARD, CLOCKPORT, 0);
				
				// Delay
				Delay(MULTITIME);
			}
	    }
	}

	for (int i=0; i<MaxRelays; i++) {  						//Initialize relay status array to the unused values
		for (int j = 0;j < MaxInputs;j++) {
			SwitchMatrixConfig2->Connections[j][i] = DisConnect;
		}
	}
}



//*******************************************************************************************************************
char *fileread(char name[], char access[]) {
//*******************************************************************************************************************
    
    // Function to read a file
	#define bufferlength 256  // max buffer length
	#define MAX_FILENAME 1024  // max buffer length
	
    int ioerr_loc, i;
	int c = 0;
    char buffer[bufferlength] = "";
    char *pFileData;
    FILE *pFile;
    
    pFileData = (char*) malloc(bufferlength*sizeof(char));
    pFileData[0] = '\0';
    pFile = fopen(name, access);
    if (pFile == NULL) {
        ioerr_loc = errno;
        //printf("File error %i\n",ferror);
        perror("error");
        return NULL;
    }
    else
        ioerr_loc = 0;
    
    for (i=0;i<bufferlength-1 && (c=getc(pFile)) != EOF; i++) // load the first chuck of data, up to 256 bytes
        pFileData[i] = c;
    //if (c !=EOF)
    pFileData[i] = '\0';  //close off the first string
    //else
    //   pFileData[i] = EOF;
    
    while (c !=EOF) {
        i=0;
        for (i=0;i<bufferlength-1 && (c=getc(pFile)) != EOF ;i++) // load the next chuck of data, up to 256 bytes
            buffer[i] = c;
        //if (c !=EOF)
        buffer[i] = '\0';  //close off the string
        //else
        // buffer[i] = EOF;
        pFileData = (char *) realloc(pFileData, (strlen(pFileData)+bufferlength+1)*sizeof(char));
        strcat(pFileData, buffer);
    }
    
    fclose(pFile);
    
    return pFileData;
}


//***********************************************
int getInputNew(char FileInput[], int *pointer, char **line) {
//***********************************************
    
    // Fuction to get a line of data from a file
    
    int length,c,i,start;
    
    if (line[0] == '\0' && (*pointer) > 0)
        return EOF;
    
    
    if ((*line) != NULL) {
        free((*line));  // If line is in use, free the space
        *line = NULL;
    }
    
    start = *pointer;
    c = FileInput[*pointer];
    if (c=='\0') {
        ;
    }
    i=0;
    while (c != '\0' && c != '\n' && c!= '\r') {  //increment pointer to find end of line
        (*pointer)++;
        i++;
        c = FileInput[*pointer];
    }
    
    length = i;
    (*line) = (char *) malloc((length+1)*sizeof(char)); //allocate space for line
    for (i=0; i<length;i++) {
        (*line)[i] = FileInput[start+i];
    }
    (*line)[i] = '\0'; // append termination
    if (c != '\0')(*pointer)++; //increment file pointer to skip end of line
    while (FileInput[*pointer] == '\n' || FileInput[*pointer]== '\r') (*pointer)++; //increment again if there is extra linefeeds or CRs
    
    if (c == '\0' && i == 0)
        length = EOF;
    //else
    // length = i;
    
    return length;
}
