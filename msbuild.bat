cd ..
mkdir build
cd build
cmake ../source/
e:/Programs/VisualStudio/MSBuild/Current/Bin/amd64/msbuild.exe ffmpeg_cpp_facade_test.sln /p:Configuration="Release"
move *.dll Release
cd Release
ffmpeg_cpp_facade_test
d:\libs\ffmpeg\ffmpeg-4.2.2-win64-shared\bin\ffmpeg.exe -i test.mp4 -c:v copy test_.mp4
remove test.mp4
move test_.mp4 test.mp4
start test.mp4
pause