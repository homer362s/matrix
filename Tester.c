//==============================================================================
//
// Title:		Tester
// Purpose:		A short description of the application.
//
// Created on:	12/30/2016 at 10:02:24 AM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <gpib.h>
#include "Tester.h"
#include "toolbox.h"
#include "SwitchMatrixControl.h"
#include "Interface.h"
#include "Keithley2400.h"


void GPIBScan();
void updateManualControls();
void updateRelays();
float getRequestedBias();
float getVBias();
float getIBias();
void takeCurrentMeasurement(Addr4882_t addr, float bias, double* data);
void takeVoltageMeasurement(Addr4882_t addr, float bias, double* data);

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;
int manualTabHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
struct SwitchMatrixConfig_type *SwitchMatrixConfig;

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Tester.uir", MAINPANEL));
	GPIBScan();
	SwitchMatrixConfig = (struct SwitchMatrixConfig_type *) malloc(sizeof (struct SwitchMatrixConfig_type));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, 1, &manualTabHandle);
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_CLOSE:
			QuitUserInterface(0);
	}

	return 0;
}

int CVICALLBACK GPIBScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{	
	switch(event)											  
	{
		case EVENT_COMMIT:
			GPIBScan();
	}
	return 0;
}

void GPIBScan()
{
	// Delete all existing items in the dropdown menu
	DeleteListItem(panelHandle, MAINPANEL_GPIBADDRESSRING, 0, -1);

	// Replace those items with the newly discovered ones
	Addr4882_t AttachedDevices[30];
	int devCount = ScanForGPIBDevices(AttachedDevices);

	char tmpstr[3];
	for(int i = 0; i < devCount; i++) {
		sprintf(tmpstr, "%d", AttachedDevices[i]);
		InsertListItem(panelHandle, MAINPANEL_GPIBADDRESSRING, 0, tmpstr, AttachedDevices[i]);
	}

}

int CVICALLBACK LoadProbeCard_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			char pathName[512];
			int success = FileSelectPopupEx("", "*.csv", "*.csv;*.*", "Select A Probe Card", VAL_LOAD_BUTTON, 0, 0, pathName);
			if (success) {
				int error = initSwitchMatrix(SwitchMatrixConfig, pathName);
	
				// Update the manual controls to reflect the available options
				updateManualControls();
			}
	}
	
	return 0;
}

int CVICALLBACK ManConnectionChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	return 0;
}

void updateManualControls()
{
	int count;
	
	int ctrlArrayHandle = GetCtrlArrayFromResourceID(manualTabHandle, MAN_CON_ARRAY);
	
	GetNumCtrlArrayItems(ctrlArrayHandle, &count);
	char tmpstr[5];
	for (int i=0; i<count; i++) {
		int MenuHandle = GetCtrlArrayItem(ctrlArrayHandle, i);
		
		// Clear the menu
		DeleteListItem(manualTabHandle, MenuHandle, 0, -1);
		
		// Fill in the menu
		for(int j = SwitchMatrixConfig->numProbePins;j > 0;j--) {
			sprintf(tmpstr, "%d", j); 
			InsertListItem(manualTabHandle, MenuHandle, 0, tmpstr, j); 
		}
		SetCtrlIndex(manualTabHandle, MenuHandle, i);
	}
	
	updateRelays();
}

void updateRelays()
{
}

int CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			setDelay(24, 1);
			setAutoOff(24,1);
			float bias;
			double data[5];
			switch(control) {
				case TABPANEL_2_MEASURECURRENTBUTTON:
					bias = getVBias();
					takeCurrentMeasurement(24, bias, data);
					break;
				case TABPANEL_2_MEASUREVOLTAGEBUTTON:
					bias = getIBias();
					takeVoltageMeasurement(24, bias, data);
					break;
			}
			
			
			// Add row to table
			int error = InsertTableRows(manualTabHandle, TABPANEL_2_MANUALTABLE, -1, 1, VAL_CELL_NUMERIC);
			
			int row;
			GetNumTableRows(manualTabHandle, TABPANEL_2_MANUALTABLE, &row); 
			
			SetTableCellVal(manualTabHandle, TABPANEL_2_MANUALTABLE, MakePoint(1,row), data[0]);
			SetTableCellVal(manualTabHandle, TABPANEL_2_MANUALTABLE, MakePoint(2,row), data[1]);
	}
	
	return 0;
}

void takeCurrentMeasurement(Addr4882_t addr, float vbias, double* data)
{
	setVoltage(addr, vbias);
	sendCommand(addr, ":CONF:CURR");
	measure(addr, data);
}

void takeVoltageMeasurement(Addr4882_t addr, float ibias, double* data)
{
	setCurrent(addr, ibias);
	sendCommand(addr, ":CONF:VOLT");
	measure(addr, data);
}


float getVBias()
{
	char strval[64];
	GetCtrlVal(manualTabHandle, TABPANEL_2_VBIASBOX, strval);
	
	return atof(strval);
}

float getIBias()
{
	char strval[64];
	GetCtrlVal(manualTabHandle, TABPANEL_2_IBIASBOX, strval);
	
	return atof(strval);
}




int CVICALLBACK SendGPIB (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//int Dev = ibdev(0, 24, 0,
            //    T10s, 1, 0);
			//
			//ibclr(Dev);
			
			//static char cmd[] = ":SOUR:VOLT 0";
			//Send(0, 24, cmd, StringLength(cmd), NLend);
			reset(24);
			setVoltage(24, 1);
			sourceOn(24);
			double data[5];
			measure(24, data);
			sourceOff(24);
			
			int tmp;
	}
	
	return 0;
}

int CVICALLBACK Run (int panel, int control, int event,
					 void *callbackData, int eventData1, int eventData2)
{
	int error;
	switch (event)
	{
		case EVENT_COMMIT:
			//Very important: Run initialization first, passing in the name of the probe card config file
			error = initSwitchMatrix (SwitchMatrixConfig, "48 Pin Probe Card.csv");
			
			//Just to be crazy safe, let's reset all relays
			//error = resetAllRelays(SwitchMatrixConfig);
			
			//Now we are ready to do the measurement.  Let's do an example of measuring betweens pins 1&2
			//and then pins 3&4,  Connect input 1 to pin 1, and input2 to pin 2
			//Syntax is:switchMatrix(switchMatrix(inputNumber, pinNumber, action, SwitchMatrixConfig)
			error = switchMatrix(1, 1, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 2, Connect, SwitchMatrixConfig);
			//Now do the measurement and record data
			
			//Next, disconnect the inputs
			error = switchMatrix(1, 1, DisConnect, SwitchMatrixConfig);
			error = switchMatrix(2, 2, DisConnect, SwitchMatrixConfig);
			
			//Now connect the inputs to pins 3&4
			error = switchMatrix(1, 3, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 4, Connect, SwitchMatrixConfig);
			//Now do the measurement and record data
			
			//Next, disconnect the inputs
			error = switchMatrix(1, 3, DisConnect, SwitchMatrixConfig);
			error = switchMatrix(2, 4, DisConnect, SwitchMatrixConfig);
			
			
			//This section will step through the entire 48 pins
			error = switchMatrix(2, 1, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 1, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 2, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 2, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 3, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 3, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 4, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 4, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 5, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 5, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 6, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 6, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 7, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 7, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 8, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 8, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 9, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 9, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 10, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 10, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 11, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 11, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 12, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 12, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 13, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 13, DisConnect, SwitchMatrixConfig);
									
			error = switchMatrix(2, 14, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 14, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 15, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 15, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 16, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 16, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 17, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 17, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 18, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 18, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 19, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 19, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 20, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 20, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 21, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 21, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 22, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 22, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 23, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 23, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 24, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 24, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 25, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 25, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 26, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 26, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 27, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 27, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 28, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 28, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 29, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 29, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 30, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 30, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 31, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 31, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 32, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 32, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 33, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 33, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 34, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 34, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 35, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 35, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 36, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 36, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 37, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 37, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 38, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 38, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 39, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 39, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 40, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 40, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 41, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 41, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 42, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 42, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 43, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 43, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 44, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 44, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 45, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 45, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 46, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 46, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 47, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 47, DisConnect, SwitchMatrixConfig);
			
			error = switchMatrix(2, 48, Connect, SwitchMatrixConfig);
			error = switchMatrix(2, 48, DisConnect, SwitchMatrixConfig);

			break;
	}
	return 0;
}
