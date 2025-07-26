#include <stdio.h>

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
	FILE *file = fopen("/usr/lib/os-release", "r");
	char buf[64+1];
	char *os;

	if (file == NULL) {
		return "Error opening os file.\n";
	}

	fgets(buf, 64, file);

	fclose(file);
	return os;
}
