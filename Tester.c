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

#include <rs232.h>
#include <formatio.h>
#include <stdio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include <gpib.h>
#include "Tester.h"
#include "toolbox.h"
#include "SwitchMatrixControl.h"
#include "Keithley2400.h"
#include "Keithley6485.h"
#include "BK9201.h"
#include "gpibTools.h"
#include "autoConfigParse.h"
#include "MeasurementSetup.h"

#define MEASURE_CURRENT 0
#define MEASURE_VOLTAGE 1

#define NODEVICE 0
#define KEITHLEY2400 1
#define KEITHLEY6485 2
#define BK9201 3


// Function prototypes
void GPIBScan();
void COMScan();
void updateManualControls();
void updateRelays();
float getRequestedBias();
float getVBias();
void takeCurrentMeasurement(struct MeasurementSetup setup, float voltage, double* data, int* wasMeasured);
void changeConnection(int port, int pin);
void setStatusBar(char* status);
void setStatusDone();
int getSelectedRow();
float getSourceCoeff();
float getMeasCoeff();
void setProbeCardDisplay(int dimmed, char* label);
int getTabCount();
void newFrame();
void incrementLabelName(char* label);
void updateHighlights();
float getHighlightMin();
float getHighlightMax();


// Global variables
static int panelHandle = 0;
int currentTabHandle;
Addr4882_t sourceAddress = 24;
Addr4882_t measAddress = 14;
int matrixAddress = 0;
int sourceDevice = BK9201;
int measDevice = KEITHLEY6485;

struct SwitchMatrixConfig_type *SwitchMatrixConfig;
struct AutoConfig* layoutConfig = NULL;
struct MeasurementSetup measurementSetup;

// Functions
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Tester.uir", MAINPANEL));
	GPIBScan();
	COMScan();
	SwitchMatrixConfig = (struct SwitchMatrixConfig_type *) malloc(sizeof (struct SwitchMatrixConfig_type));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, 0, &currentTabHandle);
	
	// Set the default measurement devices and initialize
	int sourceIndex = 0;
	int measIndex = 0;
	GetIndexFromValue(panelHandle, MAINPANEL_SOURCEDEVICERING, &sourceIndex, sourceDevice);
	GetIndexFromValue(panelHandle, MAINPANEL_MEASDEVICERING, &measIndex, measDevice);
	SetCtrlIndex(panelHandle, MAINPANEL_SOURCEDEVICERING, sourceIndex);
	SetCtrlIndex(panelHandle, MAINPANEL_MEASDEVICERING, measIndex);
	
	switch (sourceDevice) {
		case KEITHLEY2400:
			measurementSetup.source = ke24__sourceDevice;
			break;
		case BK9201:
			measurementSetup.source = bk92__sourceDevice;
			break;
	}
	
	switch (measDevice) {
		case KEITHLEY2400:
			measurementSetup.measure = ke24__measurementDevice;
			break;
		case KEITHLEY6485:
			measurementSetup.measure = ke64__measurementDevice;
			break;
	}
	
	errChk (RunUserInterface ());


Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_CLOSE:
			QuitUserInterface(0);
	}

	return 0;
}

int CVICALLBACK DevScan_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{	
	switch(event)											  
	{
		case EVENT_COMMIT:
			GPIBScan();
			COMScan();
	}
	return 0;
}

void GPIBScan()
{
	// Delete all existing items in the dropdown menu
	DeleteListItem(panelHandle, MAINPANEL_SOURCEADDRESSRING, 0, -1);
	DeleteListItem(panelHandle, MAINPANEL_MEASADDRESSRING, 0, -1);

	// Replace those items with the newly discovered ones
	Addr4882_t AttachedDevices[30];
	int oldErrorSetting = SetBreakOnLibraryErrors(0);
	int devCount = gpib__scanForDevices(AttachedDevices);
	SetBreakOnLibraryErrors(oldErrorSetting);

	InsertListItem(panelHandle, MAINPANEL_SOURCEADDRESSRING, 0, "", 0);
	InsertListItem(panelHandle, MAINPANEL_MEASADDRESSRING, 0, "", 0);
	char tmpstr[3];
	for(int i = 0; i < devCount; i++) {
		sprintf(tmpstr, "%d", AttachedDevices[i]);
		InsertListItem(panelHandle, MAINPANEL_SOURCEADDRESSRING, i+1, tmpstr, AttachedDevices[i]);
		InsertListItem(panelHandle, MAINPANEL_MEASADDRESSRING, i+1, tmpstr, AttachedDevices[i]);
	}
	sourceAddress = 0;
	measAddress = 0;

}

void COMScan()
{
	// Delete all existing items in the dropdown menu
	DeleteListItem(panelHandle, MAINPANEL_MATRIXADDRRING, 0, -1);
	
	// Discover some addresses
	int maxAddr = 20;
	int addrCount = 0;
	int addresses[maxAddr];
	int oldErrorSetting = SetBreakOnLibraryErrors(0);
	for (int i = 1;i <= maxAddr;i++) {
		if (OpenCom(i, "") >= 0){
			addresses[addrCount] = i;
			addrCount++;
			CloseCom(i);
		}
	}
	SetBreakOnLibraryErrors(oldErrorSetting);
	
	// Replace those items with the newly discovered ones
	InsertListItem(panelHandle, MAINPANEL_MATRIXADDRRING, 0, "", 0);
	char tmpstr[6];
	for (int i = 0; i < addrCount; i++) {
		sprintf(tmpstr, "COM%d", addresses[i]);
		InsertListItem(panelHandle, MAINPANEL_MATRIXADDRRING, i+1, tmpstr, addresses[i]);
	}
	
	matrixAddress = 0;
}

int CVICALLBACK deviceChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int newDevice;
	switch(event) {
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, control, &newDevice);
			int addressControl = 0;
			switch(control) {
				case MAINPANEL_SOURCEDEVICERING:
					switch (newDevice) {
						case KEITHLEY2400:
							measurementSetup.source = ke24__sourceDevice;
							break;
						case BK9201:
							measurementSetup.source = bk92__sourceDevice;
							break;
					}
					addressControl = MAINPANEL_SOURCEADDRESSRING;
					break;
				case MAINPANEL_MEASDEVICERING:
					switch (newDevice) {
						case KEITHLEY2400:
							measurementSetup.measure = ke24__measurementDevice;
							break;
						case KEITHLEY6485:
							measurementSetup.measure = ke64__measurementDevice;
							break;
					}
					addressControl = MAINPANEL_MEASADDRESSRING;
					break;
			}
			
			SetCtrlIndex(panelHandle, addressControl, 0);
	}
	return 0;
}

int CVICALLBACK addressChanged_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int value;
	switch(event) {
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, control, &value);
			switch(control) {
				case MAINPANEL_SOURCEADDRESSRING:
					measurementSetup.source.addr = (Addr4882_t) value;
					setupSource(measurementSetup);
					break;
				case MAINPANEL_MEASADDRESSRING:
					measurementSetup.measure.addr = (Addr4882_t) value;
					setupMeasurement(measurementSetup);
					break;
				case MAINPANEL_MATRIXADDRRING:
					matrixAddress = value;
					setProbeCardDisplay(!value, "");
			}
			break;
	}
	return 0;
}

int CVICALLBACK toggleColors_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int isChecked;
	
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, MAINPANEL_COLORSCHECKBOX, &isChecked);
			SetCtrlAttribute(panelHandle, MAINPANEL_MINBOX, ATTR_DIMMED, !isChecked);
			SetCtrlAttribute(panelHandle, MAINPANEL_MAXBOX, ATTR_DIMMED, !isChecked);
			updateHighlights();
			break;
	}
	return 0;
}

void setProbeCardDisplay(int dimmed, char* label)
{
	SetCtrlAttribute(panelHandle, MAINPANEL_LOADPROBECARDBUTTON, ATTR_DIMMED, dimmed);
	
	SetCtrlVal(panelHandle, MAINPANEL_PROBECARDNAME, label);
	
	if (!strcmp(label, "")) {
		SetCtrlAttribute(panelHandle, MAINPANEL_RESETRELAYSBUTTON, ATTR_DIMMED, 1);
	} else {
		SetCtrlAttribute(panelHandle, MAINPANEL_RESETRELAYSBUTTON, ATTR_DIMMED, 0);
	}
}

int CVICALLBACK LoadProbeCard_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			char pathName[512];
			int success = FileSelectPopupEx("", "*.csv", "*.csv;*.*", "Select A Probe Card", VAL_LOAD_BUTTON, 0, 0, pathName);
			if (success) {
				initSwitchMatrix(6, SwitchMatrixConfig, pathName);
	
				// Update the manual controls to reflect the available options
				updateManualControls();
				
				// Get file name (without path)
				char* fileName = pathName;
				char* tmp = pathName;
				while(tmp) {
					fileName = tmp + 1;
					tmp = strchr(fileName, '\\');
				}
				setProbeCardDisplay(0, fileName);
			}
			
			// Reset all relays
			setStatusBar("Resetting Relays");
			resetAllRelays(SwitchMatrixConfig);
			int count;
			int ctrlArrayHandle = GetCtrlArrayFromResourceID(panelHandle, MAN_CON_ARRAY);

			GetNumCtrlArrayItems(ctrlArrayHandle, &count);
			for (int i=0; i<count; i++) {
				int MenuHandle = GetCtrlArrayItem(ctrlArrayHandle, i);
				SetCtrlIndex(panelHandle, MenuHandle, 0);
			}
			setStatusDone();
	}
	
	return 0;
}

int CVICALLBACK LoadLayout_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			char pathName[512];
			int success = FileSelectPopupEx("", "*.csv", "*.csv;*.*", "Select A Layout Config", VAL_LOAD_BUTTON, 0, 0, pathName);
			if (success) {
				// If we already have a layout read in, free it before overwriting the pointer
				if (layoutConfig)
					freeConfig(layoutConfig);
				
				// Read the file
				layoutConfig = parseLayoutFile(pathName);
				
				
				// Update the UI
				SetCtrlAttribute(panelHandle, MAINPANEL_STARTMEASBUTTON, ATTR_DIMMED, 0);
				SetCtrlAttribute(panelHandle, MAINPANEL_STARTREMEASBUTTON, ATTR_DIMMED, 0);
				SetCtrlAttribute(panelHandle, MAINPANEL_AUTOMEASDEVLIST, ATTR_DIMMED, 0);
				SetCtrlAttribute(panelHandle, MAINPANEL_SINGLEAUTOMEASBUTTON, ATTR_DIMMED, 0);
				
				// Get file name (without path)
				char* fileName = pathName;
				char* tmp = pathName;
				while(tmp) {
					fileName = tmp + 1;
					tmp = strchr(fileName, '\\');
				}
				SetCtrlVal(panelHandle, MAINPANEL_LAYOUTNAME, fileName);
				
				// Update the device list
				DeleteListItem(panelHandle, MAINPANEL_AUTOMEASDEVLIST, 0, -1);
				for (int i = 0;i < layoutConfig->measurementCount;i++) {
					InsertListItem(panelHandle, MAINPANEL_AUTOMEASDEVLIST, i, layoutConfig->measurements[i]->label, i);
				}
			}
			break;
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

int CVICALLBACK renameRow_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			int row = getSelectedRow();
			char label[16];
			GetCtrlVal(panelHandle, MAINPANEL_DEVIDBOX, label);
			SetTableRowAttribute(currentTabHandle, TABPANEL_1_MANUALTABLE, row, ATTR_LABEL_TEXT, label);
			break;
	}
	return 0;
}

// Run a single current measurement
// Takes in the measurement setup and returns the data in a 3 element double array:
// data = [Voltage [V], Current [A], Resistance [Ohm]]
// measured is a 2 element boolean array indicating if the specific quantity was measured or not
// measured = [WasVoltageMeasured, WasCurrentMeasured]
// Since at least one of those two must be measured, resistance is implied to have been measured
void takeCurrentMeasurement(struct MeasurementSetup setup, float voltage, double* data, int* wasMeasured)
{
	// Initialize the source
	initializeSource(measurementSetup, voltage, 0);
	
	// Initialize the measurement
	initializeMeasurement(measurementSetup);
	
	// Enable the source
	enableSource(measurementSetup);
	
	// Delay for the source to settle
	Delay(0.1);
	
	// Take the measurement
	takeMeasurement(measurementSetup, data, wasMeasured);
	
	// Disable the source
	disableSource(measurementSetup);
	
	// Measurement cleanup
	cleanupMeasurement(measurementSetup);
	
	// Source cleanup
	cleanupSource(measurementSetup);
}

void handleSingleMeasurement(int measurementType, int newRow, char* label)
{
	int row = -1;
	if (!newRow) {
		row = getSelectedRow();
	}
	
	double current;
	double voltage;
	double resistance;
	
	double data[3];
	int wasMeasured[2];
	
	// Take the measurement
	switch(measurementType) {
		case MEASURE_CURRENT:
			voltage = getVBias();
			takeCurrentMeasurement(measurementSetup, voltage/getSourceCoeff(), data, wasMeasured);
			current = data[1];
			break;
		default:
			voltage = 1;
			current = 1;
			break;
	}
	
	resistance = voltage/current;

	// Create a new tab if one is needed
	if (getTabCount() <= 0) {
		newFrame();
	}
	
	// Create a new row if one is needed
	if (row < 0)
	{
		InsertTableRows(currentTabHandle, TABPANEL_1_MANUALTABLE, -1, 1, VAL_CELL_NUMERIC);
		GetNumTableRows(currentTabHandle, TABPANEL_1_MANUALTABLE, &row);
		SetActiveTableCell(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(1,row));
	}
	
	// Set row name
	SetTableRowAttribute(currentTabHandle, TABPANEL_1_MANUALTABLE, row, ATTR_USE_LABEL_TEXT, 1);
	SetTableRowAttribute(currentTabHandle, TABPANEL_1_MANUALTABLE, row, ATTR_LABEL_TEXT, label);

	// Fill in the values
	SetTableCellVal(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(1,row), voltage);
	SetTableCellVal(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(2,row), current);
	SetTableCellVal(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,row), resistance);
	
	// Highlight the cell if its within the "good" range
	int isChecked;
	GetCtrlVal(panelHandle, MAINPANEL_COLORSCHECKBOX, &isChecked);
	if (isChecked && resistance > getHighlightMin() && resistance < getHighlightMax()) {
		SetTableCellAttribute(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,row), ATTR_TEXT_BGCOLOR, MakeColor(200,255,200));
	}
	else {
		SetTableCellAttribute(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,row), ATTR_TEXT_BGCOLOR, MakeColor(255,255,255)); 
	}
	
}

int CVICALLBACK ManualMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			char label[16];
			GetCtrlVal(panelHandle, MAINPANEL_DEVIDBOX, label);
			
			switch(control) {
				case MAINPANEL_REMEASURECURRENTBUTTO:
					handleSingleMeasurement(MEASURE_CURRENT, 0, label);
					break;
				case MAINPANEL_MEASURECURRENTBUTTON:
					handleSingleMeasurement(MEASURE_CURRENT, 1, label);
					break;
				case MAINPANEL_REMEASUREVOLTAGEBUTTO:
					handleSingleMeasurement(MEASURE_VOLTAGE, 0, label);
					break;
				case MAINPANEL_MEASUREVOLTAGEBUTTON:
					handleSingleMeasurement(MEASURE_VOLTAGE,1, label);
					break;
			}
			
			// Increment the device ID if requested
			int checked;
			GetCtrlVal(panelHandle, MAINPANEL_AUTODEVCHECK, &checked);
			if (checked) {
				incrementLabelName(label);
				SetCtrlVal(panelHandle, MAINPANEL_DEVIDBOX, label);
			}
	}
	
	return 0;
}

float getVBias()
{
	char strval[64];
	GetCtrlVal(panelHandle, MAINPANEL_VBIASBOX, strval);
	
	return atof(strval);
}

float getSourceCoeff()
{
	char strval[64];
	GetCtrlVal(panelHandle, MAINPANEL_INPUTCOEFF, strval);
	
	return atof(strval);
}

float getMeasCoeff()
{
	char strval[64];
	GetCtrlVal(panelHandle, MAINPANEL_MEASCOEFF, strval);
	
	return atof(strval);
}

/*
float getIBias()
{
	char strval[64];
	GetCtrlVal(panelHandle, MAINPANEL_IBIASBOX, strval);
	
	return atof(strval);
}
*/

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
			
			int count;
			int ctrlArrayHandle = GetCtrlArrayFromResourceID(panelHandle, MAN_CON_ARRAY);

			GetNumCtrlArrayItems(ctrlArrayHandle, &count);
			for (int i=0; i<count; i++) {
				int MenuHandle = GetCtrlArrayItem(ctrlArrayHandle, i);
				SetCtrlIndex(panelHandle, MenuHandle, 0);
			}
			
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

// Frame manipulation callbacks and helper functions
int getCurrentTab()
{
	int currentTabIndex;
	GetActiveTabPage(panelHandle, MAINPANEL_TABS, &currentTabIndex);
	
	return currentTabIndex;
}

int getTabCount()
{
	int tabCount;
	GetNumTabPages(panelHandle, MAINPANEL_TABS, &tabCount);
	
	return tabCount;
}

void getNewFrameID(char* newID)
{
	GetCtrlVal(panelHandle, MAINPANEL_FRAMEIDBOX, newID);
}

int getSelectedRow()
{
	Point cell;
	GetActiveTableCell(currentTabHandle, TABPANEL_1_MANUALTABLE, &cell);
	return cell.y;
}

void newFrame()
{
	// Make the tab
	char newLabel[15];
	getNewFrameID(newLabel);
	int newTabIndex = InsertTabPage(panelHandle, MAINPANEL_TABS, getTabCount(), newLabel);
	SetActiveTabPage(panelHandle, MAINPANEL_TABS, newTabIndex);

	// Update the current tab handle
	GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, getCurrentTab(), &currentTabHandle);

	// Create the table in the new tab
	int tableCtrlID = NewCtrl(currentTabHandle, CTRL_TABLE, 0, 0, 0);
	SetCtrlAttribute(currentTabHandle, tableCtrlID, ATTR_WIDTH, 240);
	SetCtrlAttribute(currentTabHandle, tableCtrlID, ATTR_HEIGHT, 350);
	SetCtrlAttribute(currentTabHandle, tableCtrlID, ATTR_SCROLL_BARS, 2);
	SetCtrlAttribute(currentTabHandle, tableCtrlID, ATTR_ENABLE_COLUMN_SIZING, 0);
	SetCtrlAttribute(currentTabHandle, tableCtrlID, ATTR_ENABLE_ROW_SIZING, 0);

	InsertTableColumns(currentTabHandle, tableCtrlID, 1, 3, VAL_CELL_NUMERIC);

	for (int i = 1;i <= 3;i++) {
		SetTableColumnAttribute(currentTabHandle, tableCtrlID, i, ATTR_FORMAT, VAL_SCIENTIFIC_FORMAT);
		SetTableColumnAttribute(currentTabHandle, tableCtrlID, i, ATTR_USE_LABEL_TEXT, 1);
		SetTableColumnAttribute(currentTabHandle, tableCtrlID, i, ATTR_COLUMN_WIDTH, 60);
	}

	SetTableColumnAttribute(currentTabHandle, tableCtrlID, 1, ATTR_LABEL_TEXT, "V [V]");
	SetTableColumnAttribute(currentTabHandle, tableCtrlID, 2, ATTR_LABEL_TEXT, "I [A]");
	SetTableColumnAttribute(currentTabHandle, tableCtrlID, 3, ATTR_LABEL_TEXT, "R [Ohm]");
	
	// Update the next frame label if it is set to auto
	int checked;
	GetCtrlVal(panelHandle, MAINPANEL_AUTOFRAMECHECK, &checked);
	if (checked) {
		char label[16];
		GetCtrlVal(panelHandle, MAINPANEL_FRAMEIDBOX, label);
		incrementLabelName(label);
		SetCtrlVal(panelHandle, MAINPANEL_FRAMEIDBOX, label);
	}
}

void incrementLabelName(char* label)
{
	int len;
	int finalNum;
	
	// Get final number
	len = strlen(label);
	for (int i = len-1;i >= 0;i--) {
		if (len == 0) { // There is currently no label -> Make the next label "1"
			sprintf(label, "1");
			break;
		}
		else if (i == 0 && !(label[i] < 48 || label[i] > 57)) { // The whole label is a number -> Increment it
			finalNum = atoi(label);
			sprintf(label, "%d", finalNum + 1);
			break;
		}
		else if (i == len-1 && (label[i] < 48 || label[i] > 57)) { // No trailing number -> Append a 1
			sprintf(label, "%s%d", label, 1);
			break;
		}
		else if (label[i] < 48 || label[i] > 57) { // We found the trailing number
			char* finalNumStr;
			
			i = i + 1;
			finalNumStr = label + i;
			
			int oldNum = atoi(finalNumStr);
			sprintf(finalNumStr, "%d", oldNum + 1);
			break;
		}
	}
}

int CVICALLBACK newFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			newFrame();
			break;
	}
	return 0;
}

int CVICALLBACK deleteFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			int currentTab = getCurrentTab();
			DeleteTabPage(panelHandle, MAINPANEL_TABS, currentTab, 1);
			if (getTabCount() > 0)
			{
				SetActiveTabPage(panelHandle, MAINPANEL_TABS, currentTab == 0 ? 0 : currentTab - 1);
				GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, getCurrentTab(), &currentTabHandle);
			} else {
				currentTabHandle = -1;
			}
			break;
	}
	return 0;
}

int CVICALLBACK saveFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	char pathName[512];
	switch (event) {
		case EVENT_COMMIT:
			{
				char defaultFilename[32];
				GetTabPageAttribute(panelHandle, MAINPANEL_TABS, getCurrentTab(), ATTR_LABEL_TEXT, defaultFilename);
				sprintf(defaultFilename, "%s.dat", defaultFilename);
				
				if (FileSelectPopupEx ("%userprofile%", defaultFilename, "datafile", "Create a save file", VAL_SAVE_BUTTON, 0, 0, pathName) != VAL_NO_FILE_SELECTED)
	            {
	            	/* Open the file and write out the data */
					FILE *frameData;
	           		frameData = fopen (pathName,"w");
				
					// Get the number of rows in the table
					int numTableRows;
					GetNumTableRows (currentTabHandle, TABPANEL_1_MANUALTABLE ,&numTableRows);
				
					// Write file header
					fprintf(frameData,"Device, V [V], I [A], R[Ohm]\n");

					char devID[32];
					
					// Write data points
					double currentTableValue;
					for(int y=1;y<=numTableRows;y++)
					{
						GetTableRowAttribute(currentTabHandle, TABPANEL_1_MANUALTABLE, y, ATTR_LABEL_TEXT, devID);
						fprintf(frameData, "%s, ", devID);
						for(int x=1;x<=3;x++)
						{
							GetTableCellVal(currentTabHandle, TABPANEL_1_MANUALTABLE, MakePoint(x,y), &currentTableValue);
							fprintf(frameData,"%e, ",currentTableValue);
						}
 
						fseek(frameData, -2, SEEK_CUR);
						fprintf(frameData,"\n");
					}

					fclose (frameData);
				}
			}
			break;
	}
	return 0;
}

int CVICALLBACK saveAllFrames_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	double currentTableValue;
	char pathName[512];
	
	FILE *frameData;
	
	switch (event) {
		case EVENT_COMMIT:
			{
				if (FileSelectPopupEx ("%userprofile%", "*.dat", "datafile", "Create a save file", VAL_SAVE_BUTTON, 0, 0, pathName) != VAL_NO_FILE_SELECTED)
	            {
	            	/* Open the file and write out the data */
	            	frameData = fopen (pathName,"w");
					int numTabs = getTabCount();
					int tabHandle;
					
					fprintf(frameData,"Frame, Device, V [V], I [A], R[Ohm]\n");

					for(int currentTab=0;currentTab<numTabs;currentTab++)
					{
						int numTableRows;
						GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, currentTab, &tabHandle);
						GetNumTableRows(tabHandle, TABPANEL_1_MANUALTABLE, &numTableRows);
						
						char frameID[32];
						char devID[32];
						GetTabPageAttribute(panelHandle, MAINPANEL_TABS, currentTab, ATTR_LABEL_TEXT, frameID);

						for(int y=1;y<=numTableRows;y++)
						{
							GetTableRowAttribute(tabHandle, TABPANEL_1_MANUALTABLE, y, ATTR_LABEL_TEXT, devID);
							fprintf(frameData, "%s, %s, ", frameID, devID);
							for(int x=1;x<=3;x++)
							{
								GetTableCellVal(tabHandle, TABPANEL_1_MANUALTABLE, MakePoint(x,y), &currentTableValue);
								fprintf(frameData,"%e, ", currentTableValue);
							}

							fseek(frameData, -2, SEEK_CUR);
							fprintf(frameData,"\n");
						}
					}
					fclose (frameData);
	            }
			}
			break;
	}
	return 0;
}

int CVICALLBACK renameFrame_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			char newLabel[15];
			getNewFrameID(newLabel);
			SetTabPageAttribute(panelHandle, MAINPANEL_TABS, getCurrentTab(), ATTR_LABEL_TEXT, newLabel);
			break;
	}
	return 0;
}

int CVICALLBACK tabs_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			int oldErrorSetting = SetBreakOnLibraryErrors(0);
			int status = GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, eventData2, &currentTabHandle);
			SetBreakOnLibraryErrors(oldErrorSetting);
			
			if (status < 0)
				currentTabHandle = -1;
			break;
			
	}
	return 0;
}

int CVICALLBACK startAutoReMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			setStatusBar("Measuring");
	
			int row = getSelectedRow();
			if (row <= 0) {
				setStatusDone();
				break;
			}
			
			// Disconnect any connected relays
			quickReset(SwitchMatrixConfig);
			
			struct AutoMeasurement* meas = layoutConfig->measurements[row-1];
			
			// Connect the required relays
			for (int i = 0;i < meas->connectionCount;i++) {
				switchMatrix(meas->connections[i].input, meas->connections[i].pin, Connect, SwitchMatrixConfig);
			}
			
			// Take measurement
			handleSingleMeasurement(MEASURE_CURRENT, 0, meas->label);
			
			// Disconnect the relays
			for (int i = 0;i < meas->connectionCount;i++) {
				switchMatrix(meas->connections[i].input, meas->connections[i].pin, DisConnect, SwitchMatrixConfig);
			}
			
			setStatusDone();
			break;
	}
	
	return 0;
}

int CVICALLBACK singleAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			setStatusBar("Measuring");
			
			// Disconnect any connected relays
			quickReset(SwitchMatrixConfig);
			
			// Get the measurement index
			int index;
			GetCtrlIndex(panelHandle, MAINPANEL_AUTOMEASDEVLIST, &index);
			
			struct AutoMeasurement* meas = layoutConfig->measurements[index];
			
			// Connect the required relays
			for (int i = 0;i < meas->connectionCount;i++) {
				switchMatrix(meas->connections[i].input, meas->connections[i].pin, Connect, SwitchMatrixConfig);
			}
			
			// Take measurement
			handleSingleMeasurement(MEASURE_CURRENT, 1, meas->label);
			
			// Disconnect the relays
			for (int i = 0;i < meas->connectionCount;i++) {
				switchMatrix(meas->connections[i].input, meas->connections[i].pin, DisConnect, SwitchMatrixConfig);
			}
			
			setStatusDone();
			break;
	}
	return 0;
}

void updateHighlights() {
	float min = getHighlightMin();
	float max = getHighlightMax();
	
	int tabHandle;
	int rowCount;
	double resistance;
	for(int i = 0;i < getTabCount();i++) {
		GetPanelHandleFromTabPage(panelHandle, MAINPANEL_TABS, i, &tabHandle);
		GetNumTableRows(tabHandle, TABPANEL_1_MANUALTABLE, &rowCount);
		for(int j = 0;j < rowCount;j++) {
			GetTableCellVal(tabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,j+1), &resistance);
			int isChecked;
			GetCtrlVal(panelHandle, MAINPANEL_COLORSCHECKBOX, &isChecked);
			if (isChecked && resistance > min && resistance < max) {
				SetTableCellAttribute(tabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,j+1), ATTR_TEXT_BGCOLOR, MakeColor(200,255,200));
			} else {
				SetTableCellAttribute(tabHandle, TABPANEL_1_MANUALTABLE, MakePoint(3,j+1), ATTR_TEXT_BGCOLOR, MakeColor(255,255,255));
			}
		}
	}
}

float getHighlightMin() {
	char value[16];
	GetCtrlVal(panelHandle, MAINPANEL_MINBOX, value);
	return atof(value);
}

float getHighlightMax() {
	char value[16];
	GetCtrlVal(panelHandle, MAINPANEL_MAXBOX, value);
	return atof(value);
}

int CVICALLBACK updateHighlights_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			updateHighlights();
			break;
	}
	return 0;
}

int CVICALLBACK startAutoMeasure_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_COMMIT:
			// Create a new frame for this measurement
			newFrame();
			ProcessDrawEvents();
			
			// Reset the relays
			setStatusBar("Resetting all relays");
			resetAllRelays(SwitchMatrixConfig);
			int count;
			int ctrlArrayHandle = GetCtrlArrayFromResourceID(panelHandle, MAN_CON_ARRAY);

			GetNumCtrlArrayItems(ctrlArrayHandle, &count);
			for (int i=0; i<count; i++) {
				int MenuHandle = GetCtrlArrayItem(ctrlArrayHandle, i);
				SetCtrlIndex(panelHandle, MenuHandle, 0);
			}
			
			// Start the measurement
			setStatusBar("Measuring");
	
			// Loop over each measurement
			for (int i = 0;i < layoutConfig->measurementCount;i++) {
				// Make the needed connections
				for (int j = 0;j < layoutConfig->measurements[i]->connectionCount;j++) {
					struct AutoConnection conn = layoutConfig->measurements[i]->connections[j];
					switchMatrix(conn.input, conn.pin, Connect, SwitchMatrixConfig);
				}
				
				// Delay to make sure all the relays finish connecting
				Delay(0.2);
				
				// Do the measurement
				//printf("Taking measurement\n");
				handleSingleMeasurement(MEASURE_CURRENT, 1, layoutConfig->measurements[i]->label);
				ProcessDrawEvents();
				
				
				// Clear the connections
				for (int j = 0;j < layoutConfig->measurements[i]->connectionCount;j++) {
					struct AutoConnection conn = layoutConfig->measurements[i]->connections[j];
					switchMatrix(conn.input, conn.pin, DisConnect, SwitchMatrixConfig);
				}
			}
	
			setStatusDone();
	}
	return 0;
}
