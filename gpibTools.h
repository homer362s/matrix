
#ifndef __gpibTools_H__
#define __gpibTools_H__

void gpib__command(Addr4882_t addr, char* cmd);
void gpib__enableRemote(Addr4882_t addr);
int gpib__receive(Addr4882_t addr, char* msg, int length);
int gpib__identify(Addr4882_t addr, char* name);
void gpib__reset(Addr4882_t addr);
int gpib__scanForDevices(Addr4882_t* ListenerList);
int gpib__scanForSpecificDevice(Addr4882_t* FilteredList, char* idn);


#endif  /* ndef __gpibTools_H__ */
