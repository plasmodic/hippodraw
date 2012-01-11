include_directories(
   ${CFITSIO_INCLUDE_DIR}
)

add_srcgroup(fits
   FitsController.cxx
   FitsFile.cxx
   FitsFileBase.cxx
   FitsNTuple.cxx)

install_files(/include/HippoDraw/fits .h)
