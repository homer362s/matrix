#include "MeasurementSetup.h"

struct SourceDevice NullSource = {
	.name = ""
};

struct MeasurementDevice NullMeas = {
	.name = ""
};

// Setup of the source device happens when the source address is chosen
void setupSource(struct MeasurementSetup setup)
{
	void (*setupFunc)(Addr4882_t) = setup.source.setup;
	if (setupFunc)
		(*setupFunc)(setup.source.addr);
}

// Setup of the measurement device happens when the measurement address is chosen
void setupMeasurement(struct MeasurementSetup setup)
{
	void (*setupFunc)(Addr4882_t) = setup.measure.setup;
	if (setupFunc)
		(*setupFunc)(setup.measure.addr);
}

// Initialization of the source device happens shortly before a measurement is taken, every time
void initializeSource(struct MeasurementSetup setup, float voltage, float current)
{
	void (*initializeFunc)(Addr4882_t,float,float) = setup.source.initialize;
	if (initializeFunc)
		(*initializeFunc)(setup.source.addr, voltage, current);
}

// Initialization of a measurement device happens shortly before a measurement is taken, every time
void initializeMeasurement(struct MeasurementSetup setup)
{
	void (*initializeFunc)(Addr4882_t) = setup.measure.initialize;
	if (initializeFunc)
		(*initializeFunc)(setup.measure.addr);
}

// Enable the source immediately before a measurement
void enableSource(struct MeasurementSetup setup)
{
	void (*enableFunc)(Addr4882_t) = setup.source.enable;
	if (enableFunc)
		(*enableFunc)(setup.source.addr);
}

// Take a measurement
void takeMeasurement(struct MeasurementSetup setup, double* data, int* wasMeasured)
{
	void (*measureFunc)(Addr4882_t, double*, int*) = setup.measure.measure;
	if (measureFunc) {
		(*measureFunc)(setup.measure.addr, data, wasMeasured);
	}
	else {
		wasMeasured[0] = 0;
		wasMeasured[1] = 0;
	}
}

// Disable the source immediately after a measurement
void disableSource(struct MeasurementSetup setup)
{
	void (*disableFunc)(Addr4882_t) = setup.source.disable;
	if (disableFunc)
		(*disableFunc)(setup.source.addr);
}

// Cleanup the measurement device shortly after measurement, every time
void cleanupMeasurement(struct MeasurementSetup setup)
{
	void (*cleanupFunc)(Addr4882_t) = setup.measure.cleanup;
	if (cleanupFunc)
		(*cleanupFunc)(setup.measure.addr);
}

// Cleanup the source device shortly after a measurement, every time
void cleanupSource(struct MeasurementSetup setup)
{
	void (*cleanupFunc)(Addr4882_t) = setup.source.cleanup;
	if (cleanupFunc)
		(*cleanupFunc)(setup.source.addr);
}

