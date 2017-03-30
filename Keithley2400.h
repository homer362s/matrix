#ifndef __Keithley2400_H__
#define __Keithley2400_H__

#define KE24__FUNC_CURRENT "CURR"
#define KE24__FUNC_VOLTAGE "VOLT"
#define KE24__FUNC_RESISTANCE "RES"

#define KE24__MODE_FIXED "FIX"
#define KE24__MODE_LIST "LIST"
#define KE24__MODE_SWEEP "SWE"

#define KE24__AUTO_OFF 0
#define KE24__AUTO_ON 1

#define KE24__SOURCE_OFF 0
#define KE24__SOURCE_ON 1


void ke24__setSourceFunc(Addr4882_t addr, char* func);
void ke24__setSourceMode(Addr4882_t addr, char* func, char* mode);
void ke24__setSourceRange(Addr4882_t addr, char* func, float range);
void ke24__setSourceRangeAuto(Addr4882_t addr, char* func, int autoRange);
void ke24__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude);
void ke24__setSourceLimit(Addr4882_t addr, char* func, float level);
void ke24__setSourceDelay(Addr4882_t addr, float delay);
void ke24__setSourceDelayAuto(Addr4882_t addr, int autoDelay);

void ke24__setSenseFunc(Addr4882_t addr, char* func);
void ke24__setSenseRange(Addr4882_t addr, char* func, float range);
void ke24__setSenseRangeAuto(Addr4882_t addr, char* func, int autoRange);
void ke24__setCompliance(Addr4882_t addr, char* func, float level);
void ke24__setMeasurementSpeed(Addr4882_t addr, char* func, float cycles);

void ke24__setOutput(Addr4882_t addr, int state);
void ke24__setOutputAuto(Addr4882_t addr, int autoSource);

void ke24__initialize(Addr4882_t addr, char* sourceFunc, char* senseFunc);
void ke24__initializeVSource(Addr4882_t addr);
void ke24__initializeISource(Addr4882_t addr);

void ke24__takeMeasurement(Addr4882_t addr, double* data);


#endif  /* ndef __Keithley2400_H__ */
