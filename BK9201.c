#include <ansi_c.h>
#include <gpib.h>
#include "gpibTools.h"

void bk92__systemRemote(Addr4882_t addr, char* status)
{
	char cmd[64];
	sprintf(cmd, "SYST:%s", status);
	gpib__command(addr, cmd);
}

void bk92__setOutput(Addr4882_t addr, int status)
{
	char cmd[64];
	sprintf(cmd, "OUTP %d", status);
	gpib__command(addr, cmd);
}

void bk92__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude)
{
	char cmd[64];
	sprintf(cmd, "%s %f", func, amplitude);
	gpib__command(addr, cmd);
}

float bk92__measure(Addr4882_t addr, char* func)
{
	char cmd[64];
	sprintf(cmd, "MEAS:%s?", func);
	gpib__command(addr, cmd);
	
	char msg[256];
	gpib__receive(addr, msg, 256);
	
	return atof(msg);
}
