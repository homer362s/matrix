#ifndef __SCPIDevice_H__
#define __SCPIDevice_H__

#define SCPI__SOURCE_OFF 0
#define SCPI__SOURCE_ON 1

#define SCPI__FUNC_CURRENT "CURR"
#define SCPI__FUNC_VOLTAGE "VOLT"

void scpi__setOutput(Addr4882_t addr, int status);
void scpi__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude);

#endif  /* ndef __SCPIDevice_H__ */
