## Process this file with automake to produce Makefile.in .
#
#  Copyright (C)  2003, 2005, 2006  The Board of Trustees of  
#  The Leland Stanford Junior University.  All Rights Reserved.
#
# $Id: Makefile.am,v 1.42 2007/09/20 21:20:17 pfkeb Exp $
#
# Author: Paul_Kunz@slac.stanford.edu
#

# The following is set, otherwise it would follow GNU conventions.
AUTOMAKE_OPTIONS = foreign

examplesdir = $(pkgdatadir)/examples

examples_DATA = SimEvents.fits Z0Strip.data aptuple.tnt			\
	append_ntuple.py cuts_complex.py cut_multi_displays.py		\
	cuts_one_display.py datareps.py displays.py fft.py fitting.py	\
	fitting2.py fitsWriting.py function_ntuple.py load_hippo.py	\
	loglog.py mainpage.py merit.py run_examples.py			\
	simple_xyplot.py static_vs_dynamic.py svac_array.py

EXTRA_DIST =					\
	$(examples_DATA)			\
	EGRET_AllSky.fits			\
	EGRET_AllSky_car.fits			\
	README					\
	Z0Strip.baddata				\
	Z0Strip.baddata2			\
	Z0Strip.data				\
	aptuple.tnt				\
	earth.fits				\
	fluxes.tnt				\
	m51.fits				\
	output.txt				\
	set_hippo.csh				\
	testErfc.tnt				\
	transducer.tnt

