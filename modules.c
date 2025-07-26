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

char *getos(void) {
	FILE *file = fopen("/etc/lsb-release", "r");
	char buf[64+1];
	char *os;

	#ifdef __linux__
		if (file == NULL) {
			fclose(file);
			return "Generic Linux\n";
		}

		if ( strstr(fgets(buf, 64, file), "DISTRIB_DESCRIPTION=") ) {
			printf("Found");
		}

		fclose(file);
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
