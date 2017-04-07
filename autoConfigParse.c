
#include <ansi_c.h>
#include "autoConfigParse.h"

// Functions to parse the layout config file
struct AutoConfig* parseLayoutFile(char* filename)
{
	// Open the file
	FILE *fp = fopen(filename, "r");
	
	// Find the end of the file so we know how big it is
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Allocate a character array to hold the whole file
	char* str = malloc(fsize + 1);
	fread(str, fsize, 1, fp);
	fclose(fp);

	// Null terminate the character array
	str[fsize] = 0;
	
	// Split up the lines
	int maxLines = 25;
	char** lines = malloc(maxLines*sizeof(char*));
	int lineCount = 1;
	
	lines[0] = str;
	for (int i = 0;i < fsize;i++) {
		if (str[i] == '\n') {
			str[i] = 0;
			lineCount++;
			if (lineCount > maxLines) {
				maxLines = maxLines*2;
				lines = realloc(lines, maxLines*sizeof(char*));
			}
			lines[lineCount - 1] = &str[i+1];
			if (strlen(lines[lineCount-1]) == 0) {
				lineCount = lineCount - 1;
				
			}
		}
	}
	
	// Make an empty config
	struct AutoConfig* config = newAutoConfig();
	
	// Loop over the lines
	for (int i = 0;i < lineCount;i++) {
		
		char* token = strtok(lines[i], ",");
		
		// Create a new measurement
		struct AutoMeasurement* meas = newMeasurement(token);
		
		// Loop over the connections
		int input, pin;
		token = strtok(NULL, ",");
		while(token != NULL) {
			sscanf(token, "%d:%d", &input, &pin);
			addConnection(meas, input, pin);
			token = strtok(NULL, ",");
		}
		
		
		// Add the new measurement to the config
		addMeasurement(config, meas);
	}
	
	
	
	
	// Don't forget to free anything we don't need
	free(str);
	free(lines);
	
	return config;
}

// Functions to manipulate the config struct
void addConnection(struct AutoMeasurement* meas, int input, int pin)
{
	int oldCount = meas->connectionCount;
	if (oldCount == 0) {
		meas->connectionCount = 1;
		meas->connections = (struct AutoConnection*) malloc(sizeof(struct AutoConnection));
		meas->connections[0].input = input;
		meas->connections[0].pin = pin;
	}
	else {
		int count = oldCount + 1;
		meas->connectionCount = count;
		meas->connections = (struct AutoConnection*) realloc(meas->connections, count*sizeof(struct AutoConnection));
		meas->connections[count-1].input = input;
		meas->connections[count-1].pin = pin;
	}
}

struct AutoMeasurement* newMeasurement(char* label) {
	struct AutoMeasurement* meas = (struct AutoMeasurement*) malloc(sizeof(struct AutoMeasurement));
	int len = strlen(label) + 1;
	meas->label = malloc(len*sizeof(char));
	strcpy(meas->label, label);
	meas->connectionCount = 0;
	meas->connections = NULL;
	
	return meas;
}

void addMeasurement(struct AutoConfig* config, struct AutoMeasurement* measurement)
{
	int oldCount = config->measurementCount;
	if (oldCount == 0) {
		config->measurementCount = 1;
		config->measurements = (struct AutoMeasurement**) malloc(sizeof(struct AutoMeasurement*));
		config->measurements[0] = measurement;
	}
	else {
		int count = oldCount + 1;
		config->measurementCount = count;
		config->measurements = (struct AutoMeasurement**) realloc(config->measurements, count * sizeof(struct AutoMeasurement*));
		config->measurements[count-1] = measurement;
	}
}

struct AutoConfig* newAutoConfig()
{
	struct AutoConfig* config = (struct AutoConfig*) malloc(sizeof(struct AutoConfig));
	config->measurementCount = 0;
	config->measurements = NULL;
	
	return config;
}

void freeConfigContents(struct AutoConfig* config)
{
	for (int i = 0;i < config->measurementCount;i++) {
		free(config->measurements[i]->connections);
		config->measurements[i]->connections = NULL;
		
		free(config->measurements[i]->label);
		config->measurements[i]->label = NULL;
		
		free(config->measurements[i]);
		config->measurements[i] = NULL;
	}
}

void freeConfig(struct AutoConfig* config)
{
	freeConfigContents(config);
	
	free(config);
	config = NULL;
}
