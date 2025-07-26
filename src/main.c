#include "modules.c"

int main() {
	printf("HOST: %s", gethostname());
	getos();
	printf("SHELL: %s\n", getshell());
	printf("TERM: %s\n", getterm());
	return 0;
}
