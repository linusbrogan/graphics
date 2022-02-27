CFILES = \
	3d_shapes \
	FPToolkitDemoB \
	functional \
	get_image_from_file \
	growing_sun \
	M3d_test_sequence \
	M3d_test_view \
	movie_player \
	sprint \
	stickfigureS \
	view_test04Mb \
	view_test05MbS \
	shapes

LIBFILES = \
	2d_shape_functions \
	3d_shape_functions \
	g3d \
	lg \
	light_model \
	shape_colors

LIBOFILES = $(addsuffix .o,$(LIBFILES))

CFLAGS = -lm -lX11
INCLUDES = -Iinclude

.PHONY: all clean test

default: main

all: $(CFILES) $(LIBOFILES) test

$(CFILES):
	cc -o $@ $@.c $(CFLAGS)

$(LIBOFILES):
	cc -c $(basename $@).c

main: 3d_shapes
	./3d_shapes

test: $(CFILES)
	./FPToolkitDemoB
	./functional
	echo "700 650 a.xwd" | ./get_image_from_file
	./growing_sun 600 400 100 1.5
	./M3d_test_sequence > M3d_test_sequence.out
	diff M3d_test_sequence.res M3d_test_sequence.out
	./M3d_test_view > M3d_test_view.out
	diff M3d_test_view.res M3d_test_view.out
	./movie_player 600 400 Growing_Sun/frame_ 0 99 1 30000
	./sprint
	./stickfigureS 640 30000
	./view_test04Mb
	display view_test04Mb0000.jpg
	./view_test05MbS
	./shapes
	display 2dshapes02.xwd
	./ViewMatrixMovie/play_xwd_movieC.exec ./ViewMatrixMovie/pimage 0 90

clean:
	-rm $(CFILES)
	-rm -r Graph_3D
	-rm demo.bmp
	-rm -r Growing_Sun
	-rm M3d_test_sequence.out
	-rm M3d_test_view.out
	-rm view_test04Mb0000.xwd
	-rm $(LIBOFILES)
