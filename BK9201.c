#include <ansi_c.h>
#include <gpib.h>
#include "gpibTools.h"
#include "MeasurementSetup.h"
#include "BK9201.h"
#include "SCPIDevice.h"

struct SourceDevice bk92__sourceDevice = {
	.name = "BK Precision 9201",
	.addr = 0,
	.setup = &bk92__setup,
	.initialize = &bk92__initialize,
	.enable = &bk92__enable,
	.disable = &bk92__disable,
	.cleanup = NULL
};

// Low level functions
void bk92__systemRemote(Addr4882_t addr, char* status)
{
	char cmd[64];
	sprintf(cmd, "SYST:%s", status);
	gpib__command(addr, cmd);
}

void bk92__setOutput(Addr4882_t addr, int status)
{
	scpi__setOutput(addr, status);
}

void bk92__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude)
{
	scpi__setSourceAmplitude(addr, func, amplitude);
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

// The required interface functions
void bk92__setup(Addr4882_t addr)
{
	bk92__systemRemote(addr, BK92__REMOTE);
	gpib__reset(addr);
	bk92__setSourceAmplitude(addr, BK92__FUNC_CURRENT, 0.01);  // Set a 10 mA current limit
}

void bk92__initialize(Addr4882_t addr, float voltage, float current)
{
	bk92__systemRemote(addr, BK92__REMOTE);
	bk92__setSourceAmplitude(addr, BK92__FUNC_VOLTAGE, voltage);
}

void bk92__enable(Addr4882_t addr)
{
	bk92__setOutput(addr, BK92__SOURCE_ON);
}

void bk92__disable(Addr4882_t addr)
{
	bk92__setOutput(addr, BK92__SOURCE_OFF);
}
