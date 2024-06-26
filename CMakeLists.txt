# 最低指定的CMake版本
cmake_minimum_required(VERSION 3.5)

# 括号里面填你的工程名
set(PRO_NAME "DreamCreator")
PROJECT(PRO_NAME)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wno-nonportable-include-path)

# QT调试需要设置编译类型
set(CMAKE_SYSTEM_PROCESSOR "arm64")
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 系统路径查找
find_package(Qt5 COMPONENTS Widgets Concurrent Network REQUIRED)

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 三方库路径
set(third-party-path ${CMAKE_CURRENT_SOURCE_DIR}/third-party)

# 添加三方库
set(glm_DIR ${third-party-path}/glm/deploy/lib/cmake/glm)
find_package(glm REQUIRED)
set(OpenCV_DIR ${third-party-path}/opencv/deploy/lib/cmake/opencv5)
find_package(OpenCV COMPONENTS opencv_core opencv_video opencv_videoio opencv_highgui REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加头文件
list(APPEND INCLUDES_DIR ${SRC_DIR}/)
list(APPEND INCLUDES_DIR ${SRC_DIR}/utils/stb_image)
list(APPEND INCLUDES_DIR ${third-party-path}/json/single_include)

# 添加pag
list(APPEND INCLUDES_DIR ${third-party-path}/deploy/libpag/include)

file(GLOB PAG_LIBS ${third-party-path}/deploy/libpag/arm64/*.a)

file(GLOB SWIFTSHADER_LIBS ${third-party-path}/deploy/libpag/swiftshader/mac/arm64/*.*)

# 遍历源代码文件，构建工程
file(GLOB_RECURSE SRC_CPP ${SRC_DIR}/*.cpp ${SRC_DIR}/*.c ${SRC_DIR}/*.frag ${SRC_DIR}/*.vert)
file(GLOB_RECURSE SRC_H ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp)

# 需要拷贝字体 icons
set(MACOSX_BUNDLE_ICON_FILE app.icns)
set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/data/icon/app.icns)
set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

file(GLOB_RECURSE FONT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/fonts/*.woff)
set_source_files_properties(${FONT_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/fonts")

add_executable(${PRO_NAME}
    MACOSX_BUNDLE
    ${SRC_CPP}
    ${SRC_H}
    ${FONT_FILES}
    ${APP_ICON_MACOSX}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
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
