# -------------------------------------------------
# Project created by QtCreator 2010-01-04T22:00:59
# -------------------------------------------------
QT       += core gui widgets

TARGET = simpleencdec
TEMPLATE = app

SOURCES += main.cpp \
	 mainwindow.cpp \
         cio.cpp \
    ../../QTFFmpegWrapper/headers/ordonnanceur.cpp \
    ../../QTFFmpegWrapper/headers/agentEncoder.cpp

HEADERS += mainwindow.h \
         cio.h \
    ../../QTFFmpegWrapper/headers/ordonnanceur.hpp \
    ../../QTFFmpegWrapper/headers/agentEncoder.hpp

FORMS += mainwindow.ui
RESOURCES += resource.qrc
DEFINES += DEVELMODE

# ##############################################################################
# ##############################################################################
# FFMPEG: START OF CONFIGURATION BELOW ->
# Copy these lines into your own project
# Make sure to set the path variables for:
# 1) QTFFmpegWrapper sources (i.e. where the QVideoEncoder.cpp and QVideoDecoder.cpp lie),
# 2) FFMPEG include path (i.e. where the directories libavcodec, libavutil, etc. lie),
# 3) the binary FFMPEG libraries (that must be compiled separately).
# Under Linux path 2 and 3 may not need to be set as these are usually in the standard include and lib path.
# Under Windows, path 2 and 3 must be set to the location where you placed the FFMPEG includes and compiled binaries
# Note that the FFMPEG dynamic librairies (i.e. the .dll files) must be in the PATH
# ##############################################################################
# ##############################################################################
# ##############################################################################
# Modify here: set FFMPEG_LIBRARY_PATH and FFMPEG_INCLUDE_PATH
# ##############################################################################
# Set QTFFMPEGWRAPPER_SOURCE_PATH to point to the directory containing the QTFFmpegWrapper sources
QTFFMPEGWRAPPER_SOURCE_PATH = ../../QTFFmpegWrapper

# LINUX
#VIDENCODE = "/media/DATA/PERSO/EPSI COURS/TP/FFMPEG/viddencod/qtffmpegwrapper_src-20130507/"
# WINDOWS
VIDENCODE = ".."

# Set FFMPEG_LIBRARY_PATH to point to the directory containing the FFmpeg import libraries (if needed - typically for Windows), i.e. the dll.a files
# LINUX
#FFMPEG = "/home/vro/FFMPEG-3.0"
# WINDOWS
FFMPEG = "../../ffmpeg_lib_win32"

# Set FFMPEG_LIBRARY_PATH to point to the directory containing the FFMPEG libs
# LINUX
#FFMPEG_LIBRARY_PATH = "$$FFMPEG/lib"
# WINDOWS
FFMPEG_LIBRARY_PATH = "$$FFMPEG/lib/lib"

# Set FFMPEG_INCLUDE_PATH to point to the directory containing the FFMPEG includes (if needed - typically for Windows)
FFMPEG_INCLUDE_PATH = "$$FFMPEG/include"

# ##############################################################################
# Do not modify: FFMPEG default settings
# ##############################################################################

# Sources for QT wrapper
SOURCES += $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoEncoder.cpp
HEADERS += $$QTFFMPEGWRAPPER_SOURCE_PATH/headers/QVideoEncoder.h
SOURCES += $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoDecoder.cpp
HEADERS += $$QTFFMPEGWRAPPER_SOURCE_PATH/headers/QVideoDecoder.h

# Sub-layer for the TP
SOURCES += $$VIDENCODE/simpleencdec/QVideoEncoderTest.cpp
HEADERS += $$VIDENCODE/simpleencdec/QVideoEncoderTest.hpp
SOURCES += $$VIDENCODE/simpleencdec/QVideoDecoderTest.cpp
HEADERS += $$VIDENCODE/simpleencdec/QVideoDecoderTest.hpp

# add the include path
# for ffmpeg.h
INCLUDEPATH += $$QTFFMPEGWRAPPER_SOURCE_PATH

# for lib*.h ffmpeg source
INCLUDEPATH += $$FFMPEG_INCLUDE_PATH

# Add the library path for ffmpeg
LIBS += -L$$FFMPEG_LIBRARY_PATH

# Set list of required FFmpeg libraries parameters for the linker
LIBS += -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -lswresample

# Requied for some C99 defines
DEFINES += __STDC_CONSTANT_MACROS

# ##############################################################################
# FFMPEG: END OF CONFIGURATION
# ##############################################################################
