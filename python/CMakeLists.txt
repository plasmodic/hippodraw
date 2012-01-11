# ## Process this file with automake to produce Makefile.in
# #
# # Copyright (C) 2002-2006  The Board of Trustees of The Leland
# # Stanford Junior University.  All Rights Reserved.
# #
# # $Id: Makefile.am,v 1.213 2007/07/25 18:40:30 pfkeb Exp $
# #
# # Author: Paul_Kunz@slac.stanford.edu
# #
# 

# Sources for the Python extension module 
#include_directories($ENV{I3_SRC}/boost/public 
#  $ENV{I3_PORTS}/include/boost-1.33.1 

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_PATH}
  ${PYTHON_NUMPY_INCLUDE_DIR}/..
  ${PYTHON_NUMPY_INCLUDE_DIR}/../../../numarray
  ${CMAKE_BINARY_DIR}
)

set(HIPPOMOD_SRCS
  ObserverWrap.cxx
  QtCut.cxx
  QtDisplay.cxx
  PyApp.cxx
  PyFunctionRep.cxx
  PyDataRep.cxx
  PyDataSource.cxx
  PyNTuple.cxx
  PyNTupleController.cxx
  FunctionWrap.cxx
  ListTuple.cxx
  exportToPython.cxx
  export_Color.cxx
  export_CircularBuffer.cxx
  export_CutController.cxx
  export_QtCut.cxx
  export_DataSource.cxx
  export_FCNBase.cxx
  export_Fitter.cxx
  export_FitterFactory.cxx
  export_FunctionFactory.cxx
  export_LineStyle.cxx
  export_ListTuple.cxx
  export_NTuple.cxx
  export_NTupleController.cxx
  export_NTupleFCN.cxx
  export_PointRepFactory.cxx
  export_RepBase.cxx
  export_StatedFCN.cxx
  export_SymbolType.cxx

  # QT stuff
  PyCanvas.cxx
  export_Canvas.cxx
  export_CanvasWindow.cxx

  hippomodule.cxx
)


# Add code to export optional components

if (HAVE_CFITSIO)
   set(HIPPOMOD_SRCS
	${HIPPOMOD_SRCS}
 	PyFitsController.cxx
 	export_FitsController.cxx
 	export_FitsNTuple.cxx
   )
endif (HAVE_CFITSIO)

if (HAVE_ROOT)
   set(HIPPOMOD_SRCS
	${HIPPOMOD_SRCS}
	export_RootController.cxx
	export_RootNTuple.cxx
   )
endif (HAVE_ROOT)

if (HAVE_MINUIT OR HAVE_MINUIT2)
   include_directories( ${MINUIT2_INCLUDE_DIR} )
endif (HAVE_MINUIT OR HAVE_MINUIT2)

attach_srcgroups(HIPPOMOD_SRCS ${EXTRA_PYMODULES})

add_library(hippomodule MODULE
  ${HIPPOMOD_SRCS}
)

set_target_properties(hippomodule
  PROPERTIES
  PREFIX ""
  )

target_link_libraries(hippomodule hippo 
  ${PYTHON_LIBRARY} ${Boost_PYTHON_LIBRARY} ${QT_LIBRARIES})

if (NOT PYTHON_INSTDIR)
  execute_process(
  COMMAND
  ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_EXEC_PREFIX}')"
  OUTPUT_VARIABLE PYTHON_INSTDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  SET(PYTHON_INSTDIR ${PYTHON_INSTDIR} CACHE EXTERNAL "Location to which the HippoDraw Python module will be installed")
endif (NOT PYTHON_INSTDIR)

install_files(/include/HippoDraw/python .h)
install_files(/${PYTHON_INSTDIR}/HippoDraw .py)
install_targets(/${PYTHON_INSTDIR} hippomodule)

