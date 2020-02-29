if (WIN32)
    set(FFMPEG_INCLUDE_PATH d:/libs/ffmpeg/ffmpeg-4.2.2-win64-dev/include)
    file(GLOB FFMPEG_LIB_PATHS LIST_DIRECTORIES false d:/libs/ffmpeg/ffmpeg-4.2.2-win64-shared/bin/*.dll)

    set(FFMPEG_LIB_DIR d:/libs/ffmpeg/ffmpeg-4.2.2-win64-shared/bin)
    set(FFMPEG_IMPLIB_DIR d:/libs/ffmpeg/ffmpeg-4.2.2-win64-dev/lib)


    set(AVCODEC_LIB_PATH    ${FFMPEG_LIB_DIR}/avcodec-58.dll)
    set(AVCODEC_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/avcodec.lib)

    set(AVDEVICE_LIB_PATH    ${FFMPEG_LIB_DIR}/avdevice-58.dll)
    set(AVDEVICE_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/avdevice.lib)

    set(AVFILTER_LIB_PATH    ${FFMPEG_LIB_DIR}/avfilter-7.dll)
    set(AVFILTER_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/avfilter.lib)

    set(AVFORMAT_LIB_PATH    ${FFMPEG_LIB_DIR}/avformat-58.dll)
    set(AVFORMAT_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/avformat.lib)

    set(AVUTIL_LIB_PATH    ${FFMPEG_LIB_DIR}/avutil-56.dll)
    set(AVUTIL_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/avutil.lib)

    set(POSTPROC_LIB_PATH    ${FFMPEG_LIB_DIR}/postproc-55.dll)
    set(POSTPROC_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/postproc.lib)

    set(SWRESAMPLE_LIB_PATH    ${FFMPEG_LIB_DIR}/swresample-3.dll)
    set(SWRESAMPLE_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/swresample.lib)

    set(SWSCALE_LIB_PATH    ${FFMPEG_LIB_DIR}/swscale-5.dll)
    set(SWSCALE_IMPLIB_PATH ${FFMPEG_IMPLIB_DIR}/swscale.lib)
else()
endif()