#include <stdio.h>

int main() {
	char w[100];
	for (int i = 0; i < 30; i++) {
		sprintf(w, "Dog %04d.xwd", i);
		printf("%s\n", w);
	}
}
