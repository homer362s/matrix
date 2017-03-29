void reset(Addr4882_t addr);

void setVoltage(Addr4882_t addr, double voltage);
void setCurrent(Addr4882_t addr, double current);

void sourceOn(Addr4882_t addr);
void sourceOff(Addr4882_t addr);

void disableDisplay(Addr4882_t addr);
void enableDisplay(Addr4882_t addr);


void readDisplay(Addr4882_t addr, char* msg);

void sendCommand(Addr4882_t addr, char* cmd);

void setDelay(Addr4882_t addr, float delay);

void setAutoOff(Addr4882_t addr, int toggle);

double measure(Addr4882_t addr, double* data);
