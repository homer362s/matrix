
#ifndef __autoConfigParse_H__
#define __autoConfigParse_H__

struct AutoConnection {
	int input;
	int pin;
};

struct AutoMeasurement {
	int connectionCount;
	char* label;
	struct AutoConnection* connections;
};

struct AutoConfig {
	int measurementCount;
	struct AutoMeasurement** measurements;
};

struct AutoConfig* parseLayoutFile(char* filename);
void addConnection(struct AutoMeasurement* meas, int input, int pin);
struct AutoMeasurement* newMeasurement();
void addMeasurement(struct AutoConfig* config, struct AutoMeasurement* measurement);
struct AutoConfig* newAutoConfig();
void freeConfigContents(struct AutoConfig* config);
void freeConfig(struct AutoConfig* config);


#endif  /* ndef __autoConfigParse_H__ */
