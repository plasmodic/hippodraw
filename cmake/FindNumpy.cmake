# Copyright Sean C. Warren, from Open EBSD 3D package
#
# Try to find numarray python package
# Once done this will define
#
# PYTHON_NUMPY_FOUND        - system has numpy development package and it should be used
# PYTHON_NUMPY_INCLUDE_DIR  - directory where the libnumarray.h header file can be found
#
#

  IF(PYTHON_EXECUTABLE)
    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/det_npp.py "try: import numpy; print numpy.get_include()\nexcept: pass\n")
    EXEC_PROGRAM("${PYTHON_EXECUTABLE}"
      ARGS "\"${CMAKE_CURRENT_BINARY_DIR}/det_npp.py\""
      OUTPUT_VARIABLE NUMPY_PATH
    )
    FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/det_npp.py)
  ENDIF(PYTHON_EXECUTABLE)

  FIND_PATH(PYTHON_NUMPY_INCLUDE_DIR arrayobject.h
    "${NUMPY_PATH}/numpy/"
    "${PYTHON_INCLUDE_PATH}/numarray/"
    "${PYTHON_INCLUDE_PATH}/Numeric/"
    /usr/include/python2.4/numarray/
    /usr/include/python2.3/numarray/
    /usr/include/python2.2/numarray/
    /usr/include/python2.1/numarray/
    DOC "Directory where the arrayobject.h header file can be found. This file is part of the numpy package"
    )

  IF(PYTHON_NUMPY_INCLUDE_DIR)
    SET (PYTHON_NUMPY_FOUND 1 CACHE INTERNAL "Python numpy development package is available")
  ENDIF(PYTHON_NUMPY_INCLUDE_DIR)
