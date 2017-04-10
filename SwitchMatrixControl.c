#include <cvirte.h>	
#include <userint.h>
#include <rs232.h>
#include <utility.h>
#include <formatio.h>
#include <string.h>
#include "SwitchMatrixControl.h"
#include <ansi_c.h>
#include <visa.h>

char *fileread(char name[], char access[]);
int getInputNew(char FileInput[], int *pointer, char **line);
int relayUpdate(int boardNumber, int relayNumber, int action);

int comPort = 0;				//Number of port into which the digital I/O board is plugged.
//char device[6] = "COM6\0";		//Name of device (digital I/O board) on Windows.
long baud = 9600;				//Baud rate of port where digital I/O board is plugged: 9600 bits per second.
int parity = 0;					//Parity mode for selected port: no parity.
int dataBit = 8;				//Number of data bits for port.
int stopBit = 1;				//Number of stop bits for port.
int inputQ = 16;				//Size of input queue for port.
int outputQ = 14;				//Size of output queue for port.
int RS232;						//Receives output of function that opens COM port and sets parameters.

int initSwitchMatrix (int port, struct SwitchMatrixConfig_type *SwitchMatrixConfig, char ProbeCardConfig[])
{
	// Close a possibly already open COM port
	if (comPort)
		CloseCom(comPort);
	// Open the COM port and save the port number
	comPort = port;
	RS232 = OpenComConfig(port, "", baud, parity, dataBit, stopBit, inputQ, outputQ);
	
    //Initialize switch matrix
    //FILE *file = NULL;
    char *record = NULL, access[3], *line = NULL, *InputFile=NULL, probeCardInitPins[MaxRelays][3];
    char fileName[1024];
    int i, counter, filepointer, MatrixRelays[MaxRelays],numProbecardConnections,lineLength;
	
	for (int i = 0;i < MaxRelays; i++) {
		probeCardInitPins[i][0] = 0;
	}
    
    for (counter=0; counter<MaxRelays; counter++) {  //Initialize arrays to the unused values
        SwitchMatrixConfig->ProbeCard[counter] = Unused;
        SwitchMatrixConfig->RelayStatus[counter].status = Unused;
        SwitchMatrixConfig->BoardAddresses[counter] = Unused;
		
		for (int i = 0;i < MaxInputs; i++) {
			SwitchMatrixConfig->Connections[i][counter] = (unsigned char) DisConnect;
		}
    }
    
    //Start by reading in the switch matrix configuration
    //Open File and read its contents into InputFile
    strcpy(access,"r");
    strcpy(fileName, "SwitchMatrixConfig.csv");
    InputFile = fileread(fileName, access);
    
    filepointer = 0;
    counter = 0;
    lineLength=getInputNew(InputFile, &filepointer, &line); // Read the first Excel line containing card addresses
    record = strtok(line,","); // breaks string "line" into a series of tokens using the delimiter ",".
    record = strtok(NULL,","); //Get next field to start board addresses
    // Read in the card addresses
    while(record != NULL) {
        SwitchMatrixConfig->BoardAddresses[counter] = atoi(record);
        counter++;
        record = strtok(NULL,","); //Get next field
    }
    SwitchMatrixConfig->numBoards = counter;
    
    lineLength=getInputNew(InputFile, &filepointer, &line); // Skips the next Excel line with Column Labels
    
    counter = 0;
    while((lineLength=getInputNew(InputFile, &filepointer, &line))!=0)
    {
        // Check for end of file
        if (lineLength == EOF)
            break;
        
        record = strtok(line,","); // breaks string "line" into a series of tokens using the delimiter ",".
        //Reading pin assignments
        record = strtok(NULL,","); //skip probe card pin number and get card connector assignment
        strcpy(SwitchMatrixConfig->CardConnectPins[counter],record);
        record = strtok(NULL,","); //Get relay number corresponding to card connector assignment
        MatrixRelays[counter] = atoi(record);
        counter++;
    }
    numProbecardConnections = counter; //save the number of connections to the probe card
    
    // Free data from switch matrix configuration file
    free(InputFile);
    InputFile = NULL;
    free(line);
    line = NULL;
    
    //Now read in probe card configuration
    strcpy(SwitchMatrixConfig->ProbeCardSetupFile, ProbeCardConfig);
    //Open File and read its contents into InputFile
    strcpy(access,"r");
    InputFile = fileread(ProbeCardConfig, access);
    
    filepointer = 0;
    lineLength=getInputNew(InputFile, &filepointer, &line); // Skips the first Excel line with Column Labels
    
    counter = 0;
    while((lineLength=getInputNew(InputFile, &filepointer, &line))!=0)
    {
        // Check for end of file
        if (lineLength == EOF)
            break;
        
        record = strtok(line,","); // breaks string "line" into a series of tokens using the delimiter ",".
        //Reading pin assignments
        record = strtok(NULL,","); //skip probe card pin number(Should be in order starting at 1), get probe card connector pin
        strcpy(probeCardInitPins[counter],record);
        counter++;
    }
    SwitchMatrixConfig->numProbePins = counter; //Save the number of pins used in the probe card
    
    // Free data from probe card file
    free(InputFile);
    InputFile = NULL;
    free(line);
    line = NULL;
    
    //Now match probe card pins to corresponding relay number
    for (counter=0; counter<SwitchMatrixConfig->numProbePins; counter++) { //loop over the probe pins
        for (i=0; i<numProbecardConnections; i++) { //Loop over the possible connections, looking for a match
            if (!strcmp(probeCardInitPins[counter],SwitchMatrixConfig->CardConnectPins[i])) {
                SwitchMatrixConfig->ProbeCard[counter] = MatrixRelays[i]; //save relay number corresponding to probe pin
                break;
            }
        }
    }
  
	return 0;
}

int switchMatrix(int inputNumber, int pinNumber, int action, struct SwitchMatrixConfig_type *SwitchMatrixConfig) {
	switch(action) {
		case Connect:
			printf("Connecting input %d to pin %d\n", inputNumber, pinNumber);
			break;
		case DisConnect:
			printf("Disconnecting input %d from pin %d\n", inputNumber, pinNumber);
			break;
	}
    int error;
    
    //Perform the desired action on the switch matrix
    error = relayUpdate(SwitchMatrixConfig->BoardAddresses[inputNumber-1],SwitchMatrixConfig->ProbeCard[pinNumber-1],action);
    if (!error){
        SwitchMatrixConfig->RelayStatus[pinNumber-1].boardAddress = SwitchMatrixConfig->BoardAddresses[inputNumber-1];
        SwitchMatrixConfig->RelayStatus[pinNumber-1].relayAddress = SwitchMatrixConfig->ProbeCard[pinNumber-1];
        SwitchMatrixConfig->RelayStatus[pinNumber-1].status = action; //store card adress, relay address, and relay status
		SwitchMatrixConfig->Connections[inputNumber-1][pinNumber-1] = action;
    }
    return 0;
    
}

void quickReset(struct SwitchMatrixConfig_type *SwitchMatrixConfig)
{
	for (int i = 0;i < MaxInputs;i++) {
		for (int j = 0;j < MaxRelays;j++) {
			if (SwitchMatrixConfig->Connections[i][j]) {
				switchMatrix(i+1, j+1, DisConnect, SwitchMatrixConfig);
			}
		}
	}
}

int resetUsedRelays(struct SwitchMatrixConfig_type *SwitchMatrixConfig) {
//Open all relays that have been addressed, even those that are already open (just to be sure)
    int i, temp, boardAddressLoc, relayAddressLoc, error;
    
    for (i=0; i<SwitchMatrixConfig->numProbePins; i++) {
        if (SwitchMatrixConfig->RelayStatus[i].status != Unused) { //Check if this pin has been assigned to a relay
            boardAddressLoc = SwitchMatrixConfig->RelayStatus[i].boardAddress; //Pull out the board address
            relayAddressLoc = SwitchMatrixConfig->RelayStatus[i].relayAddress; //Pullout relay address
            error = relayUpdate(boardAddressLoc,relayAddressLoc,DisConnect); //Disconnect relay
			SwitchMatrixConfig->RelayStatus[i].status = Unused; //Update status of that pin to unused
        }
    }
	
	for(int i = 0;i < MaxInputs;i++) {
		for (int j = 0;j < MaxRelays;j++) {
			SwitchMatrixConfig->Connections[i][j] = (unsigned char) DisConnect;
		}
	}
    return 0;
}

int resetAllRelays(struct SwitchMatrixConfig_type *SwitchMatrixConfig) {
// Open all relays
	printf("Resetting all relays\n");
    int i, j, error;
    
    for (i=0; i<SwitchMatrixConfig->numBoards; i++) {
        for (j=0; j<MaxRelays; j++){
            error = relayUpdate(SwitchMatrixConfig->BoardAddresses[i],j,DisConnect); //Disconnect relay
        }
        
    }
    for (i=0; i<MaxRelays; i++) {  //Initialize relay status array to the unused values
        SwitchMatrixConfig->RelayStatus[i].status = Unused;
		
		for (int j = 0;j < MaxInputs;j++) {
			SwitchMatrixConfig->Connections[j][i] = DisConnect;
		}
    }
	return 0;
}


/* 
relayUpdate(int boardNumber, int relayNumber, int action)

Flips a specific relay on a specific board to a specific state. 
boardNumber specifies which board holds the target relay; there may be up to 64 boards.
relayNumber specifies the position of the target relay on the selected board; there are at least 48 relays, and there may be up to 64.
The relay numbering on each board goes left to right, row by row, in an 8x8 grid:

	  Col 0  Col 1  Col 2  Col 3  Col 4  Col 5  Col 6  Col 7
Row	0	1      2      3      4      5      6      7      8
Row	1	9      10     11     12     13     14     15     16
Row	2	17     18     19     20     21     22     23     24
Row	3	25     26     27     28     29     30     31     32
Row	4	33     34     35     36     37     38     39     40
Row	5	41     42     43     44     45     46     47     48
Row	6	49     50     51     52     53     54     55     56
Row	7	57     58     59     60     61     62     63     64
	
isOn specifies whether the target relay is intended to be open (1) or closed (0).

relayUpdate takes the three above items of information and creates a 13-digit command code, where each digit is either a 0 or a 1, that specifies what action will be taken.

From right to left, the first six digits of the command code give the address of the selected board (0-63) using binary: 
000000 would specify board 0. 000011 would specify board 3. 

The next three digits give the row address of the target relay using binary:
000 would specify row 0. 101 would specify row 5.

The next three digits give the column address of the target relay using binary:
001 would specify column 1. 111 would specify column 7.

The last digit specifies whether the relay will be opened or closed:
1 would open the relay. 0 would close the relay.

A full command code would look like this:

		0110101000011

	0		110		101	  000011
  action   column   row	  board
	
	0   					 110		 101   		 000011
 (close) the relay in column (6) and row (5) on board (3).
 						  [relay number 47]

Each digit in the code corresponds to a port on the digital I/O board as follows: CBA9876543210. Each digit in the command code
tells whether to set the corresponding port of the digital I/O board to low voltage or high voltage.

Digital I/O Board Ports

0: ComparitorBoardAddress1
1: ComparitorBoardAddress2
2: ComparitorBoardAddress3
3: ComparitorBoardAddress4
4: ComparitorBoardAddress5
5: ComparitorBoardAddress6
6: DecoderSetAddress1
7: DecoderSetAddress2
8: DecoderSetAddress3
9: DecoderResetAddress1
A: DecoderResetAddress2
B: DecoderResetAddress3
C: DriverInput
D: MasterEnable

From right to left, the first six digits of the command code correspond to ports 543210 and specify the board number holding the target relay. Each board has its own
8-bit identity comparitor (SN74HC688DW). Two bits are unused. The other six each receive a constant high voltage or low voltage input depending on
the board number. The comparitor receives a second group of six inputs from ports 0, 1, 2, 3, 4, and 5 of the digital I/O board. Each is either high voltage or low voltage and is paired with
one of the original inputs to the comparitor. If each of the six pairs is a match, the comparitor outputs an enabling voltage to the other components on the same board (this will
only happen with the target board). Otherwise, if all six pairs do not match, the enabling voltage will not be sent and nothing else will occur on that board (this happens with all non-target boards).

The next three digits of the command code correspond to ports 876 and specify the row address of the target relay. Each board has nine 3-8 line decoders (74LVC138AD,112). 
The first decoder receives these three inputs from ports 6, 7, and 8 of the digital I/O board. Each input is either high voltage or low voltage. With three different inputs, 
there are 8 possible addresses: rows 0-7. Depending on which row is selected, this decoder will send an enabling voltage to one of the other eight decoders. 
Each of the other eight decoders is matched up with its own row of relays. 

The next three digits of the command code correspond to ports BA9 and specify the column address of the target relay. The other eight decoders all receive these three inputs from
ports 9, A, and B of the digital I/O board. Each input is either high voltage or low voltage. With three different inputs, there are 8 possible addresses: columns 0-7. 
Depending on which column is selected, the decoder that has been enabled by the first decoder will send an enabling voltage to one of the eight drivers (74LVC125AD,112) 
for one of the eight relays in its row. 

The last digit of the command code corresponds to port C and specifies the state to which the relay will go. Port C of the digital I/O board is connected to the driver input
for each driver. This input is either high voltage or low voltage. The enabled driver will allow a high voltage to open its relay or a low voltage to close its relay.

Once all of these voltages are set, port D of the digital I/O board sends a master enabling voltage to all decoders that allows the above process to occur. 
The target relay will then flip to where it is supposed to be. Then, port D turns off the enabling voltage.

*/
int relayUpdate(int boardNumber, int relayNumber, int action)
{
	char commandCode[14], relayColumnHex[4], relayRowHex[4], boardNumberHex[7];
	int i;
	
	if (action == Connect)	//Adds the digit specifying if the target relay will be connected (closed) (1) or disconnected (opened) (0) to the command code
	{
		strcpy(commandCode,"0");
	}
	else
	{
		strcpy(commandCode,"1");
	}
    
    // convert relay number to row and column using modulo arithmatic
    int relayRow;
    int relayColumn;
    relayRow = (relayNumber)/MatrixDim;
    relayColumn = (relayNumber) % MatrixDim;
	
	//Covert column number to a binary string and append to command string
	for(i=2; i>-1; i--) {
    	relayColumnHex[i] = (relayColumn & 1) == 1 ? '1' : '0';
		relayColumn >>= 1;
  	}
	relayColumnHex[3] = '\0';
	strcat(commandCode, relayColumnHex);
	
	//Covert row number to a binary string and append to command string
	for(i=2; i>-1; i--) {
    	relayRowHex[i] = (relayRow & 1) == 1 ? '1' : '0';
		relayRow >>= 1;
  	}
	relayRowHex[3] = '\0';
	strcat(commandCode, relayRowHex);
	
	//Covert board number to a binary string and append to command string
	for(i=5; i>-1; i--) {
    	boardNumberHex[i] = (boardNumber & 1) == 1 ? '1' : '0';
		boardNumber >>= 1;
  	}
	boardNumberHex[6] = '\0';
	strcat(commandCode, boardNumberHex);
	
	
	//RS232 = 
	//	OpenComConfig(comPort, device, baud, parity, dataBit, stopBit, inputQ, outputQ);	//Opens COM port and sets parameters
	SetXMode (comPort, 0);		//Disables software handshaking and XON/XOFF sensitivity
    SetCTSMode (comPort, 0);	//Disables hardware handshaking
    SetComTime (comPort, 5);	//Sets timeout limit for input and output actions
	char ioPin[14] = {"CBA9876543210"};	//Name of each port on digital I/O board
	char command[14];		//Word command to be sent to digital I/O board. Affects voltage (high or low) of one port
	int num = 0;
	int commandSize;		//Length of word command
	int bytesSent;			//Receives output of function that sends a command to digital I/O board
	for (num = 1; num < 13; num++)
	{
		int highOrLow = commandCode[num];	//Digit from the command code. 1 asks for high voltage while 0 asks for low voltage
		char targetPin = ioPin[num];		//Name of port on digital I/O board that will be set to the voltage specified by the above digit
		if (highOrLow == '1')
		{
			strcpy(command,"gpio set ");	//Set port to high voltage
			command[9] = targetPin;			//Name of port on digital I/O board
			command[10] = 0x0D;				//Append carriage return
			command[11] = '\0';				//Null terminator
			commandSize = StringLength(command);	//Length of word command to be sent to digital I/O board
			bytesSent = ComWrt(comPort, command, commandSize);	//Send command to digital I/O board to set a certain port to high voltage
		}
		else
		{
			strcpy(command,"gpio clear ");  //Set port to low voltage
			command[11] = targetPin;		//Name of port on digital I/O board
			command[12] = '\r';				//Append carriage return
			command[13] = '\0';				//Null terminator
			commandSize = StringLength(command);	//Length of word command to be sent to digital I/O board
			bytesSent = ComWrt(comPort, command, commandSize);  //Send command to digital I/O board to set a certain port to low voltage
		}
	}
	//Delay(.004);	
	Delay(0.008);
	strcpy(command,"gpio set C"); //Set driver input HIGH 
	command[10] = '\r';
	command[11] = '\0';
	commandSize = StringLength(command);
	bytesSent = ComWrt(comPort, command, commandSize);
	//Delay(0.004);
	Delay(.008);
	
	strcpy(command,"gpio set D"); //Set master enable HIGH (this will open the relay if it is closed) 
	command[10] = '\r';
	command[11] = '\0';
	commandSize = StringLength(command);
	bytesSent = ComWrt(comPort, command, commandSize);
	//Delay(.020);
	Delay(0.040);
	
	if (action == Connect) {
		strcpy(command,"gpio clear C");  //Set driver input LOW.  (This closes the relay)
		command[12] = '\r';
		command[13] = '\0';
		commandSize = StringLength(command);
		bytesSent = ComWrt(comPort, command, commandSize);
		//Delay(.030);
		//Delay(0.060);
		Delay(0.2);
		
		strcpy(command,"gpio clear D");  //Set master enable LOW
		command[12] = '\r';
		command[13] = '\0';
		commandSize = StringLength(command);
		bytesSent = ComWrt(comPort, command, commandSize);
	}
	else  {
		strcpy(command,"gpio clear D");  //Set master enable LOW (This leaves the relay open)
		command[12] = '\r';
		command[13] = '\0';
		commandSize = StringLength(command);
		bytesSent = ComWrt(comPort, command, commandSize);
		//Delay(.004);
		Delay(0.008);
		
		strcpy(command,"gpio clear C");  //Set driver input LOW. (This has no effect)  
		command[12] = '\r';
		command[13] = '\0';
		commandSize = StringLength(command);
		bytesSent = ComWrt(comPort, command, commandSize);
	}
	return 0;
}





//*******************************************************************************************************************
char *fileread(char name[], char access[]) {
//*******************************************************************************************************************
    
    // Function to read a file
#define bufferlength 256  // max buffer length
#define MAX_FILENAME 1024  // max buffer length
    int ioerr_loc, i, c;
    char buffer[bufferlength];
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

/***********************************************/
int getInputNew(char FileInput[], int *pointer, char **line) {
/***********************************************/
    
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
    /*else
     length = i;*/
    
    return length;
}
