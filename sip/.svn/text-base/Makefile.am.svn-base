## Process this file with automake to produce Makefile.in
#
# Copyright (C) 2002-2006   The Board of Trustees of The Leland Stanford
# Junior University.  All Rights Reserved.
#
# $Id: Makefile.am,v 1.89 2007/02/24 23:29:00 pfkeb Exp $
#
# Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
#

# The following is set; otherwise, it would follow GNU conventions.
AUTOMAKE_OPTIONS = foreign

QT_DIR = @QT_DIR@
QT_INCLUDE = $(QT_DIR)/include

SIP = sip

PYQT_SRCS = -I /usr/local/share/sip -I /usr/local/share/sip/qtcanvas \
            -I /usr/share/sip -I /usr/share/sip/qtcanvas

## Subsitution from pfkeb_python_include
PY_INCLUDE = @PY_INCLUDE@
PY_LIB = @PY_LIB@

## Subsitution from pfkeb_cxx_lib_path
CXX_LIB_PATH = @CXX_LIB_PATH@

MODULE = sihippo

BUILT_SOURCES =					\
	sipsihippocmodule.cpp			\
	sipsihippohippodraw.cpp			\
	sipsihippoAxisModelBase.cpp		\
	sipsihippoCanvasWindow.cpp		\
	sipsihippoCircularBuffer.cpp		\
	sipsihippoCanvasWindowBase.cpp		\
	sipsihippoColor.cpp			\
	sipsihippoCutController.cpp		\
	sipsihippoCutPlotter.cpp		\
	sipsihippoCut1DPlotter.cpp		\
	sipsihippoDataRep.cpp			\
	sipsihippoDataRepException.cpp		\
	sipsihippoDataSource.cpp		\
	sipsihippoDisplayController.cpp		\
	sipsihippoFactoryException.cpp		\
	sipsihippoFitter.cpp			\
	sipsihippoFunctionBase.cpp		\
	sipsihippoFunctionController.cpp	\
	sipsihippoFunctionFactory.cpp		\
	sipsihippoFunctionHelper.cpp		\
	sipsihippoInspectorBase.cpp		\
	sipsihippoInspector.cpp			\
	sipsihippoLineDataRep.cpp		\
	sipsihippoNTuple.cpp			\
	sipsihippoNTupleController.cpp		\
	sipsihippoObservable.cpp		\
	sipsihippoObserver.cpp			\
	sipsihippoPlotterBase.cpp		\
	sipsihippoPlotterEvent.cpp		\
	sipsihippoQtView.cpp			\
	sipsihippohippodrawViewBase.cpp		\
	sipsihippoViewBase.cpp			\
	sipsihippoQtViewImp.cpp			\
	sipsihippoQtViewWidget.cpp		\
	sipsihippoRepBase.cpp			\
	sipsihippohippodrawRepBase.cpp		\
	sipsihippoSaveAsImageEvent.cpp		\
	sipsihippoTextRepFactory.cpp		\
	sipsihippoXyPlotter.cpp			\
	sipsihippoWindowController.cpp		\
	sipsihippostdstring.cpp			\
	sipsihippostdvector.cpp

lib_LTLIBRARIES = sihippo.la

nodist_sihippo_la_SOURCES =			\
	$(BUILT_SOURCES)

sihippo_la_LIBADD =				\
	$(top_builddir)/lib/libhippo.la		\
	$(top_builddir)/qt/libhippoqt.la

sihippo_la_LDFLAGS =				\
	-module					\
	$(QT_LIBS)				\
	-R $(QT_DIR)/lib			\
	-R $(CXX_LIB_PATH)

SIP_SRCS =					\
	AxisModelBase.sip			\
	CanvasWindow.sip			\
	CanvasWindowBase.sip			\
	CircularBuffer.sip			\
	Color.sip				\
	CutController.sip			\
	CutPlotter.sip				\
	Cut1DPlotter.sip			\
	DataRep.sip				\
	DataRepException.sip			\
	DataSource.sip				\
	DisplayController.sip			\
	FactoryException.sip			\
	Fitter.sip				\
	FunctionBase.sip			\
	FunctionController.sip			\
	FunctionFactory.sip			\
	FunctionHelper.sip			\
	Inspector.sip				\
	InspectorBase.sip			\
	LineDataRep.sip				\
	NTuple.sip				\
	NTupleController.sip			\
	Observable.sip				\
	Observer.sip				\
	PlotterBase.sip				\
	PlotterEvent.sip			\
	QtView.sip				\
	QtViewImp.sip				\
	QtViewWidget.sip			\
	RepBase.sip				\
	SaveAsImageEvent.sip			\
	TextRepFactory.sip			\
	ViewBase.sip				\
	XyPlotter.sip				\
	WindowController.sip			\
	string.sip				\
	vector.sip				\
	sihippo.sip

EXTRA_DIST = sip.dir $(SIP_SRCS) INSTALL

CXXFLAGS = -g

python_DATA =					\
	sihippo.so 	

DEFS = -DQT_THREAD_SUPPORT

INCLUDES =					\
	-I$(top_srcdir)				\
	-I$(top_srcdir)/sip			\
	-I$(top_builddir)/qt			\
	-I$(top_builddir)/qtui			\
	-I$(top_builddir)/sip			\
	-I$(PY_INCLUDE)				\
	-I$(QT_INCLUDE)

SUFFIXES = .cpp .cxx .h .sip

all-local: sihippo.so

sihippo.so: $(lib_LTLIBRARIES)
	@echo creating sihippo.so
	cp .libs/sihippo.so.0.0.0 sihippo.so

sipsihippocmodule.cpp  .stamp-sip : $(SIP_SRCS) 
	@echo creating built sources
	$(SIP) -e -c . $(PYQT_SRCS) -I $(top_srcdir)/sip  \
	-t Qt_3_3_6 -t WS_X11 $(top_srcdir)/sip/sihippo.sip
	for f in sipsihippohippodraw*.h ; do \
	sed -e 's,^class hippodraw::\(.*\)$$,namespace hippodraw { class \1 };,' -i $$f ;\
	done
	@touch .stamp-sip

$(BUILT_SOURCES): .stamp-sip

# sometimes this creates a .pyc file and sometimes it doesn't.
%.cpp: %.sip
	$(SIP) -c . -I $(top_srcdir)/sip $(PYQT_SRCS) \
	$(top_srcdir)/sip/sihippo.sip

# For platforms where automake dependency generation doesn't work.
depend: $(libsihippo_la_SOURCES)
	$(top_srcdir)/config/makedepend $(DEFS) $(INCLUDES) $(CPPFLAGS) $?

clean-local:
	if test -d html; then \
	(cd html; rm -rf *.html *.gif *.css *.dot *.png formula.repository; \
	rm -rf *.md5 *.map ) \
	fi
	-rm -rf *.bmp
	-rm -rf *.cpp
	-rm -rf sip*.h	
	-rm -rf *.so
	-rm -rf SunWS_cache
	-rm -rf .stamp-sip

docs:
	if test -d html; then \
	(cd html; rm -rf *.html ) \
	fi
	doxygen Doxyfile
