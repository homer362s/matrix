#include "MeasurementSetup.h"

// Setup of the source device happens when the source address is chosen
void setupSource(struct MeasurementSetup setup)
{
	void (*setupFunc)(Addr4882_t) = setup.source.setup;
	(*setupFunc)(setup.source.addr);
}

// Setup of the measurement device happens when the measurement address is chosen
void setupMeasurement(struct MeasurementSetup setup)
{
	void (*setupFunc)(Addr4882_t) = setup.measure.setup;
	(*setupFunc)(setup.measure.addr);
}

// Initialization of the source device happens shortly before a measurement is taken, every time
void initializeSource(struct MeasurementSetup setup, float voltage, float current)
{
	void (*initializeFunc)(Addr4882_t,float,float) = setup.source.initialize;
	(*initializeFunc)(setup.source.addr, voltage, current);
}

// Initialization of a measurement device happens shortly before a measurement is taken, every time
void initializeMeasurement(struct MeasurementSetup setup)
{
	void (*initializeFunc)(Addr4882_t) = setup.measure.initialize;
	(*initializeFunc)(setup.measure.addr);
}

// Enable the source immediately before a measurement
void enableSource(struct MeasurementSetup setup)
{
	void (*enableFunc)(Addr4882_t) = setup.source.enable;
	(*enableFunc)(setup.source.addr);
}

// Take a measurement
void takeMeasurement(struct MeasurementSetup setup, double* data, int* wasMeasured)
{
	void (*measureFunc)(Addr4882_t, double*, int*) = setup.measure.measure;
	(*measureFunc)(setup.measure.addr, data, wasMeasured);
}

// Disable the source immediately after a measurement
void disableSource(struct MeasurementSetup setup)
{
	void (*disableFunc)(Addr4882_t) = setup.source.disable;
	(*disableFunc)(setup.source.addr);
}

// Cleanup the measurement device shortly after measurement, every time
void cleanupMeasurement(struct MeasurementSetup setup)
{
	void (*cleanupFunc)(Addr4882_t) = setup.measure.cleanup;
	(*cleanupFunc)(setup.measure.addr);
}

// Cleanup the source device shortly after a measurement, every time
void cleanupSource(struct MeasurementSetup setup)
{
	void (*cleanupFunc)(Addr4882_t) = setup.source.cleanup;
	(*cleanupFunc)(setup.source.addr);
}

