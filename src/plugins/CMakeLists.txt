set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})


macro_optional_find_package(ffmpeg)
macro_log_feature(FFMPEG_FOUND "ffmpeg"
    "Complete solution to record, convert and stream audio and video. Includes libavcodec."
    "http://ffmpeg.org/"
    FALSE
    ""
    "")


add_subdirectory(recorder)
add_subdirectory(encoder)
