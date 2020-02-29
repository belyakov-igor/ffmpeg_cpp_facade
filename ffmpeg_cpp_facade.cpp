extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}

#include <stdexcept>

#include "ffmpeg_cpp_facade.h"

using namespace ffmpeg_cpp_facade;

struct VideoSaver::Impl
{
    FILE           * f     = nullptr;
    AVCodec        * codec = nullptr;
    AVCodecContext * c     = nullptr;
    AVPacket       * pkt   = nullptr;
    AVFrame        * frame = nullptr;
    SwsContext     * ctx   = nullptr;

    int w, h;

    const char * error = nullptr;
    size_t i = 0;

    explicit Impl(int width, int height) : w(width), h(height) {}
};

static bool encode( AVCodecContext * enc_ctx,
                    AVFrame * frame, AVPacket * pkt,
                    FILE * outfile                   )
{
    int ret;
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) return false;
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return true;
        else if (ret < 0) return false;
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
    throw std::runtime_error("Something went wrong");
}




VideoSaver::VideoSaver(int width, int height, int fps, const char * filename)
    : impl(std::make_unique<Impl>(width, height))
{
    if (width % 2 != 0 || height % 2 != 0) {
        impl->error = "Width and height must be even numbers";
        return;
    }

    impl->codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!impl->codec) {
        impl->error = "Codec \"x.264\" not found";
        return;
    }
    impl->c = avcodec_alloc_context3(impl->codec);
    if (!impl->c) {
        impl->error = "Could not allocate video codec context";
        return;
    }
    impl->pkt = av_packet_alloc();
    if (!impl->pkt) {
        impl->error = "Could not allocate packet";
        return;
    }

    // 1.8 bits / (pixel * frame)
    impl->c->bit_rate = static_cast<int64_t>(1.8f * impl->w * impl->h * fps);
    /* resolution must be a multiple of two */
    impl->c->width = impl->w;
    impl->c->height = impl->h;
    /* frames per second */
    impl->c->time_base = AVRational{ 1, static_cast<int>(fps) };
    impl->c->framerate = AVRational{ static_cast<int>(fps), 1 };

    impl->c->gop_size = 10;
    impl->c->max_b_frames = 1;
    impl->c->pix_fmt = AV_PIX_FMT_YUV420P;
    av_opt_set(impl->c->priv_data, "preset", "slow", 0);

    auto ret = avcodec_open2(impl->c, impl->codec, NULL);
    if (ret < 0) {
        static constexpr char pref[] = "Could not open codec: ";

        char msg[AV_ERROR_MAX_STRING_SIZE];
        av_make_error_string(msg, AV_ERROR_MAX_STRING_SIZE, ret);

        static char str[sizeof(pref) - 1 + AV_ERROR_MAX_STRING_SIZE];

        str[0] = '\0';
        strcat_s(str, sizeof(str), pref);
        strcat_s(str, sizeof(str), msg);

        impl->error = str;
        return;
    }

    fopen_s(&impl->f, filename, "wb");
    if (!impl->f) {
        static constexpr size_t maxFilepathLength = 256;
        static constexpr char pref[] = "Could not open ";
        static char str[maxFilepathLength + sizeof(pref) - 1];

        str[0] = '\0';
        strcat_s(str, sizeof(str), pref);
        strncat_s(str, sizeof(str), filename, maxFilepathLength);

        impl->error = str;
        return;
    }

    impl->frame = av_frame_alloc();
    if (!impl->frame) {
        impl->error = "Could not allocate video frame";
        return;
    }
    impl->frame->format = impl->c->pix_fmt;
    impl->frame->width  = impl->c->width;
    impl->frame->height = impl->c->height;
    ret = av_frame_get_buffer(impl->frame, 0);
    if (ret < 0) {
        impl->error = "Could not allocate the video frame data";
        return;
    }

    impl->ctx = sws_getContext( impl->w, impl->h, AV_PIX_FMT_RGB24,
                                impl->w, impl->h, AV_PIX_FMT_YUV420P,
                                0, 0, 0, 0                );
}

VideoSaver::~VideoSaver()
{
    encode(impl->c, NULL, impl->pkt, impl->f);
    fclose(impl->f);
    avcodec_free_context(&impl->c);
    av_frame_free(&impl->frame);
    av_packet_free(&impl->pkt);
    sws_freeContext(impl->ctx);
}

bool VideoSaver::SendFrame(uint8_t * rgbImage)
{
    if (impl->error) return false;

    auto ret = av_frame_make_writable(impl->frame);
    impl->frame->pts = impl->i++;

    const uint8_t * rgbData[1] = { &rgbImage[0] };
    const int rgbLinesize[1] = { 3 * impl->w };
    sws_scale( impl->ctx, rgbData, rgbLinesize, 0, impl->h,
               impl->frame->data, impl->frame->linesize     );

    if (!encode(impl->c, impl->frame, impl->pkt, impl->f))
    {
        impl->error = "Error during encoding";
        return false;
    }
    return true;
}

const char * VideoSaver::Error() { return impl->error; }