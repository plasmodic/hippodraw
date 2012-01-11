## Process this file with automake to produce Makefile.in .
#
# Copyright (C) 2002, 2003, 2006  The Board of Trustees of The Leland
# Stanford Junior University.  All Rights Reserved.
#
# $Id: Makefile.am,v 1.69 2006/03/04 17:38:02 xiefang Exp $
#
# Author: Paul_Kunz@slac.stanford.edu
#

# The following is set; otherwise, it would follow GNU conventions.
# AUTOMAKE_OPTIONS = foreign

## Subsitution from pfkeb_cxx_lib_path
CXX_LIB_PATH = @CXX_LIB_PATH@

check_PROGRAMS =				\
	testXAxisModelLinear

testXAxisModelLinear_SOURCES = testXAxisModelLinear.cxx

allSOURCES =					\
	$(testBins2D_SOURCES)			\
	$(testXAxisModelLinear_SOURCES)


INCLUDES = -I$(top_srcdir) 

TESTS = $(noinst_PROGRAMS)


# For platforms where automake dependency generation doesn't work.
depend: $(allSOURCES)
	$(top_srcdir)/config/makedepend $(DEFS) $(INCLUDES) $(CPPFLAGS) $?

clean-local:
	-rm -rf *.C

EXTRA_DIST =					\
	ConstantModelFit.py.in			\
	accesstuple.py.in			\
	autoscale.py.in				\
	axisBug.py.in				\
	canvas.py.in				\
	circbuff.py.in				\
	cuts.py.in				\
	cuts_export.py.in			\
	datareps.py.in				\
	displays.py.in				\
	documents.py.in				\
	egret.py.in				\
	empty.py.in				\
	function.py.in				\
	function_ntuple.py.in			\
	hammer.py.in				\
	 hist_bug.py.in                         \
	hist_ntuple.py.in			\
	histexceptions.py.in			\
	histogram.py.in				\
	hsimple.py				\
	lambert.py.in				\
	log.py.in				\
	logAxisBug.py.in			\
	m51.py.in				\
	manipulateDisplays.py.in		\
	ntuple.py.in				\
	overplot.py.in				\
	pl_exp_test.py.in			\
	ranges.py.in				\
	rebin.py.in				\
	setPath.py.in				\
	test_FitsImage.py.in			\
	test_addText.py.in			\
	test_getNTuple.py.in			\
	test_numarray.py.in			\
	useBinner.py.in				\
	weighted_pl.py.in			\
	zplot_pixel_offset.py.in

# Copy python test script to msvc directory when needed.
MSPYSCRIPTS =					\
	accesstuple.py				\
	canvas.py				\
	changerep.py				\
	circularbuffer.py			\
	cuts.py					\
	cuts_export.py				\
	datareps.py				\
	displays.py				\
	documents.py				\
	empty.py				\
	exponential.py				\
	function.py				\
	hammer.py				\
	histexceptions.py			\
	histogram.py				\
	log.py					\
	ntuple.py				\
	ranges.py				\
	replacetuple.py				\
	rootpdr.py

MSTESTDIR = $(top_srcdir)/msvc

msscripts :
	( for s in $(MSPYSCRIPTS); do \
	cp $$s $(MSTESTDIR)/. ; \
	done; )

