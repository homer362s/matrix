#include <gpib.h>


int ScanForGPIBDevices(Addr4882_t* ListenerList) {
	static Addr4882_t AddrList[31];

	for(short i = 0;i < 30;i++) {
		AddrList[i] = i+1;
	}
	AddrList[30] = -1;


	SendIFC(0);
	FindLstn(0, AddrList, ListenerList, 30);
	
	return(ibcnt);
}

