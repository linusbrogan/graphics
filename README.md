# Graphics

This is a raytracer and some other graphics-related code.

Download these renders in the [Releases](https://github.com/linusbrogan/graphics/releases).
<video src="https://user-images.githubusercontent.com/6074681/196287214-ab932456-7729-45cc-801d-a8a14d199ef3.mp4" autoplay controls></video>
<video src="https://user-images.githubusercontent.com/6074681/196287999-19c791ef-f4de-4413-9c50-cce427b488e3.mp4" autoplay controls loop></video>
<video src="https://user-images.githubusercontent.com/6074681/196288390-10ad542f-5906-421b-b162-e03a11545863.mp4" autoplay controls loop></video>

`make` will render the scene configured in `src/raytracer_3d.c` as `2D.mp4` and `3D.mp4` under `out/Raytracer_3D`.
The 3D video is intended to be viewed with red-cyan 3D glasses.

## Dependencies
- [ImageMagick](https://imagemagick.org/) (`convert` produces XWD texture maps)
- [FFmpeg](https://ffmpeg.org/) (merge individual frames into a video)
