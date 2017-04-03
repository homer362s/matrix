#include <ansi_c.h>
#include <gpib.h>
#include "gpibTools.h"
#include "Keithley6485.h"

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
	ke64__setZeroCheck(addr, KE64__STATUS_OFF);
}

void ke64__setRate(Addr4882_t addr, float cycles)
{
	char cmd[64];
	sprintf(cmd, ":NPLCycles %f", cycles);
	gpib__command(addr, cmd);
}

void ke64__initialize(Addr4882_t addr)
{
	gpib__reset(addr);
	ke64__zeroCorrect(addr);
	ke64__setRate(addr, KE64__RATE_SLOW);
}

double ke64__takeMeasurement(Addr4882_t addr)
{
	gpib__command(addr, "READ?");
	
	char msg[64];
	gpib__receive(addr, msg, 64);
	
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
