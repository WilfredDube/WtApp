set(Sources
  cad-file-reader-test/cadFileReaderTest.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -lm -Wl,-R/usr/local/lib")
set (OCCT_LIBS TKSTEP;TKIGES;TKXSBase;TKBRep;TKG3d;TKTopAlgo;TKMath;TKernel) 

set (OpenCASCADE_INCLUDE_DIR  "/usr/local/include/opencascade")

find_package(Threads REQUIRED)

add_executable (cad_file_reader ${Sources})
target_include_directories(cad_file_reader PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS})
target_link_libraries(cad_file_reader view model fxtract seqgen -g ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} -lwthttp -lwt -lwtdbo -lwtdbosqlite3 -lboost_serialization)