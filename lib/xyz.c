#include <stdio.h>
#include <stdlib.h>

void check_pointer(void *p, char *msg) {
printf("Checking pointer %p with warning %s\n", p, msg);
	if (p == NULL) {
		fprintf(stderr, "Bad pointer: %s\n", msg);
		exit(1);
	}
}

struct polygon {
	int points;
	int *point_list;
};

struct shape {
	int points;
	double *point_list;
	int polygons;
	struct polygon *polygon_list;
};

struct shape *load_shape(char *file_name) {
printf("Load shape %s\n", file_name);
	// Open file
	FILE* fp = fopen(file_name, "r");
	check_pointer(fp, "could not open file");

	// Allocate shape
	struct shape *shape = malloc(sizeof(struct shape));
	check_pointer(shape, "could not allocate shape");

	// Read points and find center of mass
	fscanf(fp, "%d", &(shape->points));
printf("Points: %d\n", shape->points);
	shape->point_list = malloc((shape->points+10) * 3 * sizeof(double));
	check_pointer(shape->point_list, "could not allocate shape point list");
	double xyz_avg[3] = {0, 0, 0};
	for (int pt = 0; pt < shape->points; pt++) {
if (pt>570)printf("Loading point %d\n", pt);
		for (int i = 0; i < 3; i++) {
			double *coordinate = &(shape->point_list[pt * 3 + i]);
			fscanf(fp, "%lf", coordinate);
			xyz_avg[i] += *coordinate;
		}
	}
printf("Normalize average\n");
	for (int i = 0; i < 3; i++) {
		xyz_avg[i] /= shape->points;
	}

	// Recenter points about center of mass
	for (int pt = 0; pt <= shape->points; pt++) {
printf("Recentering point %d\n", pt);
		for (int i = 0; i < 3; i++) {
			double *coordinate = &(shape->point_list[pt * 3 + i]);
			*coordinate -= xyz_avg[i];
		}
	}

	// Read polygons
	fscanf(fp, "%d", &(shape->polygons));
printf("Polygons: %d\n", shape->polygons);
	shape->polygon_list = malloc(shape->polygons * sizeof(struct polygon));
	check_pointer(shape->polygon_list, "could not allocate shape polygon list");
	for (int poly = 0; poly < shape->polygons; poly++) {
		int *points = &(shape->polygon_list[poly].points);
		fscanf(fp, "%d", points);
		int *point_list = malloc(*points * sizeof(int));
		check_pointer(point_list, "could not allocate polygon point list");
		shape->polygon_list[poly].point_list = point_list;
		for (int pt = 0; pt < *points; pt++)
			fscanf(fp, "%d", &(point_list[pt]));
	}

	// Clean up
	fclose(fp);
	return shape;
}
