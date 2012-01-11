add_definitions(-DQT_THREAD_SUPPORT -DQT_COMPAT)
add_srcgroup(qtxml
  QtXMLController.cxx
  QtXmlDocument.cxx
  QtXmlElement.cxx
  QtXmlNode.cxx
  QtXmlTextNode.cxx)

install_files(/include/HippoDraw/qtxml .h)

