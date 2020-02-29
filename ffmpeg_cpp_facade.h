#ifndef FFMPEG_CPP_FACADE_H
#define FFMPEG_CPP_FACADE_H

#include <memory>

namespace ffmpeg_cpp_facade {

    struct VideoSaver final
    {
        explicit VideoSaver(int width, int height, int fps, const char* filename);
        ~VideoSaver();

        [[nodiscard]] bool SendFrame(uint8_t* rgbImage);

        const char* Error();

        VideoSaver(const VideoSaver&) = delete;
        VideoSaver(VideoSaver&&) = delete;
        VideoSaver& operator=(const VideoSaver&) = delete;
        VideoSaver& operator=(VideoSaver&&) = delete;
    private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

}

#endif // FFMPEG_CPP_FACADE_H