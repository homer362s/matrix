#include <ansi_c.h>
#include <gpib.h>
#include <math.h>
#include <userint.h>
#include "gpibTools.h"
#include "Keithley6485.h"
#include "MeasurementSetup.h"
#include "Keithley6485Config.h"

// Config variables
static int configHandle = 0;
static Addr4882_t gpibAddress = 0;

// Measurement device setup
struct MeasurementDevice ke64__measurementDevice = {
	.name = "Keithley 6485",
	.idn = "KEITHLEY INSTRUMENTS INC.,MODEL 6485",
	.addr = 0,
	.setup = &ke64__setup,
	.initialize = NULL,
	.measure = &ke64__measure,
	.cleanup = NULL,
	.config = &ke64__config
};

// Low level functions
void ke64__setZeroCheck(Addr4882_t addr, int status) 
{
	char cmd[64];
	sprintf(cmd, "SYST:ZCH %d", status);
	gpib__command(addr, cmd);
}

void ke64__setZeroCorr(Addr4882_t addr, int status)
{
	char cmd[64];
	sprintf(cmd, "SYST:ZCOR %d", status);
	gpib__command(addr, cmd);
}

void ke64__setRange(Addr4882_t addr, float range)
{
	char cmd[64];
	sprintf(cmd, "CURR:RANG %f", range);
	gpib__command(addr, cmd);
}

void ke64__setRangeAuto(Addr4882_t addr, int status)
{
	char cmd[64];
	sprintf(cmd, "CURR:RANG:AUTO %d", status);
	gpib__command(addr, cmd);
}

void ke64__zeroCorrect(Addr4882_t addr)
{
	ke64__setZeroCheck(addr, KE64__STATUS_ON);
	ke64__setRange(addr, 2e-9);
	gpib__command(addr, "INIT");
	gpib__command(addr, "SYST:ZCOR:ACQ");
	ke64__setZeroCorr(addr, KE64__STATUS_ON);
	ke64__setRangeAuto(addr, KE64__STATUS_ON);
	// Zero check should be enabled when connecting or disconnecting a supply
	//ke64__setZeroCheck(addr, KE64__STATUS_OFF);
}

void ke64__setRate(Addr4882_t addr, float cycles)
{
	char cmd[64];
	sprintf(cmd, ":NPLCycles %f", cycles);
	gpib__command(addr, cmd);
}

float ke64__getRate(Addr4882_t addr)
{
	gpib__command(addr, ":NPLCycles?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	return atof(msg);
}

// Rank is an integer from 1-5 indicating 1/2 the number of points that are filtered
void ke64__setMedianRank(Addr4882_t addr, int rank)
{
	char cmd[64];
	sprintf(cmd, "MED:RANK %d", rank);
	gpib__command(addr, cmd);
}

int ke64__getMedianRank(Addr4882_t addr)
{
	gpib__command(addr, "MED:RANK?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	return atoi(msg);
}

void ke64__enableMedianFilter(Addr4882_t addr, int state)
{
	char cmd[64];
	sprintf(cmd, "MED %d", state);
	gpib__command(addr, cmd);
}
int ke64__isMedianFilterEnabled(Addr4882_t addr)
{
	gpib__command(addr, "MED?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	return atoi(msg);
}

// Digital filters average multiple readings
void ke64__setDigitalFilterControl(Addr4882_t addr, char* filterControl)
{
	char cmd[64];
	sprintf(cmd, "AVER:TCON %s", filterControl);
	gpib__command(addr, cmd);
}

int ke64__getDigitalFilterControl(Addr4882_t addr)
{
	gpib__command(addr, "AVER:TCON?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	msg[3] = 0;
	if (strcmp(msg, KE64__FILTER_MOVING) == 0) {
		return 1;
	}
	else if(strcmp(msg, KE64__FILTER_REPEATING) == 0) {
		return 2;
	}
	else {
		return 0;
	}
}

// Count is from 1-100
void ke64__setDigitalFilterCount(Addr4882_t addr, int count)
{
	char cmd[64];
	sprintf(cmd, "AVER:COUN %d", count);
	gpib__command(addr, cmd);
}

int ke64__getDigitalFilterCount(Addr4882_t addr)
{
	gpib__command(addr, "AVER:COUN?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	return atoi(msg);
}

void ke64__enableDigitalFilter(Addr4882_t addr, int state)
{
	char cmd[64];
	sprintf(cmd, "AVER %d", state);
	gpib__command(addr, cmd);
}

int ke64__isDigitalFilterEnabled(Addr4882_t addr)
{
	gpib__command(addr, "AVER?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	return atoi(msg);
}

void ke64__enableDigitalFilterAdvanced(Addr4882_t addr, int state)
{
	char cmd[64];
	sprintf(cmd, "AVER:ADV %d", state);
	gpib__command(addr, cmd);
}   

// tolerence is an integer from 1-105 (represeting percentage)
void ke64__setDigitalFilterAdvancedNoiseTolerence(Addr4882_t addr, int tolerence)
{
	char cmd[64];
	sprintf(cmd, "AVER:ADV:NTOL %d", tolerence);
	gpib__command(addr, cmd);
}

double ke64__takeMeasurement(Addr4882_t addr)
{
	gpib__command(addr, "READ?");
	
	char msg[64];
	int err = gpib__receive(addr, msg, 64); 
	if (err != 0)
		return INFINITY;
	
	char reading[14];		// Measured current in amps. Overflow is sent as +9.9E37, NAN is sent as 9.91E37
	char units[2];			// Always amps (A)
	char timestamp[14];
	char status[14];
	
	strncpy(reading, msg, 13);			reading[13] = 0;
	strncpy(units, msg+13, 1);			units[1] = 0;
	strncpy(timestamp, msg+15, 13);		timestamp[13] = 0;
	strncpy(status, msg+29, 13);		status[13] = 0;
	
	return atof(reading);
}

// Configuration callbacks
int CVICALLBACK ke64__configPanel_CB(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch(event) {
		case EVENT_CLOSE:
			HidePanel(configHandle);
	}

	return 0;
}

int CVICALLBACK ke64__digitalCount_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			char countStr[8];
			GetCtrlVal(configHandle, CONFIG_DIGITALCOUNT, countStr);
			int chosenCount = atoi(countStr);
			if (chosenCount >= 2 && chosenCount <= 100) {
				// Set the value on the tool
				ke64__setDigitalFilterCount(gpibAddress, chosenCount);
				
				// Set the value in the display (rounding if necesary)
				sprintf(countStr, "%d", chosenCount);
				SetCtrlVal(configHandle, CONFIG_DIGITALCOUNT, countStr);
			}
			else {
				// Restore the value from the tool
				sprintf(countStr, "%d", ke64__getDigitalFilterCount(gpibAddress));
				SetCtrlVal(configHandle, CONFIG_DIGITALCOUNT, countStr);
			}
			break;
	}
	return 0;
}

int CVICALLBACK ke64__digitalFilterControl_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			int index;
			char controlType[4];
			GetCtrlIndex(configHandle, CONFIG_DIGITALFILTERRING, &index);
			GetValueFromIndex(configHandle, CONFIG_DIGITALFILTERRING, index, controlType);
			ke64__setDigitalFilterControl(gpibAddress, controlType);
			break;
	}
	return 0;
}

int CVICALLBACK ke64__medianRank_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			char rankStr[8];
			GetCtrlVal(configHandle, CONFIG_MEDIANRANK, rankStr);
			int chosenRank = atoi(rankStr);
			if (chosenRank >= 1 && chosenRank <= 5) {
				// Set the value on the tool
				ke64__setMedianRank(gpibAddress, chosenRank);
			}
			else {
				// Restore the value from the tool
				sprintf(rankStr, "%d", ke64__getMedianRank(gpibAddress));
				SetCtrlVal(configHandle, CONFIG_MEDIANRANK, rankStr);
			}
			
			break;
	}
	
	return 0;
}

int CVICALLBACK ke64__nplc_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			// If it was a button, set the string in the text box
			// Then process the text box like normal
			switch (control) {
				case CONFIG_NPLCSLOW:
					SetCtrlVal(configHandle, CONFIG_NPLC, "6");
					break;
				case CONFIG_NPLCMED:
					SetCtrlVal(configHandle, CONFIG_NPLC, "1.00");
					break;
				case CONFIG_NPLCFAST:
					SetCtrlVal(configHandle, CONFIG_NPLC, "0.01");
					break;
					
			}
			char nplcStr[16];
			GetCtrlVal(configHandle, CONFIG_NPLC, nplcStr);
			float chosenNPLC = atof(nplcStr);
			if (chosenNPLC <= 0) {
				// Restore the value from the tool and quit
				sprintf(nplcStr, "%0.2f", ke64__getRate(gpibAddress));
				SetCtrlVal(configHandle, CONFIG_NPLC, nplcStr);
				break;
			}
			// Adjust out of range values
			if (chosenNPLC < 0.01)
				chosenNPLC = 0.01;
			if (chosenNPLC > 60)
				chosenNPLC == 60;
			
			// Set the value on the tool
			ke64__setRate(gpibAddress, chosenNPLC);
			
			// Set the string
			sprintf(nplcStr, "%0.2f", chosenNPLC);
			SetCtrlVal(configHandle, CONFIG_NPLC, nplcStr);
			break;
	}
	
	return 0;
}

int CVICALLBACK ke64__toggleDigitalFilter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			int isEnabled;
			GetCtrlVal(configHandle, CONFIG_DIGITALFILTERENABLE, &isEnabled);
			ke64__enableDigitalFilter(gpibAddress, isEnabled);
			break;
	}
	return 0;
}

int CVICALLBACK ke64__toggleMedianFilter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			int isEnabled;
			GetCtrlVal(configHandle, CONFIG_MEDIANFILTERENABLE, &isEnabled);
			ke64__enableMedianFilter(gpibAddress, isEnabled);
			break;
	}
	return 0;
}

// The required interface functions
void ke64__setup(Addr4882_t addr)
{
	// Reset the tool so we know what we have
	gpib__reset(addr);
	ke64__zeroCorrect(addr);
	
	// These seem like reasonable defaults
	ke64__setRate(addr, KE64__RATE_MED);
	ke64__setMedianRank(addr, 5);
	ke64__enableMedianFilter(addr, KE64__STATUS_ON);
	ke64__setDigitalFilterCount(addr, 20);
	ke64__setDigitalFilterControl(addr, KE64__FILTER_REPEATING);
	ke64__enableDigitalFilter(addr, KE64__STATUS_ON);
}

void ke64__measure(Addr4882_t addr, double* data, int* wasMeasured)
{
	ke64__setZeroCheck(addr, KE64__STATUS_OFF);
	data[1] = ke64__takeMeasurement(addr);
	wasMeasured[0] = 0;
	wasMeasured[1] = 1;
	ke64__setZeroCheck(addr, KE64__STATUS_ON);
}

void ke64__config(Addr4882_t addr)
{
	// Store the address
	gpibAddress = addr;
	
	// Create the panel
	if (!configHandle)
		configHandle = LoadPanel(0, "Keithley6485Config.uir", CONFIG);
	
	// Populate the panel
	char tmpStr[32];
	SetCtrlVal(configHandle, CONFIG_MEDIANFILTERENABLE, ke64__isMedianFilterEnabled(addr));
	sprintf(tmpStr, "%d", ke64__getMedianRank(addr));
	SetCtrlVal(configHandle, CONFIG_MEDIANRANK, tmpStr);
	SetCtrlVal(configHandle, CONFIG_DIGITALFILTERENABLE, ke64__isDigitalFilterEnabled(addr));
	sprintf(tmpStr, "%d", ke64__getDigitalFilterCount(addr));
	SetCtrlVal(configHandle, CONFIG_DIGITALCOUNT, tmpStr);
	SetCtrlIndex(configHandle, CONFIG_DIGITALFILTERRING, ke64__getDigitalFilterControl(gpibAddress)-1);
	sprintf(tmpStr, "%0.2f", ke64__getRate(addr));
	SetCtrlVal(configHandle, CONFIG_NPLC, tmpStr);
	
	// Show the panel
	DisplayPanel(configHandle);
}
