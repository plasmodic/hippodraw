# This script is for developers to use to set the Python path to the
# development version of the HippoDraw Python extension module instead
# of taking it from the installed version.  Developers should source
# this script before attempting to test any of the Python scripts in
# this directory.  The two variables, SOURCE_DIR and BUILD_DIR, may
# need to be modified before using this script.

# Author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>


# Set this variable to the directory where HippoDraw sources are
# located.
setenv SOURCE_DIR ../..//hippodraw

# Set this variable to the directory where HippoDraw was built
setenv BUILD_DIR ../../hippodraw-BUILD

# Python will use this variable to set these paths ahead of the
# default paths

setenv PYTHONPATH $BUILD_DIR/python/.libs:$SOURCE_DIR/python
