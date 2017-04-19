#ifndef __MeasurementSetup_H__
#define __MeasurementSetup_H__

#include <gpib.h>

struct SourceDevice{
	char name[64];
	char idn[64];
	Addr4882_t addr;
	void (*setup)(Addr4882_t);
	void (*initialize)(Addr4882_t, float, float);
	void (*enable)(Addr4882_t);
	void (*disable)(Addr4882_t);
	void (*cleanup)(Addr4882_t);
};

struct MeasurementDevice {
	char name[64];
	char idn[64];
	Addr4882_t addr;
	void (*setup)(Addr4882_t);
	void (*initialize)(Addr4882_t);
	void (*measure)(Addr4882_t, double*, int*);
	void (*cleanup)(Addr4882_t);
};

struct MeasurementSetup {
	struct SourceDevice source;
	struct MeasurementDevice measure;
};

struct SourceDevice NullSource;
struct MeasurementDevice NullMeasure;

void setupSource(struct MeasurementSetup setup);
void setupMeasurement(struct MeasurementSetup setup) ;
void initializeSource(struct MeasurementSetup setup, float voltage, float current);
void initializeMeasurement(struct MeasurementSetup setup);
void enableSource(struct MeasurementSetup setup) ;
void takeMeasurement(struct MeasurementSetup setup, double* data, int* wasMeasured);
void disableSource(struct MeasurementSetup);
void cleanupMeasurement(struct MeasurementSetup);
void cleanupSource(struct MeasurementSetup);

#endif  /* ndef __MeasurementSetup_H__ */
