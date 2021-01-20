set(Sources
  src/BendFeatureDao.cpp
#  src/ModelFile.cpp
#  src/BendSequence.cpp
  src/ProcessPlanDao.cpp
  src/ToolDao.cpp
  src/MaterialDao.cpp
  src/UserDao.cpp
  src/ProjectDao.cpp
)

set(Headers
  include/BendFeatureDao.h
#  include/ModelFile.h
#  include/BendSequence.h
  include/ProcessPlanDao.h
  include/ToolDao.h
  include/MaterialDao.h
  include/UserDao.h
  include/ProjectDao.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -lm -Wl,-R/usr/local/lib")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)

set (OpenCASCADE_INCLUDE_DIR  "/usr/local/include/opencascade")

add_library(dbdao ${Sources} ${Headers})
target_compile_features(fxtract PUBLIC cxx_std_17)
target_include_directories(dbdao PUBLIC ${Boost_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(dbdao seqgen ${Boost_LIBRARIES} ${OCCT_LIBS} Threads::Threads ${wt_LIBRARIES} wthttp wt wtdbo wtdbosqlite3)