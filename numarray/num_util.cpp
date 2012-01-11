// Copyright 2006  Phil Austin (http://www.eos.ubc.ca/personal/paustin)
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define PY_ARRAY_UNIQUE_SYMBOL HippoPyArrayHandle
#define NO_IMPORT_ARRAY
#include "num_util.h"

namespace { const char* rcsid = "$Id: num_util.cpp,v 1.12 2007/05/31 21:17:55 pfkeb Exp $"; }

using namespace boost::python;

namespace num_util{

  //specializations for use by makeNum


  template <>
  PyArray_TYPES getEnum<unsigned char>(void)
  {
    return PyArray_UBYTE;
  }


  template <>
  PyArray_TYPES getEnum<signed char>(void)
  {
#ifdef HAVE_NUMPY
    return PyArray_BYTE;
#else
    return PyArray_SBYTE;
#endif
  }

  template <>
  PyArray_TYPES getEnum<short>(void)
  {
    return PyArray_SHORT;
  }

  template <>
  PyArray_TYPES getEnum<unsigned short>(void)
  {
    return PyArray_USHORT;
  }


  template <>
  PyArray_TYPES getEnum<unsigned int>(void)
  {
    return PyArray_UINT;
  }

  template <>
  PyArray_TYPES getEnum<int>(void)
  {
    return PyArray_INT;
  }

  template <>
  PyArray_TYPES getEnum<long>(void)
  {
    return PyArray_LONG;
  }

#ifdef HAVE_NUMPY
  template <>
  PyArray_TYPES getEnum<unsigned long>(void)
  {
    return PyArray_ULONG;
  }

  template <>
  PyArray_TYPES getEnum<long long>(void)
  {
    return PyArray_LONGLONG;
  }

  template <>
  PyArray_TYPES getEnum<unsigned long long>(void)
  {
    return PyArray_ULONGLONG;
  }
#endif

  template <>
  PyArray_TYPES getEnum<float>(void)
  {
    return PyArray_FLOAT;
  }

  template <>
  PyArray_TYPES getEnum<double>(void)
  {
    return PyArray_DOUBLE;
  }

#ifdef HAVE_NUMPY    
  template <>
  PyArray_TYPES getEnum<long double>(void)
  {
    return PyArray_LONGDOUBLE;
  }
#endif
  template <>
  PyArray_TYPES getEnum<std::complex<float> >(void)
  {
    return PyArray_CFLOAT;
  }


  template <>
  PyArray_TYPES getEnum<std::complex<double> >(void)
  {
    return PyArray_CDOUBLE;
  }

#ifdef HAVE_NUMPY
  template <>
  PyArray_TYPES getEnum<std::complex<long double> >(void)
  {
    return PyArray_CLONGDOUBLE;
  }
#endif

  template <> boost::python::numeric::array makeNum<double> (double * data, std::vector<int> dims){
    intp total = std::accumulate(dims.begin(),dims.end(),1,std::multiplies<intp>());
    boost::python::object obj(boost::python::handle<>(PyArray_FromDims(dims.size(),&dims[0], PyArray_DOUBLE)));
#ifdef HAVE_NUMPY
    void *arr_data = PyArray_DATA((PyArrayObject*) obj.ptr());
    memcpy(arr_data, data, PyArray_ITEMSIZE((PyArrayObject*) obj.ptr()) * total);
#else
  char *arr_data = ((PyArrayObject*) obj.ptr())->data; 
  memcpy(arr_data, data, sizeof(double) * total); // copies the input data to 
                                           // PyArrayObject->data
#endif
    return boost::python::extract<boost::python::numeric::array>(obj);
  }


typedef KindStringMap::value_type  KindStringMapEntry;
KindStringMapEntry kindStringMapEntries[] =
  {
    KindStringMapEntry(PyArray_UBYTE,  "PyArray_UBYTE"),
#ifdef HAVE_NUMPY
    KindStringMapEntry(PyArray_BYTE,   "PyArray_BYTE"),
#else
    KindStringMapEntry(PyArray_SBYTE,   "PyArray_SBYTE"),
#endif
    KindStringMapEntry(PyArray_SHORT,  "PyArray_SHORT"),
    KindStringMapEntry(PyArray_INT,    "PyArray_INT"),
    KindStringMapEntry(PyArray_LONG,   "PyArray_LONG"),
    KindStringMapEntry(PyArray_FLOAT,  "PyArray_FLOAT"),
    KindStringMapEntry(PyArray_DOUBLE, "PyArray_DOUBLE"),
    KindStringMapEntry(PyArray_CFLOAT, "PyArray_CFLOAT"),
    KindStringMapEntry(PyArray_CDOUBLE,"PyArray_CDOUBLE"),
    KindStringMapEntry(PyArray_OBJECT, "PyArray_OBJECT"),
    KindStringMapEntry(PyArray_NTYPES, "PyArray_NTYPES"),
    KindStringMapEntry(PyArray_NOTYPE ,"PyArray_NOTYPE")
  };

typedef KindCharMap::value_type  KindCharMapEntry;
KindCharMapEntry kindCharMapEntries[] =
  {
    KindCharMapEntry(PyArray_UBYTE,  'B'),
#ifdef HAVE_NUMPY
    KindCharMapEntry(PyArray_BYTE,   'b'),
#else
    KindCharMapEntry(PyArray_SBYTE,   'b'),
#endif
    KindCharMapEntry(PyArray_SHORT,  'h'),
    KindCharMapEntry(PyArray_INT,    'i'),
    KindCharMapEntry(PyArray_LONG,   'l'),
    KindCharMapEntry(PyArray_FLOAT,  'f'),
    KindCharMapEntry(PyArray_DOUBLE, 'd'),
    KindCharMapEntry(PyArray_CFLOAT, 'F'),
    KindCharMapEntry(PyArray_CDOUBLE,'D'),
    KindCharMapEntry(PyArray_OBJECT, 'O')
  };
  
typedef KindTypeMap::value_type  KindTypeMapEntry;
KindTypeMapEntry kindTypeMapEntries[] =
  {
    KindTypeMapEntry('B',PyArray_UBYTE),
#ifdef HAVE_NUMPY
    KindTypeMapEntry('b',PyArray_BYTE),
#else
    KindTypeMapEntry('b',PyArray_SBYTE),
#endif
    KindTypeMapEntry('h',PyArray_SHORT),
    KindTypeMapEntry('i',PyArray_INT),
    KindTypeMapEntry('l',PyArray_LONG),
    KindTypeMapEntry('f',PyArray_FLOAT),
    KindTypeMapEntry('d',PyArray_DOUBLE),
    KindTypeMapEntry('F',PyArray_CFLOAT),
    KindTypeMapEntry('D',PyArray_CDOUBLE),
    KindTypeMapEntry('O',PyArray_OBJECT)
  };

  
int numStringEntries = sizeof(kindStringMapEntries)/sizeof(KindStringMapEntry);
int numCharEntries = sizeof(kindCharMapEntries)/sizeof(KindCharMapEntry);
int numTypeEntries = sizeof(kindTypeMapEntries)/sizeof(KindTypeMapEntry);


using namespace boost::python;
  
static KindStringMap kindstrings(kindStringMapEntries,
                                   kindStringMapEntries + numStringEntries);

static KindCharMap kindchars(kindCharMapEntries,
                                   kindCharMapEntries + numCharEntries);

static KindTypeMap kindtypes(kindTypeMapEntries,
                                   kindTypeMapEntries + numTypeEntries);

//Create a numarray referencing Python sequence object
numeric::array makeNum(object x){
  if (!PySequence_Check(x.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a sequence");
    throw_error_already_set();
  }
  object obj(handle<>
	     (PyArray_ContiguousFromObject(x.ptr(),PyArray_NOTYPE,0,0)));
  check_PyArrayElementType(obj);
  return extract<numeric::array>(obj); 
}

//Create a one-dimensional Numeric array of length n and Numeric type t
numeric::array makeNum(int n, PyArray_TYPES t=PyArray_DOUBLE){
  object obj(handle<>(PyArray_FromDims(1, &n, t)));
  return extract<numeric::array>(obj);
}
  
//Create a Numeric array with dimensions dimens and Numeric type t
numeric::array makeNum(std::vector<int> dimens, 
		       PyArray_TYPES t=PyArray_DOUBLE){
  object obj(handle<>(PyArray_FromDims(dimens.size(), &dimens[0], t)));
  return extract<numeric::array>(obj);
}

numeric::array makeNum(const numeric::array& arr){
  //Returns a reference of arr by calling numeric::array copy constructor.
  //The copy constructor increases arr's reference count.
  return numeric::array(arr);
} 

PyArray_TYPES type(numeric::array arr){
#ifdef HAVE_NUMPY
  return PyArray_TYPES(PyArray_TYPE(arr.ptr()));
#else
  return char2type(arr.typecode());
#endif
}

void check_type(boost::python::numeric::array arr, 
		PyArray_TYPES expected_type){
  PyArray_TYPES actual_type = type(arr);
  if (actual_type != expected_type) {
    std::ostringstream stream;
    stream << "expected Numeric type " << kindstrings[expected_type]
	   << ", found Numeric type " << kindstrings[actual_type] << std::ends;
    PyErr_SetString(PyExc_TypeError, stream.str().c_str());
    throw_error_already_set();
  }
  return;
}

//Return the number of dimensions
int rank(numeric::array arr){
  //std::cout << "inside rank" << std::endl;
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
#ifdef HAVE_NUMPY
  return PyArray_NDIM(arr.ptr());
#else
  return ((PyArrayObject*) arr.ptr())->nd;
#endif
}

void check_rank(boost::python::numeric::array arr, int expected_rank){
  int actual_rank = rank(arr);
  if (actual_rank != expected_rank) {
    std::ostringstream stream;
    stream << "expected rank " << expected_rank 
	   << ", found rank " << actual_rank << std::ends;
    PyErr_SetString(PyExc_RuntimeError, stream.str().c_str());
    throw_error_already_set();
  }
  return;
}

intp size(numeric::array arr)
{
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
  return PyArray_Size(arr.ptr());
}

void check_size(boost::python::numeric::array arr, intp expected_size){
  intp actual_size = size(arr);
  if (actual_size != expected_size) {
    std::ostringstream stream;
    stream << "expected size " << expected_size 
	   << ", found size " << actual_size << std::ends;
    PyErr_SetString(PyExc_RuntimeError, stream.str().c_str());
    throw_error_already_set();
  }
  return;
}

std::vector<intptr_t> shape(numeric::array arr){
  std::vector<intptr_t> out_dims;
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
#ifdef HAVE_NUMPY
  intp* dims_ptr = PyArray_DIMS(arr.ptr());
#else
  int* dims_ptr = ((PyArrayObject*) arr.ptr())->dimensions;
#endif
  int the_rank = rank(arr);
  for (int i = 0; i < the_rank; i++){
    out_dims.push_back(*(dims_ptr + i));
  }
  return out_dims;
}

intp get_dim(boost::python::numeric::array arr, int dimnum){
  assert(dimnum >= 0);
  int the_rank=rank(arr);
  if(the_rank < dimnum){
    std::ostringstream stream;
    stream << "Error: asked for length of dimension ";
    stream << dimnum << " but rank of array is " << the_rank << std::ends;
    PyErr_SetString(PyExc_RuntimeError, stream.str().c_str());       
    throw_error_already_set();
  }
  std::vector<intptr_t> actual_dims = shape(arr);
  return actual_dims[dimnum];
}

void check_shape(boost::python::numeric::array arr, std::vector<intptr_t> expected_dims){
  std::vector<intptr_t> actual_dims = shape(arr);
  if (actual_dims != expected_dims) {
    std::ostringstream stream;
    stream << "expected dimensions " << vector_str(expected_dims)
	   << ", found dimensions " << vector_str(actual_dims) << std::ends;
    PyErr_SetString(PyExc_RuntimeError, stream.str().c_str());
    throw_error_already_set();
  }
  return;
}

void check_dim(boost::python::numeric::array arr, int dimnum, intp dimsize){
  std::vector<intptr_t> actual_dims = shape(arr);
  if(actual_dims[dimnum] != dimsize){
    std::ostringstream stream;
    stream << "Error: expected dimension number ";
    stream << dimnum << " to be length " << dimsize;
    stream << ", but found length " << actual_dims[dimnum]  << std::ends;
    PyErr_SetString(PyExc_RuntimeError, stream.str().c_str());       
    throw_error_already_set();
  }
  return;
}

bool iscontiguous(numeric::array arr)
{
  //return arr.iscontiguous();
  return (((PyArrayObject*) arr.ptr())->flags & C_CONTIGUOUS);
}

void check_contiguous(numeric::array arr)
{
  if (!iscontiguous(arr)) {
    PyErr_SetString(PyExc_RuntimeError, "expected a contiguous array");
    throw_error_already_set();
  }
  return;
}

void* data(numeric::array arr){
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
#ifdef HAVE_NUMPY
  return PyArray_DATA(arr.ptr());
#else
  return ((PyArrayObject*) arr.ptr())->data;
#endif
}

//Copy data into the array
void copy_data(boost::python::numeric::array arr, char* new_data){
  char* arr_data = (char*) data(arr);
  intp nbytes = PyArray_NBYTES((PyArrayObject *)arr.ptr());
  for (intp i = 0; i < nbytes; i++) {
    arr_data[i] = new_data[i];
  }
  return;
} 

//Return a clone of this array
numeric::array clone(numeric::array arr){
#ifdef HAVE_NUMPY
  object obj(handle<>(PyArray_NewCopy((PyArrayObject*)arr.ptr(),PyArray_CORDER)));
#else
  object obj(handle<>(PyArray_Copy((PyArrayObject*)arr.ptr())));
#endif
  return makeNum(obj);
}

  
//Return a clone of this array with a new type
numeric::array astype(boost::python::numeric::array arr, PyArray_TYPES t){
  return (numeric::array) arr.astype(type2char(t));
}

std::vector<intp> strides(numeric::array arr){
  std::vector<intp> out_strides;
  if(!PyArray_Check(arr.ptr())){
    PyErr_SetString(PyExc_ValueError, "expected a PyArrayObject");
    throw_error_already_set();
  }
#ifdef HAVE_NUMPY
  intp* strides_ptr = PyArray_STRIDES(arr.ptr());
#else
  int* strides_ptr = ((PyArrayObject*) arr.ptr())->strides;
#endif
  intp the_rank = rank(arr);
  for (intp i = 0; i < the_rank; i++){
    out_strides.push_back(*(strides_ptr + i));
  }
  return out_strides;
}

int refcount(numeric::array arr){
  return REFCOUNT(arr.ptr());
}

void check_PyArrayElementType(object newo){
#ifdef HAVE_NUMPY
  PyArray_TYPES theType=PyArray_TYPES(PyArray_TYPE(newo.ptr()));
#else
  PyArrayObject* PyArrayPtr = (PyArrayObject*) newo.ptr();
  PyArray_TYPES theType=(PyArray_TYPES) PyArrayPtr->descr->type_num;
#endif
  if(theType == PyArray_OBJECT){
      std::ostringstream stream;
      stream << "array elments have been cast to PyArray_OBJECT, "
             << "numhandle can only accept arrays with numerical elements" 
	     << std::ends;
      PyErr_SetString(PyExc_TypeError, stream.str().c_str());
      throw_error_already_set();
  }
  return;
}

std::string type2string(PyArray_TYPES t_type){
  return kindstrings[t_type];
}

char type2char(PyArray_TYPES t_type){
  return kindchars[t_type];
}

PyArray_TYPES char2type(char e_type){
  return kindtypes[e_type];
}

template <class T>
inline std::string vector_str(const std::vector<T>& vec)
{
  std::ostringstream stream;
  stream << "(" << vec[0];

  for(std::size_t i = 1; i < vec.size(); i++){
    stream << ", " << vec[i];
  }
  stream << ")";
  return stream.str();
}

inline void check_size_match(std::vector<intp> dims, intp n)
{
  intp total = std::accumulate(dims.begin(),dims.end(),1,std::multiplies<intp>());
  if (total != n) {
    std::ostringstream stream;
    stream << "expected array size " << n
           << ", dimensions give array size " << total << std::ends;
    PyErr_SetString(PyExc_TypeError, stream.str().c_str());
    throw_error_already_set();
  }
  return;
}

} //namespace num_util

