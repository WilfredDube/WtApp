cmake_minimum_required(VERSION 3.0)

project(hello_x)

add_subdirectory(logging)
add_subdirectory(libseqgen)
add_subdirectory(libfxtract)
add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(tests)

set(main_source_files
 main.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

IF((CMAKE_CXX_STANDARD EQUAL 17) OR (CMAKE_CXX_STANDARD GREATER 17))
SET(WT_CPP17_ANY_DEFAULT_IMPLEMENTATION "std")
ENDIF()

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs -Wall -lm -Wl,-R/usr/local/lib")
set(wt_INCLUDE_DIRS /usr/local/include/wt)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt dbo dbosqlite3 http REQUIRED)

add_executable (hello ${main_source_files})
target_include_directories(hello PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS})
target_link_libraries(hello logger view model fxtract seqgen  -g ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} -lwthttp -lwt -lwtdbo -lwtdbosqlite3 -lboost_serialization)
