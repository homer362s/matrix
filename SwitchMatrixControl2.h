#ifndef __SwitchMatrixControl2_H__
#define __SwitchMatrixControl2_H__



#define Connect 1 			//Connect (close a relay)
#define DisConnect 0 		//Disconnect (open a relay)
#define Unused 0xFFFF 		//Code for an unused pin
#define MaxRelays 48 		//Maximum number of relays
#define MaxInputs 5 		//Maximum number of switch matrix inputs
#define MatrixDim 8 		//Switch matrix dimension
						
struct RelayStatus_type{
	int boardAddress;
	int relayAddress;
	int status;
};

struct RelayAddress_type{
	int relayRow;
	int rowIndex;
};

struct SwitchMatrixConfig2_type{
 	struct RelayAddress_type RelayAddress[MaxRelays];		//Contents of struct are the relay addresses according to row number and index within row
															//Binary numbering for pin (starts at 0) and for relay number (starts at 0)
	unsigned char Connections[MaxInputs][MaxRelays];
    int BoardAddresses[MaxRelays]; 							//Array containing the addresses of installed cards
	char CardConnectPins[MaxRelays][3]; 					//Card edge connector names
    int numProbePins; 										//Number of pins in the probe card
    int numBoards;  										//Number of relay boards installed
    char ProbeCardSetupFile[1024]; 							//Filename for probe card definition
    char ErrorMessage[256];						
};


#endif  /* ndef __SwitchMatrixControl2_H__ */


//Function prototypes
int initSwitchMatrix (struct SwitchMatrixConfig2_type *SwitchMatrixConfig2, char ProbeCardConfig[]);
void switchMatrix(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2, int inputNumber, int pinNumber, int action);
void fullReset(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2);
void quickReset(struct SwitchMatrixConfig2_type *SwitchMatrixConfig2);
void setRelaysBBM(int boardNumber, int rowAddress, int *rowStatus);
void setRelaysMBB(int boardNumber, int rowAddress, int *rowStatus);
void closeRelay(int boardAddress, int rowAddress, int rowIndex);
void closeRelays(int boardAddress, int rowAddress, int *rowMask);
void openRelay(int boardAddress, int rowAddress, int rowIndex);
void openRelays(int boardAddress, int rowAddress, int *rowMask);
