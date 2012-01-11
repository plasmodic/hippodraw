set (MINIMIZER_SRCS
  BFGSFitter.cxx
  Fitter.cxx
  FitterFactory.cxx
  LMFitter.cxx
  NTupleChiSqFCN.cxx
  NTuplePearsonFCN.cxx
  NTupleFCN.cxx
  NTupleLikeliHoodFCN.cxx
  NumLinAlg.cxx
  StatedFCN.cxx
)

if (HAVE_MINUIT OR HAVE_MINUIT2)
  set(MINIMIZER_SRCS ${MINIMIZER_SRCS} MinuitMigrad.cxx)
  include_directories( ${MINUIT2_INCLUDE_DIR} )
endif (HAVE_MINUIT OR HAVE_MINUIT2)

add_srcgroup(minimizers ${MINIMIZER_SRCS})

install_files(/include/HippoDraw/minimizers .h)

