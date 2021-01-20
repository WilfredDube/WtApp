set(Sources
  src/BreadCrumbWidget.cpp
  src/FileDropApplication.cpp
  src/FXtractUI.cpp
  src/ModelFileWidget.cpp
  src/ModelInfoPanels.cpp
  src/ModelViewerControls.cpp
  src/ModelViewerWidget.cpp
  src/NavBarWidget.cpp
  src/PaintWidget.cpp
  src/PanelList.cpp
  src/ProjectList.cpp
  src/ProjectListWidget.cpp
  src/ProjectSummaryWidget.cpp
  src/ProcessPlanDialog.cpp
  src/FeatureDialog.cpp
)

set(Headers
  include/BreadCrumbWidget.h
  include/FileDropApplication.h
  include/FXtractUI.h
  include/ModelFileWidget.h
  include/ModelInfoPanels.h
  include/ModelViewerControls.h
  include/ModelViewerWidget.h
  include/NavBarWidget.h
  include/PaintWidget.h
  include/PanelList.h
  include/ProjectList.h
  include/ProjectListWidget.h
  include/ProjectSummaryWidget.h
  include/ProcessPlanDialog.h
  include/FeatureDialog.h
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

IF((CMAKE_CXX_STANDARD EQUAL 17) OR (CMAKE_CXX_STANDARD GREATER 17))
SET(WT_CPP17_ANY_DEFAULT_IMPLEMENTATION "std")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(wt COMPONENTS wt wtdbo wtdbosqlite3 wthttp REQUIRED)

add_library(view ${Sources} ${Headers})
target_include_directories(view PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(view fxtract ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} stdc++fs wthttp wt wtdbo wtdbosqlite3)
