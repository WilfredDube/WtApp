set(Headers
  include/ModelUtils.h
  include/MFace.h
  include/StepProcessor.h
  include/IgesProcessor.h
  include/ModelEdge.h
  include/ModelReader.h
  include/ModelFace.h
  include/ModelBend.h
  include/Model.h
  include/ModelMath.h
)

set(Sources
  src/ModelUtils.cpp
  src/MFace.cpp
  src/StepProcessor.cpp
  src/IgesProcessor.cpp
  src/ModelEdge.cpp
  src/ModelReader.cpp
  src/ModelFace.cpp
  src/ModelBend.cpp
  src/Model.cpp
  src/ModelMath.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -lm -Wl,-R/usr/local/lib")
set (OCCT_LIBS TKSTEP;TKIGES;TKXSBase;TKBRep;TKG3d;TKTopAlgo;TKMath;TKernel) 

#;TKMath;TKG2d;TKG3d;TKGeomBase;TKBRep;TKGeomAlgo;TKTopAlgo;TKPrim;TKBO;TKShHealing;TKBool;
#TKHLR;TKFillet;TKOffset;TKFeat;TKMesh;TKXMesh;TKService;TKV3d;TKOpenGl;TKMeshVS;TKCDF;TKLCAF;
#TKCAF;TKBinL;TKXmlL;TKBin;TKXml;TKStdL;TKStd;TKTObj;TKBinTObj;TKXmlTObj;TKVCAF;TKXSBase;TKSTEPBase;
#TKSTEPAttr;TKSTEP209;TKSTEP;TKIGES;TKXCAF;TKXDEIGES;TKXDESTEP;TKSTL;TKVRML;TKXmlXCAF;TKBinXCAF;
#TKDraw;TKTopTest;TKViewerTest;TKXSDRAW;TKDCAF;TKXDEDRAW;TKTObjDRAW;TKQADraw)

set (OpenCASCADE_INCLUDE_DIR  "/usr/local/include/opencascade")

find_package(Threads REQUIRED)
#find_package(OpenCASCADE REQUIRED)
#find_library(OpenCASCADE PATHS "/usr/local/lib/" NO_DEFAULT_PATH) 

add_library(fxtract STATIC ${Sources} ${Headers})
target_compile_features(fxtract PUBLIC cxx_std_17)
set_target_properties(fxtract PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(fxtract PUBLIC ${Boost_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(fxtract ${Boost_LIBRARIES} Threads::Threads ${OCCT_LIBS})
