set(model_source_files
  BendFeature.cpp
  BendFeature.h
  CMakeLists.txt
  FxtUserDatabase.cpp
  FxtUserDatabase.h
  ModelFile.cpp
  ModelFile.h
  Project.cpp
  Project.h
  BendSequence.cpp
  BendSequence.h
  MachineParam.cpp
  MachineParam.h
  ProcessPlan.cpp
  ProcessPlan.h
  Session.cpp
  Session.h
  Token.cpp
  Token.h
  Tool.cpp
  Tool.h
  Material.cpp
  Material.h
  User.cpp
  User.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)

add_library(model ${model_source_files})
target_link_libraries(model ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} wthttp wt wtdbo wtdbosqlite3 ${OpenCASCADE_LIBRARIES})