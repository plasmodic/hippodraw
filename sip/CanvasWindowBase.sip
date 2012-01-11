/* -*- mode:c++ -*- */

/** @file 

SIP interface definition for CanvasWindowBase

Copyright (C)  2002, 2003  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: CanvasWindowBase.sip,v 1.13 2004/03/25 18:04:55 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class CanvasWindowBase : QMainWindow
{
%TypeHeaderCode
#include "CanvasWindowBase.h"
%End


protected:

  CanvasWindowBase ();

public:

  ~CanvasWindowBase();

private:

  virtual void editClear() = 0;
  virtual void editCopy() = 0;
  virtual void editCut() = 0;
  virtual void editPaste() = 0;
  virtual void editUndo() = 0;

  virtual void helpAbout() = 0;

  virtual void fileExportTextTuple() = 0;
  virtual void fileImportTextTuple() = 0;
  virtual void fileNew() = 0;
  virtual void fileOpen() = 0;
  virtual void filePrint() = 0;
  virtual void fileQuit() = 0;
  virtual void fileSave() = 0;
  virtual void fileSaveAll() = 0;
  virtual void fileSaveAs() = 0;
  virtual void fileSaveSelected() = 0;

  virtual void viewAddPage() = 0;
  virtual void viewLock() = 0;
  virtual void viewUnlock() = 0;
  virtual void viewZoomIn() = 0;
  virtual void viewZoomOut() = 0;

};
