#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	FILE *time_output = fopen("./time_output.txt", "r");
	
	system("rm time_output.txt");
	for (int run = 0; run < 100; run++) {
		system("./test >> time_output.txt");
	}
	
	char str[BUFSIZ];
	double sum = 0;
	double curr = 0;
	double min = 1;
	double max = 0;
	while (fgets(str, BUFSIZ, time_output)) {
		curr = atof(str);
		sum += curr;
		if (curr < min) min = curr;
		else if (curr > max) max = curr;
	}
	printf("Avg: %f\n", sum/100.0);
	printf("Max: %f\n", max);
	printf("Min: %f\n", min);
}
