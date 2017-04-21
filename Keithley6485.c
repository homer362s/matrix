#include <ansi_c.h>
#include <gpib.h>
#include <math.h>
#include <userint.h>
#include "gpibTools.h"
#include "Keithley6485.h"
#include "MeasurementSetup.h"
#include "Keithley6485Config.h"

// Config variables
int configHandle = 0;

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
void ke64__setZeroCheck(Addr4882_t addr, char* status) 
{
	char cmd[64];
	sprintf(cmd, "SYST:ZCH %s", status);
	gpib__command(addr, cmd);
}

void ke64__setZeroCorr(Addr4882_t addr, char* status)
{
	char cmd[64];
	sprintf(cmd, "SYST:ZCOR %s", status);
	gpib__command(addr, cmd);
}

void ke64__setRange(Addr4882_t addr, float range)
{
	char cmd[64];
	sprintf(cmd, "CURR:RANG %f", range);
	gpib__command(addr, cmd);
}

void ke64__setRangeAuto(Addr4882_t addr, char* status)
{
	char cmd[64];
	sprintf(cmd, "CURR:RANG:AUTO %s", status);
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

void ke64__enableMedianFilter(Addr4882_t addr, char* state)
{
	char cmd[64];
	sprintf(cmd, "MED %s", state);
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

void ke64__enableDigitalFilter(Addr4882_t addr, char* state)
{
	char cmd[64];
	sprintf(cmd, "AVER %s", state);
	gpib__command(addr, cmd);
}

int ke64__isDigitalFilterEnabled(Addr4882_t addr)
{
	gpib__command(addr, "AVER?");
	char msg[128];
	gpib__receive(addr, msg, 127);
	return atoi(msg);
}

void ke64__enableDigitalFilterAdvanced(Addr4882_t addr, char* state)
{
	char cmd[64];
	sprintf(cmd, "AVER:ADV %s", state);
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
			break;
	}
	return 0;
}

int CVICALLBACK ke64__medianRank_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			break;
	}
	
	return 0;
}

int CVICALLBACK ke64__toggleDigitalFilter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			break;
	}
	return 0;
}

int CVICALLBACK ke64__toggleMedianFilter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			break;
	}
	return 0;
}

// The required interface functions
void ke64__setup(Addr4882_t addr)
{
	gpib__reset(addr);
	ke64__zeroCorrect(addr);
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
	
	// Show the panel
	DisplayPanel(configHandle);
}
