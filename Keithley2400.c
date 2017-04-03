#include <ansi_c.h>
#include <formatio.h>
#include <gpib.h>

#include "Keithley2400.h"
#include "gpibTools.h"

// Table 3-6 (pg 3-19) in the documentation has a good example

void ke24__setSourceFunc(Addr4882_t addr, char* func) {
	char cmd[64];
	sprintf(cmd, ":SOUR:FUNC %s", func);
	gpib__command(addr, cmd);
}

void ke24__setSourceMode(Addr4882_t addr, char* func, char* mode) {
	char cmd[64];
	sprintf(cmd, ":SOUR:%s:MODE %s", func, mode);
	gpib__command(addr, cmd);
}

void ke24__setSourceRange(Addr4882_t addr, char* func, float range) {
	char cmd[64];
	sprintf(cmd, ":SOUR:%s:RANG %e", func, range);
	gpib__command(addr, cmd);
}

void ke24__setSourceRangeAuto(Addr4882_t addr, char* func, int autoRange) {
	char cmd[64];
	sprintf(cmd, ":SOUR:%s:RANG:AUTO %d", func, autoRange);
	gpib__command(addr, cmd);
}

void ke24__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude) {
	char cmd[64];
	sprintf(cmd, ":SOUR:%s %e", func, amplitude);
	gpib__command(addr, cmd);
}

void ke24__setSourceLimit(Addr4882_t addr, char* func, float level) {
	char cmd[64];
	sprintf(cmd, ":SOUR:%s:PROT %e", func, level);
	gpib__command(addr, cmd);
}

// Set the delay before a measurement can be taken in seconds. 0-999.9999
void ke24__setSourceDelay(Addr4882_t addr, float delay) {
	char cmd[64];
	sprintf(cmd, ":SOUR:DEL %f", delay);
	gpib__command(addr, cmd);
}

void ke24__setSourceDelayAuto(Addr4882_t addr, int autoDelay) {
	char cmd[64];
	sprintf(cmd, ":SOUR:DEL:AUTO %d", autoDelay);
	gpib__command(addr, cmd);
}


void ke24__setSenseFunc(Addr4882_t addr, char* func) {
	char cmd[64];
	sprintf(cmd, ":SENS:FUNC \"%s\"", func);
	gpib__command(addr, cmd);
}

void ke24__setSenseRange(Addr4882_t addr, char* func, float range) {
	char cmd[64];
	sprintf(cmd, ":%s:RANG %e", func, range);
	gpib__command(addr, cmd);
}

void ke24__setSenseRangeAuto(Addr4882_t addr, char* func, int autoRange) {
	char cmd[64];
	sprintf(cmd, ":%s:RANG:AUTO %d", func, autoRange);
	gpib__command(addr, cmd);
}

void ke24__setCompliance(Addr4882_t addr, char* func, float level) {
	char cmd[64];
	sprintf(cmd, ":%s:PROT %e", func, level);
	gpib__command(addr, cmd);
}

void ke24__setMeasurementSpeed(Addr4882_t addr, char* func, float cycles) {
	char cmd[64];
	sprintf(cmd, ":%s:NPLC %f", func, cycles);
	gpib__command(addr, cmd);
}

void ke24__setOutput(Addr4882_t addr, int state) {
	char cmd[64];
	sprintf(cmd, ":OUTP %d", state);
	gpib__command(addr, cmd);
}

void ke24__setOutputAuto(Addr4882_t addr, int autoSource) {
	char cmd[64];
	sprintf(cmd, ":SOUR:CLE:AUTO %d", autoSource);
	gpib__command(addr, cmd);
}

void ke24__initialize(Addr4882_t addr, char* sourceFunc, char* senseFunc) {
	ke24__setSourceFunc(addr, sourceFunc);
	ke24__setSenseFunc(addr, senseFunc);
	ke24__setSourceMode(addr, sourceFunc, KE24__MODE_FIXED);
	ke24__setSourceRangeAuto(addr, sourceFunc, KE24__AUTO_ON);
	ke24__setOutputAuto(addr, KE24__AUTO_ON);
	//ke24__setSourceDelayAuto(addr, KE24__AUTO_ON);
	ke24__setSourceDelay(addr, 0.1);
}

void ke24__initializeVSource(Addr4882_t addr) {
	ke24__initialize(addr, KE24__FUNC_VOLTAGE, KE24__FUNC_CURRENT);
}

void ke24__initializeISource(Addr4882_t addr) {
	ke24__initialize(addr, KE24__FUNC_CURRENT, KE24__FUNC_VOLTAGE);
}

// data will be populated with the measured values:
// data = [voltage, current, resistance, time, status]
// Note. Resistance seems to give incorrect values.
// It should probably not be used
double ke24__takeMeasurement(Addr4882_t addr)
{
	double current;
	double data[5];
	
	gpib__enableRemote(addr);
	
	gpib__command(addr, ":MEAS?");
	
	char msg[256];
	gpib__receive(addr, msg, 256);
	
	char aval[14];
	
	// Extract Values
	for (int i = 0;i < 5;i++) {
		strncpy(aval, msg+14*i, 13);
		aval[13] = 0;
		data[i] = atof(aval);
	}
	
	return data[1];
}

