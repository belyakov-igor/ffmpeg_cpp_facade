# ffmpeg c++ facade

Use the VideoSaver class to merge a sequence of raw RGB images into a video file with x264 encoder. Just put ffmpeg_cpp_facade.* files into your project.

  - Create VideoSaver object to start saving file
  - Feed RGB images to it (each must be a sequence of uint8_t: ...rgbrgbrgb...)
  - VideoSaver's destructor finishes the work and does some clean-up
  - Done. Your video file misses timestamps though. It must be some kind of misuse of ffmpeg libraries which I couldn't figure out yet. It can be fixed with ffmpeg utility: `ffmpeg.exe -i test.mp4 -c:v copy fixed.mp4`

To build ffmpeg_cpp_facade_test project, create lib_paths.cmake file with your library paths, like in lib_paths(example).cmake. To build it on Windows, put your paths in msbuild.bat and launch it.