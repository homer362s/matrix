
#ifndef __gpibTools_H__
#define __gpibTools_H__

void gpib__command(Addr4882_t addr, char* cmd);
void gpib__enableRemote(Addr4882_t addr);
void gpib__receive(Addr4882_t addr, char* msg, int length);
void gpib__reset(Addr4882_t addr);
int gpib__scanForDevices(Addr4882_t* ListenerList);


#endif  /* ndef __gpibTools_H__ */
