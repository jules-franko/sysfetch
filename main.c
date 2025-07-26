#include "modules.c"

int main() {
	printf("HOST: %s", gethostname());
	printf("OS: %s", getos());
	printf("SHELL: %s\n", getshell());
	return 0;
}
