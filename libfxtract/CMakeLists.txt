set(Headers
  include/cad-file-reader/CadFileReader.h
  include/cad-file-reader/CadFileReaderFactory.h
  include/cad-file-reader/IgesFileReader.h
  include/cad-file-reader/StepFileReader.h
  include/cad-file-reader/NullFileReader.h
  include/cad-file-reader/UnknownFileFormatException.h

  include/obj-file-reader/readObj.h

  include/Computation.h
  include/sheet-metal-component/BoostSerializer.h
  include/sheet-metal-component/SheetMetal.h
  include/sheet-metal-component/MFaceAbstract.h
  include/sheet-metal-component/bend/Point.h

  include/sheet-metal-component/edge/ModelEdge.h

  include/sheet-metal-component/bend/BendFeature.h
  include/sheet-metal-component/bend/ModelBend.h
  include/sheet-metal-component/bend/BendLine.h

  include/sheet-metal-component/face/ModelFace.h
)

set(Sources
  src/cad-file-reader/CadFileReaderFactory.cpp
  src/cad-file-reader/IgesFileReader.cpp
  src/cad-file-reader/StepFileReader.cpp

  src/obj-file-reader/readObj.cpp
  
  src/Computation.cpp
  src/sheet-metal-component/BoostSerializer.cpp
  src/sheet-metal-component/SheetMetal.cpp
  src/sheet-metal-component/MFaceAbstract.cpp
  src/sheet-metal-component/face/ModelFace.cpp

  src/sheet-metal-component/edge/ModelEdge.cpp

  src/sheet-metal-component/bend/BendFeature.cpp
  src/sheet-metal-component/bend/ModelBend.cpp
  src/sheet-metal-component/bend/BendLine.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -lm -Wl,-R/usr/local/lib")
set (OCCT_LIBS TKSTEP;TKIGES;TKXSBase;TKBRep;TKG3d;TKTopAlgo;TKMath;TKernel) 

set (OpenCASCADE_INCLUDE_DIR  "/usr/local/include/opencascade")

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_library(fxtract STATIC ${Sources} ${Headers})
target_compile_features(fxtract PUBLIC cxx_std_17)
set_target_properties(fxtract PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(fxtract PUBLIC ${Boost_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(fxtract logger ${Boost_LIBRARIES} Threads::Threads ${OCCT_LIBS} -lboost_serialization)
