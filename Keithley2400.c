#include <ansi_c.h>
#include <formatio.h>
#include <gpib.h>

#include "Keithley2400.h"


void reset(Addr4882_t addr) {
	Send(0, addr, "*RST", 4, NLend);
}

void setVoltage(Addr4882_t addr, double voltage) {
	sendCommand(addr, ":SOUR:FUNC VOLT");
	sendCommand(addr, ":SOUR:VOLT:MODE FIX");
	sendCommand(addr, ":SOUR:VOLT:RANG:AUTO 1");
	
	static char cmd[256];
	sprintf(cmd, ":SOUR:VOLT %e", voltage);
	sendCommand(addr, cmd);
}

void setCurrent(Addr4882_t addr, double current) {
	sendCommand(addr, ":SOUR:FUNC CURR");
	sendCommand(addr, ":SOUR:CURR:MODE FIX");
	sendCommand(addr, ":SOUR:CURR:RANG:AUTO 1");
	
	static char cmd[256];
	sprintf(cmd, ":SOUR:CURR %e", current);
	sendCommand(addr, cmd);
}

void sourceOn(Addr4882_t addr) {
	Send(0, addr, ":OUTP 1", 7, NLend);
}

void sourceOff(Addr4882_t addr) {
	Send(0, addr, ":OUTP 0", 7, NLend);
}

void disableDisplay(Addr4882_t addr) {
	Send(0, addr, ":DISP:ENAB 0", 12, NLend);
}

void enableDisplay(Addr4882_t addr) {
	Send(0, addr, ":DISP:ENAB 1", 12, NLend);
}

// Read the text of the display. There are better ways to get data.
void readDisplay(Addr4882_t addr, char* msg) {
	Send(0, addr, ":DISP:DATA?", 11, NLend);
	Receive(0, addr, msg, 256, STOPend);
}


void sendCommand(Addr4882_t addr, char* cmd) {
	Send(0, 24, cmd, StringLength(cmd), NLend);
}

void setDelay(Addr4882_t addr, float delay) {
	char cmd[64];
	sprintf(cmd, ":SOUR:DEL %f", delay);
	sendCommand(addr, cmd);
}

void setAutoOff(Addr4882_t addr, int toggle) {
	char cmd[64];
	sprintf(cmd, ":SOUR:CLE:AUTO %d", toggle);
	sendCommand(addr, cmd);
}

// data will be populated with the measured values:
// data = [voltage, current, resistance, time, status]
// Note. Resistance seems to give incorrect values.
// It should probably not be used
double measure(Addr4882_t addr, double* data) {
	
	Addr4882_t addrList[2];
	addrList[0] = addr;
	addrList[1] = -1;
	EnableRemote(0, addrList);
	
	sendCommand(addr, ":MEAS?");
	
	char msg[256];
	Receive(0, addr, msg, 256, STOPend);
	
	char aval[14];
	
	// Extract Values
	for (int i = 0;i < 5;i++) {
		strncpy(aval, msg+14*i, 13);
		aval[13] = 0;
		data[i] = atof(aval);
	}
	
	return 0;
}
