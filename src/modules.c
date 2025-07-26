#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char *getos(void) { /*WIP*/
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

		printf("OS: %s\n", os);

		fclose(file);
		free(os);
		return os;
	#elif __FreeBSD__
		return "FreeBSD";
	#elif __OpenBSD__
		return "OpenBSD";
	#endif
		return "Error finding OS.";
}

char *getshell(void) {
	return getenv("SHELL");
}

char *getterm(void) {
	return getenv("TERM");
}
