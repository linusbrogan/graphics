# Graphics

This is a raytracer and some other graphics-related code.

Check out some renders in the [Releases](https://github.com/linusbrogan/graphics/releases).

`make` will render the scene configured in `src/raytracer_3d.c` as `2D.mp4` and `3D.mp4` under `out/Raytracer_3D`.
The 3D video is intended to be viewed with red-cyan 3D glasses.

## Dependencies
- [ImageMagick](https://imagemagick.org/) (`convert` produces XWD texture maps)
- [FFmpeg](https://ffmpeg.org/) (merge individual frames into a video)
