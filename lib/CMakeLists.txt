
attach_srcgroups(LIBHIPPO_SRCS
  axes
  binners
  colorreps
  controllers
  datareps
  datasrcs
  functions
  graphics
  minimizers
  hippoui
  pattern
  plotters
  projectors
  reps
  transforms
  xml
  qtxml
  hippoqt
  ${EXTRA_MODULES}
)


add_library(hippo SHARED
  version.cxx
  ${LIBHIPPO_SRCS}
)

add_dependencies(hippo hippoui_headers qt hippoui)

target_link_libraries(hippo
  ${EXTRA_MODULES_LIBS}
  ${QT_LIBRARIES}
  )

install_targets(/lib hippo)

