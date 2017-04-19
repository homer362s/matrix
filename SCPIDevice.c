#include <ansi_c.h>
#include <gpib.h>
#include "SCPIDevice.h"
#include "gpibTools.h"

void scpi__setOutput(Addr4882_t addr, int status)
{
	char cmd[64];
	sprintf(cmd, "OUTP %d", status);
	gpib__command(addr, cmd);
}

void scpi__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude)
{
	char cmd[64];
	sprintf(cmd, "%s %f", func, amplitude);
	gpib__command(addr, cmd);
}
