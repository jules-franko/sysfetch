/*See LICENSE file for copyright and license details*/
/*An extremely simple system fetch utility*/
/*July 2025 - Julian Franko*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define CONVERSION_GB 1024.0

#define COLOR_BOLD  "\033[1m"
#define COLOR_OFF   "\033[m"

#ifndef __bool_true_false_are_defined
    #ifdef _Bool
        #define bool                        _Bool
    #else
        #define bool                        int
    #endif
    #define true                            1
    #define false                           0
    #define __bool_true_false_are_defined   1
#endif

/*Function Declarations*/
char *gethostname(void);
char *getos(void);
char *getshell(void) {return getenv("SHELL");}
char *getterm(void) {return getenv("TERM");}
int getmem(void);
int getuptime(void);

double kb_to_gb(double input);
char *getquotesubstr(char *string);
void printascii(char *file);

int main(int argc, char *argv[]) {
	int i;

	if (argc > 1) {
		for (i = 1; i < argc; i+=2) {
			if (strcmp(argv[i], "-h") == 0) {
				printf("Sysfetch - A simple system info utlity. \
\n\nARGUMENTS:\n \
	-f - Give file to print ascii art from.\n\
	-h - Display help menu.\n\n\
Configurations can be made in the config.h file.\n");
				return 0;
			}
			else if (strcmp(argv[i], "-f") == 0) {
				printascii(argv[i+1]);
			}
			else {
				printf("Unknown command. Do -h for help.\n");
				return -1;
			}
		}
	}

	i = 0;
	while (strcmp(modules[i], "END") != 0) {
		if (strcmp(modules[i], "hostname") == 0) {
			printf( COLOR_BOLD "HOST: " COLOR_OFF " %s", gethostname());
		}
		else if (strcmp(modules[i], "os") == 0) {
			printf(COLOR_BOLD "OS: " COLOR_OFF "%s\n", getos());
		}
		else if (strcmp(modules[i], "shell") == 0) {
			printf(COLOR_BOLD "SHELL: " COLOR_OFF "%s\n", getshell());
		}
		else if (strcmp(modules[i], "term") == 0) {
			printf(COLOR_BOLD "TERM: " COLOR_OFF "%s\n", getterm());
		}
		else if (strcmp(modules[i],"mem") == 0) {
			printf(COLOR_BOLD "MEM: " COLOR_OFF); 
			getmem();
		}
		else if (strcmp(modules[i], "uptime") == 0) {
			printf(COLOR_BOLD "UPTIME: " COLOR_OFF "%d hours\n", getuptime()); 
		}
		else {
			printf("%s: UNKNOWN MODULE\n", modules[i]);
			return -1;
		}
		i++;
	}

	return 0;
}

void printascii(char *file) {
	FILE *fp = fopen(file, "r");
	char buf[64+1];

	if (fp == NULL) {
		printf("Failed to open ascii file!\n");
		exit(-1);
		return;
	}

	while ((fgets(buf, 64, fp)) != NULL) {
		printf(buf);
	}

	fclose(fp);
	return;
}

int getuptime(void) {
	FILE *file = fopen("/proc/uptime", "r");
	double buf;

	if (file == NULL) {
		return -1;
	}
	fscanf(file, "%lf", &buf);

	buf = (buf / 60) / 60;

	fclose(file);
	return buf;
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
