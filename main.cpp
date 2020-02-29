#include <vector>
#include <algorithm>
#include <iostream>

#include "ffmpeg_cpp_facade.h"

static void UpdateImageRGB(std::vector<uint8_t> & imageRGB, int w, int h)
{
    static int d = 50;
    imageRGB.assign(w * h * 3, 0);
    for (int i = 0; i < h; ++i)
        for ( int j = std::max(0, w / 2 - d);
              j < std::min(w, w / 2 + d);
              ++j                             )
        {
            imageRGB[(w * i + j) * 3 + 0] = 50;
            imageRGB[(w * i + j) * 3 + 1] = 200;
            imageRGB[(w * i + j) * 3 + 2] = 50;
        }
    d += 5;
}

int main()
{
    int ret = 0;

    static constexpr int w = 1920, h = 1080, fps = 20;
    static constexpr float time = 5.f;
    static constexpr int nFrames = static_cast<int>(fps * time);
    static std::vector<uint8_t> imageRGB(w * h * 3, 0);

    ffmpeg_cpp_facade::VideoSaver vs(w, h, fps, "test.mp4");
    if (vs.Error())
    {
        std::cerr << vs.Error() << '\n';
        return 1;
    }

    for (int i = 0; i < nFrames; i++)
    {
        UpdateImageRGB(imageRGB, w, h);
        if (!vs.SendFrame(&imageRGB[0]))
        {
            std::cerr << vs.Error() << '\n';
            return 1;
        }
    }

    return 0;
}
