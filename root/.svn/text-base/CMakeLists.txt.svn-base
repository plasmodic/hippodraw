include_directories(
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_PATH}
  ${PYTHON_NUMPY_INCLUDE_DIR}/..
  ${PYTHON_NUMPY_INCLUDE_DIR}/../../../numarray
  ${CMAKE_BINARY_DIR}
  ${ROOT_INCLUDE_DIR}
)

add_srcgroup(pyroot
   QtRootController.cxx
   QtRootNTuple.cxx
)

add_srcgroup(root
   RootBranch.cxx
   RootController.cxx
   RootNTuple.cxx
)

install_files(/include/HippoDraw/root .h)

