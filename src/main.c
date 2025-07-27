/*See LICENSE file for copyright and license details*/
/*An extremely simple system fetch utility*/
/*July 2025 - Julian Franko*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define CONVERSION_GB 1024.0

/*Function Declarations*/
char *gethostname(void);
char *getos(void);
char *getshell(void) {return getenv("SHELL");}
char *getterm(void) {return getenv("TERM");}
int getmem(void);

double kb_to_gb(double input);
char *getquotesubstr(char *string);

int main(int argc, char *argv[]) {

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-h") == 0) {
				printf("Sysfetch - A simple system info utlity. \
\n\nARGUMENTS:\n \
	-h - HELP MENU\n");
				return 0;
			}
			else {
				printf("Unknown command. Do -h for help.\n");
				return -1;
			}
		}
	}

	int i = 0;
	while (modules[i] != "END") {
		if (modules[i] == "hostname") {
			printf("HOST: %s", gethostname());
		}
		else if (modules[i] == "os") {
			printf("OS: %s\n", getos());
		}
		else if (modules[i] == "shell") {
			printf("SHELL: %s\n", getshell());
		}
		else if (modules[i] == "term") {
			printf("TERM: %s\n", getterm());
		}
		else if (modules[i] == "mem") {
			printf("MEM: "); 
			getmem();
		}
		else {
			printf("%s: UNKNOWN MODULE\n", modules[i]);
			return -1;
		}
		i++;
	}

	return 0;
}

double kb_to_gb(double input) {
	return (input / CONVERSION_GB) / CONVERSION_GB;
}

int getmem(void) {
	char line[20+1];
	double total;
	double free;
	double buffers;
	double cached;
	double used;

	FILE *file = fopen("/proc/meminfo", "r");
	if (file == NULL) {
		printf("Error opening /proc/meminfo");
		return 1;
	};
	
	/*Get Total Memory*/
	while (strcmp(line, "MemTotal:") != 0) {
		fscanf(file, "%s", line);
	}
	fscanf(file, "%lf", &total);

	/*Get Total Memory Usage*/
	while (strcmp(line, "MemFree:") != 0) {
		fscanf(file, "%s", line);
	}
	fscanf(file, "%lf", &free);

	/*Get Total Memory Usage*/
	while (strcmp(line, "Buffers:") != 0) {
		fscanf(file, "%s", line);
	}
	fscanf(file, "%lf", &buffers);

	/*Get Total Memory Usage*/
	while (strcmp(line, "Cached:") != 0) {
		fscanf(file, "%s", line);
	}
	fscanf(file, "%lf", &cached);

	used = (total - free - buffers - cached);

	used = kb_to_gb(used);
	total = kb_to_gb(total);

	printf("%.2fGiB/%.2fGiB\n", used, total);

	fclose(file);
	return 0;
}

char *gethostname(void) {
	FILE *file = fopen("/etc/hostname", "r");
	char buf[64+1];
	char *hostname;

	if (file == NULL) {
		return "Error opening hostname file.\n";
	}

	hostname = fgets(buf, 64, file);

	fclose(file);
	return hostname;
}

char *getquotesubstr(char *string) {
	int i = 0;
	int substrmv = 0;
	bool isQuotes = true;
	char* substr = (char*)malloc(32);

	while (string[i] != '\0') {
		if (string[i] == '\"') {
			switch (isQuotes) {
				case true:
					isQuotes = false;
					break;
				case false:
					isQuotes = true;
					break;
			};
		}

		if (!isQuotes && string[i] != '\"') {
			substr[substrmv] = string[i];
			substrmv++;
		}

		i++;
	}
	
	return substr;
}

char *getos(void) {
	FILE *file = fopen("/etc/lsb-release", "r");
	char buf[64+1];
	char *os;

	#ifdef __linux__
		if (file == NULL) {
			fclose(file);
			printf("Generic Linux\n");
			return "Generic Linux\n";
		}

		os = fgets(buf, 64, file);
		os = fgets(buf, 64, file);
		os = fgets(buf, 64, file);

		os = getquotesubstr(os);

		fclose(file);
		/*Not de-allocating the memory*/
		return os;
	#elif __FreeBSD__
		return "FreeBSD";
	#elif __OpenBSD__
		return "OpenBSD";
	#endif
		return "Error finding OS.";
}
