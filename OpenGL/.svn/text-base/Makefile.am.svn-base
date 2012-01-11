## Process this file with automake to produce Makefile.in
#
#  Copyright (C)  2003, 2005, 2006  The Board of Trustees of  
#  The Leland Stanford Junior University.  All Rights Reserved.
#
# $Id: Makefile.am,v 1.22 2006/09/21 17:51:34 pfkeb Exp $
#
# Author: <Paul_Kunz@slac.stanford.edu>
#

# The following is set; otherwise, it would follow GNU conventions.
AUTOMAKE_OPTIONS = foreign

SUBDIRS = . tests

## Subsitution from pfkeb_cxx_lib_path
CXX_LIB_PATH = @CXX_LIB_PATH@

GL_LIBS = @GL_LIBS@


## Subsitution from bnv_have_qt, need qt for XML
QT_CXXFLAGS = @QT_CXXFLAGS@
QT_DIR = @QT_DIR@
QT_MOC = @QT_MOC@
QT_UIC = @QT_UIC@

MINUIT_LDFLAGS = @MINUIT_LDFLAGS@
ROOT_LDFLAGS = @ROOT_LDFLAGS@

lib_LTLIBRARIES = libhippoGL.la

hippoGLincludedir = $(includedir)/hippoGL

hippoGLinclude_HEADERS =			\
	OpenGL.h				\
	OpenGLApp.h				\
	OpenGLView.h				\
	OpenGLWindow.h				\
	TextTTF.h

libhippoGL_la_SOURCES =				\
	OpenGLApp.cxx				\
	OpenGLView.cxx				\
	OpenGLWindow.cxx			\
	TextTTF.cxx

EXTRA_DIST =OpenGL.dir README

# libhippo_la_SOURCES = $(libhippoGL_la_SOURCES)

libhippoGL_la_LIBADD =				\
	$(top_builddir)/lib/libhippo.la		\
	/usr/lib/libfreetype.la

libhippoGL_la_LDFLAGS =				\
	$(MINUIT_LDFLAGS)			\
	$(ROOT_LDFLAGS)				\
	$(GL_LIBS)				\
	-lttf

## Needs `-DHAVE_TTF' in the following, but only if freetype 1 is available.

DEFS = -DHAVE_CONFIG_H  -DHAVE_TTF

INCLUDES =					\
	-I$(top_srcdir)				\
	-I/usr/include/freetype1/freetype

# For platforms where automake dependency generation doesn't work.
depend: $(libhippoGL_la_SOURCES)
	$(top_srcdir)/config/makedepend $(DEFS) $(INCLUDES) $(CPPFLAGS) $?

# An extra clean target for a Sun CC compiler.
clean-local:
	-rm -rf SunWS_cache
	-rm -rf hippodraw
