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

static void ke64__setZeroCheck(Addr4882_t addr, char* status);
static void ke64__setZeroCorr(Addr4882_t addr, char* status);
static void ke64__setRange(Addr4882_t addr, float range);
static void ke64__setRangeAuto(Addr4882_t addr, char* status);
static void ke64__zeroCorrect(Addr4882_t addr);
static double ke64__takeMeasurement(Addr4882_t addr);

// Filters
static void ke64__setMedianRank(Addr4882_t addr, int rank);
static void ke64__enableMedianFilter(Addr4882_t addr, char* state);
static void ke64__setDigitalFilterControl(Addr4882_t addr, char* filterControl);
static void ke64__setDigitalFilterCount(Addr4882_t addr, int count);
static void ke64__enableDigitalFilter(Addr4882_t addr, char* state);
static void ke64__enableDigitalFilterAdvanced(Addr4882_t addr, char* state);
static void ke64__setDigitalFilterAdvancedNoiseTolerence(Addr4882_t addr, int tolerence);

// The required interface functions
void ke64__setup(Addr4882_t addr);
void ke64__enable(Addr4882_t addr);
void ke64__measure(Addr4882_t addr, double* data, int* wasMeasured);
void ke64__disable(Addr4882_t addr);

#endif  /* ndef __Keithley6485_H__ */
