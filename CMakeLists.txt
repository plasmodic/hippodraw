
cmake_minimum_required(VERSION 2.6)

set(SUBDIRS 
  axes 
  binners 
  colorreps 
  controllers
  datareps
  datasrcs
  functions
  graphics 
  images
  minimizers
  pattern
  plotters
  qtui
  projectors
  reps
  transforms
  qt
  qtxml
  xml

  python
  lib
  )

SET(SUBDIRS
  ${SUBDIRS}
  )

set(PACKAGE_VERSION "1.21.3")
set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_C_FLAGS "-fPIC")

set(CMAKE_MODULE_PATH
  ${EXTRA_CMAKE_MODULE_PATH}
  ${CMAKE_SOURCE_DIR}/cmake)

# set(DESIRED_QT_VERSION "4.3.4")

set(QT_REQUIRED TRUE)
set(QT_USE_QTXML TRUE)

find_package(Qt4)

unset(CFITSIO_LIBRARIES)
unset(CFITSIO_LIBRARIES CACHE)
# Optional Packages
find_package(PythonLibs)
find_package(PythonInterp)
find_package(Boost COMPONENTS python)
find_package(Numpy)
find_package(CFITSIO)
find_package(LATEX)
find_package(Minuit2)
find_package(ROOT)

set(USE_WCSLIB True)

if (QT4_FOUND)
   SET(HAVE_QT4 TRUE)
endif (QT4_FOUND)

add_definitions(-DQT_THREAD_SUPPORT)
include(${QT_USE_FILE})
include(SourceGroup)

if (PYTHON_NUMPY_FOUND)
   SET(HAVE_NUMPY TRUE)
   SET(HAVE_NUMARRAY TRUE)

   SET(SUBDIRS numarray ${SUBDIRS} )
   SET(EXTRA_PYMODULES numarray ${EXTRA_PYMODULES} )
   SET(EXTRA_FEATURES ${EXTRA_FEATURES} "Python Numarray Support")
endif (PYTHON_NUMPY_FOUND)

if (CFITSIO_FOUND)
   SET(HAVE_CFITSIO TRUE)

   SET(SUBDIRS fits ${SUBDIRS} )
   SET(EXTRA_MODULES fits ${EXTRA_MODULES} )
   SET(EXTRA_FEATURES ${EXTRA_FEATURES} "FITS File Support")
   SET(EXTRA_MODULES_LIBS ${CFITSIO_LIBRARIES} ${EXTRA_MODULES_LIBS} )
endif (CFITSIO_FOUND)

if (ROOT_FOUND)
   SET(HAVE_ROOT TRUE)

   SET(SUBDIRS root ${SUBDIRS} )
   SET(EXTRA_MODULES root ${EXTRA_MODULES} )
   SET(EXTRA_PYMODULES pyroot ${EXTRA_PYMODULES} )
   SET(EXTRA_FEATURES ${EXTRA_FEATURES} "ROOT File Support")
   SET(EXTRA_MODULES_LIBS "-L/usr/X11/lib" ${ROOT_LIBRARIES} ${EXTRA_MODULES_LIBS} )
endif (ROOT_FOUND)

if (MINUIT2_FOUND)
   SET(HAVE_MINUIT2 TRUE)

   SET(EXTRA_FEATURES ${EXTRA_FEATURES} Minuit2 )
   SET(EXTRA_MODULES_LIBS ${MINUIT2_LIBRARIES} ${EXTRA_MODULES_LIBS} )
endif (MINUIT2_FOUND)

if (USE_WCSLIB)
   SET(HAVE_WCSLIB TRUE)
   SET(WCS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/wcslib)

   SET(SUBDIRS wcslib ${SUBDIRS} )
   SET(EXTRA_FEATURES ${EXTRA_FEATURES} "World Coordinate System" )
   SET(EXTRA_MODULES_LIBS wcs ${EXTRA_MODULES_LIBS} )
endif (USE_WCSLIB)

if (LATEX_COMPILER)
   SET(HAVE_TEX_UTILS TRUE)

   SET(EXTRA_FEATURES ${EXTRA_FEATURES} "LaTeX" )
endif (LATEX_COMPILER)

# Set up config.h and include paths

configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake.in
  ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/config.h
  @ONLY)
add_definitions(-DHAVE_CONFIG_H -DHAVE_QT_APP)
include_directories(${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
include_directories(${CMAKE_SOURCE_DIR})

# Include subdirectories and print a status report

foreach(DIR ${SUBDIRS})
  add_subdirectory(${DIR})
endforeach()

MESSAGE(STATUS "Enabled optional features: ") 

foreach(DIR ${EXTRA_FEATURES})
	MESSAGE(STATUS "  " ${DIR})
endforeach()

