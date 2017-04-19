#include <ansi_c.h>
#include <utility.h>
#include <gpib.h>
#include <formatio.h>

void gpib__command(Addr4882_t addr, char* cmd)
{
	Send(0, addr, cmd, StringLength(cmd), NLend);
}

void gpib__enableRemote(Addr4882_t addr)
{
	Addr4882_t addrList[2];
	addrList[0] = addr;
	addrList[1] = -1;
	EnableRemote(0, addrList);
}

int gpib__receive(Addr4882_t addr, char* msg, int length)
{
	int oldValue = SetBreakOnLibraryErrors(0);
	Receive(0, addr, msg, length, STOPend);
	SetBreakOnLibraryErrors(oldValue);
	
	return iberr;
}

int gpib__identify(Addr4882_t addr, char* name)
{
	gpib__command(addr, "*IDN?");

	return gpib__receive(addr, name, 127);
}


void gpib__reset(Addr4882_t addr)
{
	Send(0, addr, "*RST", 4, NLend);
}

int gpib__scanForDevices(Addr4882_t* ListenerList)
{
	static Addr4882_t AddrList[31];

	for(short i = 0;i < 30;i++) {
		AddrList[i] = i+1;
	}
	AddrList[30] = -1;


	SendIFC(0);
	FindLstn(0, AddrList, ListenerList, 30);
	
	return(ibcnt);
}

int gpib__scanForSpecificDevice(Addr4882_t* FilteredList, char* idn)
{
	Addr4882_t ListenerList[30];
	int unfilteredCount = gpib__scanForDevices(ListenerList);
	
	int count = 0;
	for (int i = 0;i < unfilteredCount;i++) {
		char name[128];
		int nameLen = strlen(idn);
		gpib__identify(ListenerList[i], name);
		name[nameLen] = 0;
		
		if (strcmp(name, idn) == 0) {
			FilteredList[count] = ListenerList[i];
			count = count + 1; 
		}
	}
	
	FilteredList[count] = -1;
	
	return count;
}
