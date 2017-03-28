//
//  SwitchMatrixControl.h
//
//  Created by Greg Snider on 12/28/16.
//

#define Connect 1 //Connect (close a relay)
#define DisConnect 0 //Disconnect (open a relay
#define Unused 0xFFFF //Code for an unused pin
#define MaxRelays 48 //Maximum number of relays
#define MatrixDim 8 //Switch matrix dimension

struct RelayStatus_type{
	int boardAddress;
	int relayAddress;
	int status;
};

struct SwitchMatrixConfig_type{
    int ProbeCard[MaxRelays];	//Contents of array are the relay ID corresponding to card pin number
                        //Binary numbering for pin (starts at 0) and for relay number (starts at 0)
    struct RelayStatus_type RelayStatus[MaxRelays]; //Array containing the relay info (board, ID and state) connected to each pin (starts at 0)
                        //0xFFFF if unused (unknown)
    int BoardAddresses[MaxRelays]; //Array containing the addresses of installed cards
	char CardConnectPins[MaxRelays][3]; //Card edge connector names
    int numProbePins; //Number of pins in the probe card
    int numBoards;  //Number of relay boards installed
    char ProbeCardSetupFile[1024]; //Filename for probe card definition
    char ErrorMessage[256];
};

//Function prototypes
int initSwitchMatrix (struct SwitchMatrixConfig_type *SwitchMatrixConfig, char ProbeCardConfig[]);
int switchMatrix(int inputNumber, int pinNumber, int action, struct SwitchMatrixConfig_type *SwitchMatrixConfig);
int resetUsedRelays(struct SwitchMatrixConfig_type *SwitchMatrixConfig);
int resetAllRelays(struct SwitchMatrixConfig_type *SwitchMatrixConfig);
