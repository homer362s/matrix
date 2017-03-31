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
#include "Keithley2400.h"
#include "gpibTools.h"


void GPIBScan();
void updateManualControls();
void updateRelays();
float getRequestedBias();
float getVBias();
float getIBias();
void takeCurrentMeasurement(Addr4882_t addr, float bias, double* data);
void takeVoltageMeasurement(Addr4882_t addr, float bias, double* data);
void changeConnection(int port, int pin);
void setStatusBar(char* status);
void setStatusDone();

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
	GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, 0, &manualTabHandle);
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
	int oldErrorSetting = SetBreakOnLibraryErrors(0);
	int devCount = gpib__scanForDevices(AttachedDevices);
	SetBreakOnLibraryErrors(oldErrorSetting);

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
				initSwitchMatrix(SwitchMatrixConfig, pathName);
	
				// Update the manual controls to reflect the available options
				updateManualControls();
				
				// Enable the currently disabled controls
				SetCtrlAttribute(panelHandle, MAINPANEL_RESETRELAYSBUTTON, ATTR_DIMMED, 0);
			}
			
			// Reset all relays
			setStatusBar("Resetting Relays");
			resetAllRelays(SwitchMatrixConfig);
			setStatusDone();
	}
	
	return 0;
}

int CVICALLBACK ManConnectionChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			int port = 0;
			switch(control) {
				case MAINPANEL_MAN_CON1_RING:
					port = 1;
					break;
				case MAINPANEL_MAN_CON2_RING:
					port = 2;
					break;
				// Note. At the time of writing this inputs 3-5 were inoperable.
				// They are included here for ease of future expansion
				case MAINPANEL_MAN_CON3_RING:
					port = 3;
					break;
				case MAINPANEL_MAN_CON4_RING:
					port = 4;
					break;
				case MAINPANEL_MAN_CON5_RING:
					port = 5;
					break;
			}
			int pin;
			GetCtrlIndex(panelHandle, control, &pin);
			
			changeConnection(port, pin);
	}
	
	return 0;
}

void updateManualControls()
{
	int count;
	
	int ctrlArrayHandle = GetCtrlArrayFromResourceID(panelHandle, MAN_CON_ARRAY);
	
	GetNumCtrlArrayItems(ctrlArrayHandle, &count);
	char tmpstr[5];
	for (int i=0; i<count; i++) {
		int MenuHandle = GetCtrlArrayItem(ctrlArrayHandle, i);
		
		// Clear the menu
		DeleteListItem(panelHandle, MenuHandle, 0, -1);
		
		// Fill in the menu
		for(int j = SwitchMatrixConfig->numProbePins+1;j > 1;j--) {
			sprintf(tmpstr, "%d", j-1); 
			InsertListItem(panelHandle, MenuHandle, 0, tmpstr, j); 
		} 
		InsertListItem(panelHandle, MenuHandle, 0, "None", 0);
		SetCtrlIndex(panelHandle, MenuHandle, 0);
	}
}

int CVICALLBACK LoadLayout_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	return 0;
}

int CVICALLBACK UpdateLabel_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	return 0;
}

int CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			ke24__setSourceDelay(24,1);
			ke24__setOutputAuto(24,KE24__AUTO_ON);
			
			float bias;
			double data[5];
			switch(control) {
				case MAINPANEL_MEASURECURRENTBUTTON:
					bias = getVBias();
					takeCurrentMeasurement(24, bias, data);
					break;
				case MAINPANEL_MEASUREVOLTAGEBUTTON:
					bias = getIBias();
					takeVoltageMeasurement(24, bias, data);
					break;
				default:
					data[0] = 0;
					data[1] = 0;
					break;
			}
			
			
			// Add row to table
			InsertTableRows(manualTabHandle, TABPANEL_1_MANUALTABLE, -1, 1, VAL_CELL_NUMERIC);
			
			int row;
			GetNumTableRows(manualTabHandle, TABPANEL_1_MANUALTABLE, &row); 
			
			SetTableCellVal(manualTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(1,row), data[0]);
			SetTableCellVal(manualTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(2,row), data[1]);
			SetTableCellVal(manualTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,row), data[0]/data[1]);
			
			// Set row name
			char label[16];
			GetCtrlVal(panelHandle, MAINPANEL_DEVIDBOX, label);
			SetTableRowAttribute(manualTabHandle, TABPANEL_1_MANUALTABLE, row, ATTR_USE_LABEL_TEXT, 1);
			SetTableRowAttribute(manualTabHandle, TABPANEL_1_MANUALTABLE, row, ATTR_LABEL_TEXT, label);
			
			// Scroll table if necessary
	}
	
	return 0;
}

void takeCurrentMeasurement(Addr4882_t addr, float vbias, double* data)
{
	ke24__initializeVSource(addr);
	ke24__setSourceAmplitude(addr, KE24__FUNC_VOLTAGE, vbias);
	ke24__takeMeasurement(addr, data);
}

void takeVoltageMeasurement(Addr4882_t addr, float ibias, double* data)
{
	ke24__initializeISource(addr);
	ke24__setSourceAmplitude(addr, KE24__FUNC_CURRENT, ibias);
	ke24__takeMeasurement(addr, data);
}


float getVBias()
{
	char strval[64];
	GetCtrlVal(panelHandle, MAINPANEL_VBIASBOX, strval);
	
	return atof(strval);
}

float getIBias()
{
	char strval[64];
	GetCtrlVal(panelHandle, MAINPANEL_IBIASBOX, strval);
	
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
			gpib__reset(24);
			ke24__initializeVSource(24);
			ke24__setSourceAmplitude(24, KE24__FUNC_VOLTAGE, 1);
			double data[5];
			ke24__takeMeasurement(24, data);
	}
	
	return 0;
}

// Chance the connection for the selected port from whatever it currently is to the specified probe pin 
void changeConnection(int port, int pin) {
	// Disconnect this port from any pins it is currently connected to
	for (int i = 0;i < MaxRelays;i++) {
		if (SwitchMatrixConfig->Connections[port-1][i]) {
			switchMatrix(port, i+1, DisConnect, SwitchMatrixConfig);
		}
	}
	
	// Connect the new pin
	if (pin)
		switchMatrix(port, pin, Connect, SwitchMatrixConfig);
}

int CVICALLBACK resetRelays_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			setStatusBar("Resetting Relays");
			resetAllRelays(SwitchMatrixConfig);
			setStatusDone();
			break;
	}
	
	return 0;
}

void setStatusBar(char* status)
{
	SetCtrlVal(panelHandle, MAINPANEL_STATUSBAR, status);
}

void setStatusDone()
{
	setStatusBar("Done");
}

int CVICALLBACK startAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			setStatusBar("Measuring");
	
			// Do measurement in here
	
			setStatusDone();
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
