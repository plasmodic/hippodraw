
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})

if (HAVE_WCSLIB)
  include_directories(${WCS_INCLUDE_DIR})
endif (HAVE_WCSLIB)

set(MOC_HDRS
    AxisWidget.h
    CanvasView.h
    CanvasViewProxy.h
    CanvasWindow.h
    CanvasSettings.h
    CreateNTuple.h
    FigureEditor.h
    Inspector.h
    PickTable.h
    PlotTable.h
)
    
qt4_wrap_cpp(MOC_SRCS ${MOC_HDRS})

add_srcgroup(hippoqt
  QtViewImp.cxx
  QtView.cxx			
  QtViewWidget.cxx		
  QtGroupView.cxx			
  FigureEditor.cxx		
  AxisWidget.cxx 
  CanvasView.cxx 
  CanvasViewProxy.cxx
  ${MOC_SRCS}
  CanvasSelectionEvent.cxx	
  CanvasSettings.cxx		
  CanvasWindow.cxx		
  DrawBorder.cxx			
  AxisWidgetPlugin.cxx		
  QtFileDialog.cxx		
  HdThread.cxx			
  Inspector.cxx			
  ListDialog.cxx			
  PlotTableEvent.cxx		
  PlotterEvent.cxx		
  PlotTable.cxx			
  PickTable.cxx			
  CreateNTuple.cxx		
  QtApp.cxx			
  QtFont.cxx			
  QtViewFactory.cxx		
  SaveAsImageEvent.cxx		
  WindowController.cxx			
  util.cxx)

add_dependencies(hippoqt CanvasWindowBase.h)

install_files(/include/HippoDraw/qt .h)

add_subdirectory(app)

