# ## Process this file with automake to produce Makefile.in
# #
# # Copyright (C) 2005, 2006 The Board of Trustees of The Leland Stanford
# # Junior University.  All Rights Reserved.
# #
# # $Id: Makefile.am,v 1.31 2007/07/23 18:10:23 pfkeb Exp $
# #
# # Author: <Paul_Kunz@slac.stanford.edu>
# #
# 
# # The following is set; otherwise, it would follow GNU conventions.
# AUTOMAKE_OPTIONS = foreign
# 
# ## Subsitution from bnv_have_qt
# QT_CXXFLAGS = @QT_CXXFLAGS@
# QT_DIR = @QT_DIR@
# QT_MOC = @QT_MOC@
# QT_UIC = @QT_UIC@
# 
# MINUIT_LDFLAGS = @MINUIT_LDFLAGS@
# MINUIT2_LDFLAGS = @MINUIT2_LDFLAGS@
# 
# WCSLIB_LDFLAGS = @WCSLIB_LDFLAGS@
# 
# # The -u argument needed for Mac OS X
# if ROOTBUILD
# ROOTLDFLAGS = @ROOT_LDFLAGS@ -u _G__cpp_setup_initializerG__Tree
# else
# ROOTLDFLAGS =
# endif
# 
# BOOST_CXXFLAGS = @BOOST_CXXFLAGS@ -ftemplate-depth-32
# 
# ## Subsitution from pfkeb_cxx_lib_path
# CXX_LIB_PATH = @CXX_LIB_PATH@ 
# 
# EXEEXT =
# 
# 


add_definitions(-DHAVE_CONFIG_H -DQT_THREAD_SUPPORT)

if (APPLE)
  set(HIPPODRAW_NAME HippoDraw)
else (APPLE)
  set(HIPPODRAW_NAME hippodraw)
endif (APPLE)

add_executable(${HIPPODRAW_NAME} MACOSX_BUNDLE
  ${CMAKE_SOURCE_DIR}/images/hippodraw.icns
  main.cxx)

set_source_files_properties(
	${CMAKE_SOURCE_DIR}/images/hippodraw.icns PROPERTIES
	MACOSX_PACKAGE_LOCATION Resources
)

target_link_libraries(${HIPPODRAW_NAME} hippo ${QT_LIBRARIES})

set_target_properties(${HIPPODRAW_NAME} PROPERTIES
  MACOSX_BUNDLE_ICON_FILE hippodraw.icns
  MACOSX_BUNDLE_BUNDLE_NAME HippoDraw
  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
)

install(TARGETS ${HIPPODRAW_NAME} 
	RUNTIME DESTINATION bin
	BUNDLE	DESTINATION /Applications
)

install_files(/share/applications HippoDraw.desktop)
