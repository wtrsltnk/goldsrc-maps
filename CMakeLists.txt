cmake_minimum_required(VERSION 3.7)

project(goldsrc-maps)

list(APPEND CMAKE_PREFIX_PATH "$ENV{userprofile}/.cpppm/install")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_PREFIX_PATH}/cmake)

find_package(GLFW3)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(goldsrc-maps
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    src/imgui_impl_glfw_gl3.cpp
    include/imgui_impl_glfw_gl3.h
    src/main.cpp
    src/glad.c
    src/program.cpp
    src/program-about.cpp
    src/program-menu.cpp
    src/program-view.cpp
    src/program.h
    src/noc_file_dialog.cpp
    src/map/brushops.cpp
    src/map/brushops.h
    src/map/mapbrush.cpp
    src/map/mapbrush.h
    src/map/mapbrushside.cpp
    src/map/mapbrushside.h
    src/map/mapbrushsidematerial.cpp
    src/map/mapbrushsidematerial.h
    src/map/mapentity.cpp
    src/map/mapentity.h
    src/map/mapkeyvaluepair.cpp
    src/map/mapkeyvaluepair.h
    src/map/mapoperations.cpp
    src/map/mapoperations.h
    src/map/mapparser.cpp
    src/map/mapparser.h
    src/map/maprenderdata.cpp
    src/map/maprenderdata.h
    src/map/mapscene.cpp
    src/map/mapscene.h
    src/map/worldsize.h
    src/common/tokenizer.cpp
    include/tokenizer.h
    src/common/shader.cpp
    include/shader.h
    include/stb_image_write.h
    include/noc_file_dialog.h
    resource.rc
    )

target_include_directories(goldsrc-maps
    PUBLIC include
    PRIVATE imgui
    PRIVATE system.io/include
    PRIVATE "$ENV{userprofile}/.cpppm/install/include"
    )

target_compile_features(goldsrc-maps
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_lambdas
    PRIVATE cxx_thread_local
    )


target_link_libraries(goldsrc-maps
    ${OPENGL_LIBRARIES}
    glfw
    )

ADD_CUSTOM_COMMAND(
    TARGET goldsrc-maps
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ARGS ${CMAKE_SOURCE_DIR}/imgui/misc/fonts/Roboto-Medium.ttf ${CMAKE_BINARY_DIR}/Roboto-Medium.ttf
)
