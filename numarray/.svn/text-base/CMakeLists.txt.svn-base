#include_directories($ENV{I3_SRC}/boost/public 
#  $ENV{I3_PORTS}/include/boost-1.33.1 

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_PATH}
  ${PYTHON_NUMPY_INCLUDE_DIR}/..
  ${PYTHON_NUMPY_INCLUDE_DIR}/../../../numarray)

add_srcgroup(numarray
  NumArrayTuple.cxx
  export_NumArrayTuple.cxx
  num_util.cpp)

install_files(/include/HippoDraw/numarray .h)

# INCLUDES =					\
# 	-I$(top_srcdir)				\
# 	-I$(top_srcdir)/python			\
# 	$(BOOST_CXXFLAGS)			\
# 	-I$(NUMARRAY_INCLUDE)			\
# 	-I$(PY_INCLUDE)

