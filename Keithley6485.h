#ifndef __Keithley6485_H__
#define __Keithley6485_H__

#define KE64__STATUS_OFF "OFF"
#define KE64__STATUS_ON "ON"

#define KE64__RATE_SLOW 6
#define KE64__RATE_MED 1
#define KE64__RATE_FAST 0.1

#define KE64__FILTER_REPEATING "REP"
#define KE64__FILTER_MOVING "MOV"

struct MeasurementDevice ke64__measurementDevice;

void ke64__setZeroCheck(Addr4882_t addr, char* status);
void ke64__setZeroCorr(Addr4882_t addr, char* status);
void ke64__setRange(Addr4882_t addr, float range);
void ke64__setRangeAuto(Addr4882_t addr, char* status);
void ke64__zeroCorrect(Addr4882_t addr);
double ke64__takeMeasurement(Addr4882_t addr);

// Filters
void ke64__setMedianRank(Addr4882_t addr, int rank);
void ke64__enableMedianFilter(Addr4882_t addr, char* state);
void ke64__setDigitalFilterControl(Addr4882_t addr, char* filterControl);
void ke64__setDigitalFilterCount(Addr4882_t addr, int count);
void ke64__enableDigitalFilter(Addr4882_t addr, char* state);
void ke64__enableDigitalFilterAdvanced(Addr4882_t addr, char* state);
void ke64__setDigitalFilterAdvancedNoiseTolerence(Addr4882_t addr, int tolerence);

// The required interface functions
void ke64__setup(Addr4882_t addr);
void ke64__initialize(Addr4882_t addr);
void ke64__measure(Addr4882_t addr, double* data, int* wasMeasured);
void ke64__cleanup(Addr4882_t addr);

#endif  /* ndef __Keithley6485_H__ */
