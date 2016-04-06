#! /bin/bash
# raspistill/raspivid/raspiyuv

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT( camcv )
SET(COMPILE_DEFINITIONS -Werror)

FIND_PACKAGE( OpenCV REQUIRED )

SET(GL_SCENE_SOURCES
gl_scenes/models.c
gl_scenes/mirror.c
gl_scenes/yuv.c
gl_scenes/sobel.c
gl_scenes/square.c
gl_scenes/teapot.c)

INCLUDE_DIRECTORIES(/home/sean/Desktop/Project/camvc)


INCLUDE_DIRECTORIES(/usr/local/include/opencv)
INCLUDE_DIRECTORIES(/opt/vc/include)
INCLUDE_DIRECTORIES(/opt/vc/include/EGL)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland/host_applications/linux/libs/bcm_host/include)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland/interface/vcos)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland/interface/vcos/pthreads)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland/interface/vmcs_host/linux)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland/interface/khronos/include)

INCLUDE_DIRECTORIES(~/Source/userland_mmal/userland/interface/khronos/common)

#INCLUDE_DIRECTORIES(/opt/vc/include/)


ADD_EXECUTABLE(camcv RaspiCamControl.c RaspiTex.c RaspiCLI.c RaspiPreview.c camcv_vid1.cpp RaspiTexUtil.c tga.c ${GL_SCENE_SOURCES})

TARGET_LINK_LIBRARIES(camcv /opt/vc/lib/libmmal_core.so /opt/vc/lib/libmmal_util.so /opt/vc/lib/libmmal_vc_client.so /opt/vc/lib/libvcos.so /opt/vc/lib/libbcm_host.so /opt/vc/lib/libGLESv2.so /opt/vc/lib/libEGL.so libpthread.so libm.so /home/sean/Source/libfacerec-0.04/libopencv_facerec.a ${OpenCV_LIBS})

