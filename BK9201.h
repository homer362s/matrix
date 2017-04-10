#ifndef __BK9201_H__
#define __BK9201_H__

#define BK92__REMOTE "REM"
#define BK92__LOCAL "LOC"

#define BK92__SOURCE_OFF 0
#define BK92__SOURCE_ON 1

#define BK92__FUNC_CURRENT "CURR"
#define BK92__FUNC_VOLTAGE "VOLT"

struct SourceDevice bk92__sourceDevice;

// Low level functions
void bk92__systemRemote(Addr4882_t addr, char* status);
void bk92__setOutput(Addr4882_t addr, int status);
void bk92__setSourceAmplitude(Addr4882_t addr, char* func, float amplitude);
float bk92__measure(Addr4882_t addr, char* func);

// Interface functions
void bk92__setup(Addr4882_t addr);
void bk92__initialize(Addr4882_t addr, float voltage, float current);
void bk92__enable(Addr4882_t addr);
void bk92__disable(Addr4882_t addr);
void bk92__cleanup(Addr4882_t addr);

#endif  /* ndef __BK9201_H__ */
