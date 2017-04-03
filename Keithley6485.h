#ifndef __Keithley6485_H__
#define __Keithley6485_H__

#define KE64__STATUS_OFF "OFF"
#define KE64__STATUS_ON "ON"

#define KE64__RATE_SLOW 6
#define KE64__RATE_MED 1
#define KE64__RATE_FAST 0.1

void ke64__setZeroCheck(Addr4882_t addr, char* status);
void ke64__setZeroCorr(Addr4882_t addr, char* status);
void ke64__setRange(Addr4882_t addr, float range);
void ke64__setRangeAuto(Addr4882_t addr, char* status);
void ke64__zeroCorrect(Addr4882_t addr);
void ke64__initialize(Addr4882_t addr) ;
double ke64__takeMeasurement(Addr4882_t addr);

#endif  /* ndef __Keithley6485_H__ */
