SRC_FILES = \
	2d_Simple_RaytracerS \
	3d_shapes \
	3d_Simple_RaytracerDS \
	FPToolkitDemoB \
	functional \
	get_image_from_file \
	growing_sun \
	M3d_test_sequence \
	M3d_test_view \
	movie_player \
	raytracer_3d \
	shapes \
	space_station \
	sprint \
	stickfigureS \
	view_test04Mb \
	view_test05MbS \
	xwd_blend_example \
	xwd_no_graphics_window_example

LIB_FILES = \
	2d_shape_functions \
	3d_shape_functions \
	g3d \
	lg \
	light_model \
	raytracing \
	rt_shapes \
	rt_utils \
	shape_colors \
	xwd_tools_03

SRC_TARGETS = $(addprefix build/,$(SRC_FILES))
LIB_TARGETS = $(addprefix build/,$(addsuffix .o,$(LIB_FILES)))

INCLUDES = -Iinclude
C_FLAGS = -lm -lX11 $(INCLUDES)

.PHONY: all clean default test

default: clean raytracer

raytracer: build/3d_Simple_RaytracerDS build/raytracer_3d build/movie_player
	./build/3d_Simple_RaytracerDS
	display out/3d_Simple_RaytracerDS.xwd
	./build/raytracer_3d
	./build/movie_player 1080 720 out/Raytracer_3D/frame_ 0 99 1 40000

asset/earth.xwd:
	convert asset/earth.jpg asset/earth.convert.xwd
	./tool/change_CONVERTxwd_to_JEFFxwd.exec asset/earth.convert.xwd asset/earth.xwd

space_station: build/space_station build/movie_player asset/earth.xwd
	time ./build/space_station 0 100
	display out/Space_Station/frame_0000.xwd
	./build/movie_player 1080 720 out/Space_Station/frame_ 0 99 0 40000

all: $(SRC_TARGETS) $(LIB_TARGETS)

$(SRC_TARGETS):
	mkdir -p $(dir $@)
	cc -o $@ src/$(notdir $@).c $(C_FLAGS)

$(LIB_TARGETS):
	mkdir -p $(dir $@)
	cc -c -o $@ lib/$(basename $(notdir $@)).c $(INCLUDES)

test: asset/earth.xwd $(SRC_TARGETS)
	./build/2d_Simple_RaytracerS
	-timeout 10 ./build/3d_shapes
	./build/movie_player 1080 720 out/Graph_3D/frame_ 0 $$(($$(ls out/Graph_3D | wc -l) - 1)) 1 40000
	./build/FPToolkitDemoB
	./build/functional
	echo "700 650 asset/a.xwd" | ./build/get_image_from_file
	./build/growing_sun 600 400 100 1.5
	./build/M3d_test_sequence > out/M3d_test_sequence.out
	diff out/M3d_test_sequence.res out/M3d_test_sequence.out
	./build/M3d_test_view > out/M3d_test_view.out
	diff out/M3d_test_view.res out/M3d_test_view.out
	./build/movie_player 600 400 out/Growing_Sun/frame_ 0 99 1 30000
	./build/sprint
	./build/stickfigureS 640 30000
	./build/view_test04Mb
	display out/view_test04Mb0000.jpg
	./build/view_test05MbS
	./build/shapes
	display out/2dshapes02.xwd
	./tool/play_xwd_movieC.exec ./out/ViewMatrixMovie/pimage 0 90
	echo asset/clock.xwd asset/mandelbrot_set.xwd | ./build/xwd_blend_example
	./build/xwd_no_graphics_window_example
	display out/taller.xwd
	display out/wider.xwd

clean:
	-rm -r build
	-rm out/2d_Simple_RaytracerS.xwd
	-rm out/3d_Simple_RaytracerDS.xwd
	-rm -r out/Graph_3D
	-rm out/demo.bmp
	-rm -r out/Growing_Sun
	-rm out/M3d_test_sequence.out
	-rm out/M3d_test_view.out
	-rm -r out/Raytracer_3D
	-rm -r out/Space_Station
	-rm asset/earth.convert.xwd asset/earth.xwd
	-rm out/view_test04Mb0000.xwd
	-rm out/taller.xwd out/wider.xwd
