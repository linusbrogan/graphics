#include <unistd.h>

int main() {
	char *argv[] = {"display", "asset/Space_station_v.xwd", NULL};
	execvp(argv[0], argv);
}
