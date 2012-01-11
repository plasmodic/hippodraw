include_directories(${CMAKE_SOURCE_DIR}/qt ${QT_INCLUDE_DIR})

set(UI_SRCS
  CanvasSettingsBase.ui
  CanvasWindowBase.ui
  InspectorBase.ui
  ListDialogBase.ui
  PickTableBase.ui
  PlotTableBase.ui
  CreateNTupleBase.ui
)

set(QT_RSRCS
  CanvasWindowBase.qrc
  HippoIcons.qrc
)

qt4_wrap_ui(UI_HDRS ${UI_SRCS})
qt4_add_resources(SRCS ${QT_RSRCS})
 
add_custom_target(hippoui_headers ALL DEPENDS ${UI_HDRS})

add_srcgroup(hippoui
  ${SRCS}
  )

install_files(/include/HippoDraw/qtui FILES ${UI_HDRS})

