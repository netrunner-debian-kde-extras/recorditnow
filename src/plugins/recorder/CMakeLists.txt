add_subdirectory(recordmydesktop)
add_subdirectory(screenshot)

if(FFMPEG_FOUND)
    add_subdirectory(ffmpegrecorder)
endif()
