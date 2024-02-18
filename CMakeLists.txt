# 最低指定的CMake版本
cmake_minimum_required(VERSION 3.5)

# 括号里面填你的工程名
set(PRO_NAME "DreamCreator")
PROJECT(PRO_NAME)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wno-nonportable-include-path)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 系统路径查找
find_package(Qt5 COMPONENTS Widgets Concurrent Network REQUIRED)

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 三方库路径
set(third-party-path ${CMAKE_CURRENT_SOURCE_DIR}/third-party)

# Data路径
set(data-path ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(DATA_PATH ${data-path}/)

# 添加三方库
set(glm_DIR ${third-party-path}/glm/deploy/lib/cmake/glm)
find_package(glm REQUIRED)
set(OpenCV_DIR ${third-party-path}/opencv/deploy/lib/cmake/opencv5)
find_package(OpenCV COMPONENTS opencv_core opencv_video opencv_videoio opencv_highgui REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加头文件
list(APPEND INCLUDES_DIR ${SRC_DIR}/)
list(APPEND INCLUDES_DIR ${SRC_DIR}/common)
list(APPEND INCLUDES_DIR ${third-party-path}/json/single_include)

# 添加pag
list(APPEND INCLUDES_DIR ${third-party-path}/deploy/libpag/libpag/include)

file(GLOB PAG_LIBS ${third-party-path}/deploy/libpag/libpag/mac/arm64/*.a)
file(GLOB SWIFTSHADER_LIBS ${third-party-path}/deploy/libpag/swiftshader/mac/arm64/*.*)

# 配置文件
configure_file(
    "${SRC_DIR}/common/Config.h.in"
    "${SRC_DIR}/common/Config.h"
)

# 遍历源代码文件，构建工程
file(GLOB_RECURSE SRC_CPP ${SRC_DIR}/*.cpp ${SRC_DIR}/*.c)
file(GLOB_RECURSE SRC_H ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp)

add_executable(${PRO_NAME}
    MACOSX_BUNDLE
    ${SRC_CPP}
    ${SRC_H}
)

target_include_directories(${PRO_NAME}
    PUBLIC
    ${INCLUDES_DIR}
    ${SRC_DIR}
)

set(SYS_LIBS ${LZMA_FRAMEWORK} ${LZ_FRAMEWORK} ${LBZ2_FRAMEWORK} ${LICONV_FRAMEWORK})

if(APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices REQUIRED)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${APPLICATION_SERVICES_FRAMEWORK})
    find_library(QUARTZ_CORE QuartzCore REQUIRED)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${QUARTZ_CORE})
    find_library(COCOA Cocoa REQUIRED)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${COCOA})
    find_library(FOUNDATION Foundation REQUIRED)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${FOUNDATION})
    find_library(ICONV_LIBRARIES NAMES iconv libiconv libiconv-2 c)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${ICONV_LIBRARIES})
    find_library(VIDEOTOOLBOX VideoToolbox)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${VIDEOTOOLBOX})
    find_library(CORE_MEDIA CoreMedia)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${CORE_MEDIA})
    find_library(COMPRESSION_LIBRARIES NAMES compression)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${COMPRESSION_LIBRARIES})
elseif(WIN32)
    set(BUILD_USE_64BITS ON)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
    find_library(Bcrypt_LIB Bcrypt)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${Bcrypt_LIB})
    find_library(ws2_32_LIB ws2_32)
    list(APPEND PAG_VIEWER_PLATFORM_LIBS ${ws2_32_LIB})
endif()

target_link_libraries(${PRO_NAME}
    PRIVATE
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::Network
    glm
    opencv_core
    opencv_video
    opencv_videoio
    opencv_highgui
    ${PAG_LIBS}
    ${SWIFTSHADER_LIBS}
    ${PAG_VIEWER_PLATFORM_LIBS}
    ${SYS_LIBS}
)
