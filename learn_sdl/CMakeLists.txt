cmake_minimum_required(VERSION 3.3)
project(learnSDL2)

set(SDL2_PATH ${PROJECT_SOURCE_DIR}/SDL2-2.0.8/i686-w64-mingw32/)
set(SDL2_IMAGE_PATH ${PROJECT_SOURCE_DIR}/SDL2_image-2.0.3/i686-w64-mingw32/)

set(SDL2_INCLUDE_DIRS ${SDL2_PATH}/include)

set(SDL2_LIBDIR "${SDL2_PATH}/lib")
set(SDL2_LIBRARIES "-lmingw32 -lSDL2main -lSDL2 -lopengl32 -lSDL2_image")

include_directories(${PROJECT_SOURCE_DIR}/../lib)

include_directories(${SDL2_INCLUDE_DIRS})

link_directories(${SDL2_LIBDIR})
link_directories(${SDL2_IMAGE_PATH}/lib)

add_executable(learnSDL2 main.cpp glad.c Obj.cpp Obj.h ObjLoad.cpp ObjLoad.h)

target_link_libraries(learnSDL2 ${SDL2_LIBRARIES})