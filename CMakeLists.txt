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


# CLIENT
add_executable(
    client 
    src/client/client.cpp 
    src/client/gui_base.h 
    src/client/gui_base.cpp 
    src/client/gui_mmo.h 
    src/client/gui_mmo.cpp 
    src/common/game_data.h 
    src/common/game_data.cpp 
    src/common/net_common.h 
    src/common/olcPGEX_Network.h 
    
)

target_include_directories(client PUBLIC "${ASIO_PATH}")

target_include_directories(client PUBLIC "${SFML_PATH}/include")
target_link_directories(client PUBLIC "${SFML_PATH}/lib")
target_link_libraries(client opengl32 winmm gdi32 sfml-system-d sfml-window-d sfml-graphics-d)


# SERVER
add_executable(
    server 
    src/server/server.cpp 
    src/common/game_data.h 
    src/common/game_data.cpp 
    src/common/net_common.h 
    src/common/olcPGEX_Network.h 
)

target_include_directories(server PUBLIC "${ASIO_PATH}")
target_include_directories(server PUBLIC "${SFML_PATH}/include")

target_link_directories(server PUBLIC "${SFML_PATH}/lib")
target_link_libraries(server sfml-system-d)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(server PRIVATE Threads::Threads)
endif(UNIX)
