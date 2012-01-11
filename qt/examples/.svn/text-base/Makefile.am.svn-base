## Process this file with automake to produce Makefile.in
#
# Copyright (C) 2001-2003, 2006 The Board of Trustees of The Leland Stanford
# Junior University.  All Rights Reserved.
#
# $Id: Makefile.am,v 1.129 2007/08/29 21:07:08 pfkeb Exp $
#
# Author: <Paul_Kunz@slac.stanford.edu>
#

# The following is set; otherwise, it would follow GNU conventions.
AUTOMAKE_OPTIONS = foreign

## Subsitution from bnv_have_qt
QT_CXXFLAGS = @QT_CXXFLAGS@
QT_DIR = @QT_DIR@
QT_MOC = @QT_MOC@
QT_UIC = @QT_UIC@

#QT_UIC = $(QT_DIR)/bin/uic <- Does not compile on debian

## Subsitution from pfkeb_cxx_lib_path
CXX_LIB_PATH = @CXX_LIB_PATH@

ROOTLDFLAGS = @ROOT_LDFLAGS@

CFITSIOLDFLAGS = @CFITSIO_LDFLAGS@

EXEEXT = 

BUILT_SOURCES =					\
	QtViewWidgetWindow.h			\
	QtViewWidgetWindow.cpp			\
	moc_QtViewWidgetWindow.cpp

hippoqtdir = $(pkgincludedir)/qt/examples

noinst_HEADERS =				\
	QtViewWidgetWindow.ui			\
	QtViewWidgetWindow.ui.h

EXTRA_DIST = \
	QtViewWidgetWindow.ui \
	testqtviewwidget.pro

check_PROGRAMS =				\
	testPrinter				\
	testProfile				\
	testwidget				\
	testwindow				\
	teststatic				\
	ilapp

testProfile_SOURCES = testProfile.cc
testProfile_LDADD = $(top_builddir)/qt/libhippoqt.la

testPrinter_SOURCES =testPrinter.cxx
testPrinter_LDADD = $(top_builddir)/qt/libhippoqt.la

teststatic_SOURCES = teststatic.cc
teststatic_LDADD = $(top_builddir)/qt/libhippoqt.la

ilapp_SOURCES = ilapp.cxx
ilapp_LDADD = $(top_builddir)/qt/libhippoqt.la

LDFLAGS =					\
	$(QT_LIBS)				\
	$(ROOTLDFLAGS)				\
	$(CFITSIOLDFLAGS)

testwidget_SOURCES = testwidget.cc
testwidget_LDADD = $(top_builddir)/qt/libhippoqt.la


testwindow_SOURCES =				\
	QtViewWidgetWindow.cpp			\
	moc_QtViewWidgetWindow.cpp		\
	testwindow.cxx 

testwindow_LDADD = $(top_builddir)/qt/libhippoqt.la

DEFS = -DHAVE_CONFIG_H -DQT_THREAD_SUPPORT

INCLUDES =					\
	-I$(top_srcdir)				\
	-I$(top_srcdir)/qt			\
	-I$(top_srcdir)/qt/examples		\
	-I$(top_builddir)			\
	-I$(top_builddir)/qtui			\
	-I$(top_builddir)/qt/examples		\
	$(QT_CXXFLAGS)

SUFFIXES = .cpp .cxx .h .ui

moc_QtViewWidgetWindow.cpp : QtViewWidgetWindow.h
	$(QT_MOC) QtViewWidgetWindow.h -o moc_QtViewWidgetWindow.cpp


if QT4MAKE
.ui.h:
	$(QT_UIC) $< -o $@

.ui.cpp:
	$(QT_UIC) $< -i $*.h -o $@

else
.ui.h: 
	$(QT_UIC) $< > $*.h

.ui.cpp: 
	$(QT_UIC) -impl $*.h $< > $@

.h.moc:
	$(QT_MOC) $(top_srcdir)/qt/$<  -o $@

endif
# For platforms where automake dependency generation doesn't work.
depend: $(libhippoqt_la_SOURCES)
	$(top_srcdir)/config/makedepend $(DEFS) $(INCLUDES) $(CPPFLAGS) $?

# An extra clean target for a Sun CC compiler.
clean-local:
	-rm -rf moc_*.cpp
	-rm -rf *.so
	-rm -rf SunWS_cache
	-rm -rf QtViewWidgetWindow.h QtViewWidgetWindow.cpp

# Do not distribute built sources
dist-hook:
	( cd $(distdir); rm -rf $(BUILT_SOURCES) )

