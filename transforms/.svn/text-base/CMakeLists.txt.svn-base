## subsitution from pfkeb_have_wcslib
#WCSLIB_CXXFLAGS = @WCSLIB_CXXFLAGS@

if (HAVE_WCSLIB)
  include_directories(${WCS_INCLUDE_DIR})

  set(WCS_TRANSFORMS
    WCSTransform.cxx
    HammerAito.cxx
    Lambert.cxx
    Mercator.cxx
    Cartesian.cxx
    GlobalSinusoidal.cxx
    ARC.cxx
    TAN.cxx
    SIN.cxx
    STG.cxx
    AIR.cxx
  )
endif (HAVE_WCSLIB)

add_srcgroup(transforms
  BinaryTransform.cxx
  PeriodicBinaryTransform.cxx
  LinearTransform.cxx
  LogTransform.cxx
  TransformBase.cxx
  TransformFactory.cxx
  UnaryTransform.cxx
  XYTransform.cxx
  XYZTransform.cxx
  XYZTransformBase.cxx
 
  ${WCS_TRANSFORMS}
)

install_files(/include/HippoDraw/transforms .h)

