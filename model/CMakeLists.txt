set(Sources
  src/BendFeature.cpp
  src/FxtUserDatabase.cpp
  src/ModelFile.cpp
  src/Project.cpp
  src/BendSequence.cpp
  src/ProcessPlan.cpp
  src/Session.cpp
  src/Token.cpp
  src/Tool.cpp
  src/Material.cpp
  src/User.cpp
  src/ProjectDao.cpp
)

set(Headers
  include/BendFeature.h
  include/FxtUserDatabase.h
  include/ModelFile.h
  include/Project.h
  include/BendSequence.h
  include/ProcessPlan.h
  include/Session.h
  include/Token.h
  include/Tool.h
  include/Material.h
  include/User.h
  include/ProjectDao.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)

add_library(model ${Sources} ${Headers})
target_link_libraries(model ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} wthttp wt wtdbo wtdbosqlite3 ${OpenCASCADE_LIBRARIES})