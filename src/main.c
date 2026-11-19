#include <stdio.h>
#include <pigpio.h>

#define PIN 17

int main() {
	float temperature, sum = 0;
	float minTemp = 68.0;
	float maxTemp = 76.0;
	float average;
	int count, i;
	
	FILE *fp;
	
	if (gpioInitialise() < 0) {
		printf("GPIO initilization failed.\n");
		return 1;
	}
	
	gpioSetMode(PIN, PI_OUTPUT);
	
	fp = fopen("temperature_log.txt", "w");
	
	if (fp == NULL) {
		printf("Error opening file.\n");
		gpioTerminate();
		return 1;
	}
	
	printf("Smart Classroom Comfort Monitor\n");
	printf("--------------------------------\n");
	
	printf("Enter minimum acceptable temperature: ");
	scanf("%f", &minTemp);
	
	printf("Enter maximum acceptable temperature: ");
	scanf("%f", &maxTemp);
	
	printf("How many temperature readings will be entered? ");
	scanf("%d", &count);
	
	fprintf(fp, "Temperature Log\n");
	fprintf(fp, "Acceptable Range: %.1fF - %.1fF\n\n", minTemp, maxTemp);
	
	for(i = 0; i < count; i++) {
		printf("\nEnter temperature reading #%d: ", i + 1);
		scanf("%f", &temperature);
		
		sum += temperature;
		
		if(temperature >= minTemp && temperature <= maxTemp) {
			printf("Status: NORMAL\n");
			printf("Warning LED: OFF\n");
			
			gpioWrite(PIN, 0); // LED OFF
			
			fprintf(fp, "%.1fF\tNORMAL\n", temperature);
		}
		else
		{
			printf("Status: WARNING\n");
			printf("Warning LED: ON\n");
			
			gpioWrite(PIN, 1); // LED ON
			
			fprintf(fp, "%.1fF\tWARNING\n", temperature);
		}
	}
	
	average = sum / count;
	
	printf("\nAverage Temperature: %.2fF\n", average);
	fprintf(fp, "\nAverage Temperature: %.2fF\n", average);
	
	fclose(fp);
	
	printf("\nTemperature data saved to " "temperature_log.txt\n");
	
	gpioWrite(PIN, 0); // Turn OFF LED at end
	gpioTerminate();
	
	return 0;
}
