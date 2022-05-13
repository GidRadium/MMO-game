cmake_minimum_required(VERSION 3.5)

project(MMO-game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_DATE_TIME_NO_LIB -DBOOST_REGEX_NO_LIB -SFML_STATIC")



# libs ----------------
set(ASIO_PATH "deps/asio-1.22.1/include")
# SFML ----------
# add_definitions(-SFML_STATIC=TRUE)
#set(SFML_STATIC TRUE)
#set(SFML_STATIC_LIBRARIES TRUE)
#set(SFML_USE_STATIC_STD_LIBS TRUE)
if(UNIX)
set(SFML_PATH "deps/sfml-2.5.1-linux")
else()
set(SFML_PATH "deps/sfml-2.5.1-windows")
endif()
# ---------------------



add_executable(client src/client.cpp src/gui_base.h src/gui_base.cpp src/gui_mmo.h src/gui_mmo.cpp src/game_data.h src/game_data.cpp)
target_include_directories(client PUBLIC "${SFML_PATH}/include")
target_link_directories(client PUBLIC "${SFML_PATH}/lib")
target_link_libraries(client opengl32 winmm gdi32 sfml-system-d sfml-window-d sfml-graphics-d)


add_executable(AsioTest src/asio-test.cpp)
target_include_directories(AsioTest PUBLIC "${ASIO_PATH}")
if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(AsioTest PRIVATE Threads::Threads)
endif(UNIX)