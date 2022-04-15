#include "lib/xyz.c"

int main() {
	setbuf(stdout, NULL);
printf("Go\n");
	struct shape *s = load_shape("asset/xyz/sphere.xyz");
	printf("s: {%d pts, %d polys}\n", s->points, s->polygons);
	initialize_xyz_files();
	printf("WORKED");
}
