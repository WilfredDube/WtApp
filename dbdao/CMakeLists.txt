set(Sources
#  src/BendFeature.cpp
#  src/ModelFile.cpp
#  src/Project.cpp
#  src/BendSequence.cpp
  src/ProcessPlanDao.cpp
  src/ToolDao.cpp
  src/MaterialDao.cpp
  src/UserDao.cpp
  src/ProjectDao.cpp
)

set(Headers
#  include/BendFeature.h
#  include/ModelFile.h
#  include/Project.h
#  include/BendSequence.h
  include/ProcessPlanDao.h
  include/ToolDao.h
  include/MaterialDao.h
  include/UserDao.h
  include/ProjectDao.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)

add_library(dbdao ${Sources} ${Headers})
target_link_libraries(dbdao ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} wthttp wt wtdbo wtdbosqlite3)