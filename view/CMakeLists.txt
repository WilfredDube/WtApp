set(view_source_files
  BreadCrumbWidget.cpp
  BreadCrumbWidget.h
  FileDropApplication.cpp
  FileDropApplication.h
  FXtractUI.cpp
  FXtractUI.h
  ModelFileWidget.cpp
  ModelFileWidget.h
  ModelInfoPanels.cpp
  ModelInfoPanels.h
  ModelViewerControls.cpp
  ModelViewerControls.h
  ModelViewerWidget.cpp
  ModelViewerWidget.h
  NavBarWidget.cpp
  NavBarWidget.h
  PaintWidget.cpp
  PaintWidget.h
  PanelList.cpp
  PanelList.h
  ProjectList.cpp
  ProjectList.h
  ProjectListWidget.cpp
  ProjectListWidget.h
  ProjectSummaryWidget.cpp
  ProjectSummaryWidget.h
  readObj.cpp
  readObj.h
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

add_library(view ${view_source_files})
target_include_directories(view PRIVATE ${Boost_INCLUDE_DIRS} ${wt_INCLUDE_DIRS} ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(view fxtract ${Boost_LIBRARIES} Threads::Threads ${wt_LIBRARIES} stdc++fs wthttp wt wtdbo wtdbosqlite3)
