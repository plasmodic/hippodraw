## Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.5 2005/01/09 16:29:42 pfkeb Exp $
#
# Author: <Paul_Kunz@slac.stanford.edu>
#

# The following is set; otherwise, it would follow GNU conventions.
AUTOMAKE_OPTIONS = foreign

## Subsitution from pfkeb_cxx_lib_path
CXX_LIB_PATH = @CXX_LIB_PATH@

EXTRA_DIST = test.py.in

check_PROGRAMS = hippodraw

hippodraw_SOURCES = hippoTestOpenGL.cxx

hippodraw_LDADD =				\
	$(top_builddir)/OpenGL/libhippoGL.la

hippodraw_LDFLAGS =			\
	-R $(libdir)				\
	-R $(CXX_LIB_PATH)

INCLUDES = -I$(top_srcdir)


# For platforms where automake dependency generation doesn't work.
depend: $(hippodraw_SOURCES)
	$(top_srcdir)/config/makedepend $(DEFS) $(INCLUDES) $(CPPFLAGS) $?

# An extra clean target for a Sun CC compiler.
clean-local:
	-rm -rf SunWS_cache
	-rm -rf hippodraw
