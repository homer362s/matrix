#include <gpib.h>
#include <formatio.h>

void gpib__command(Addr4882_t addr, char* cmd) {
	Send(0, addr, cmd, StringLength(cmd), NLend);
}

void gpib__enableRemote(Addr4882_t addr) {
	Addr4882_t addrList[2];
	addrList[0] = addr;
	addrList[1] = -1;
	EnableRemote(0, addrList);
}

void gpib__receive(Addr4882_t addr, char* msg, int length) {
	Receive(0, addr, msg, length, STOPend);
}


void gpib__reset(Addr4882_t addr) {
	Send(0, addr, "*RST", 4, NLend);
}

int gpib__scanForDevices(Addr4882_t* ListenerList) {
	static Addr4882_t AddrList[31];

	for(short i = 0;i < 30;i++) {
		AddrList[i] = i+1;
	}
	AddrList[30] = -1;


	SendIFC(0);
	FindLstn(0, AddrList, ListenerList, 30);
	
	return(ibcnt);
}
