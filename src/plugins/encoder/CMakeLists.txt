
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

macro_optional_find_package(Mencoder)
macro_log_feature(MENCODER_FOUND "mencoder"
    "MEncoder is a free command line video decoding, encoding and filtering tool."
    "http://www.mplayerhq.hu/design7/news.html"
    FALSE
    ""
    "")

if(MENCODER_FOUND)
   add_subdirectory(mencoder)
endif()

if(FFMPEG_FOUND)
   add_subdirectory(ffmpeg)
endif()
