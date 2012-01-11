/* -*- mode: c++ -*- */

/** @file 

SIP interface definition for CanvasWindow

Copyright (C)  2002, 2003, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: CanvasWindow.sip,v 1.45 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class CanvasWindow : QMainWindow
{
%TypeHeaderCode
#include "qt/CanvasWindow.h"
using hippodraw::CanvasWindow;
%End


// private to hide from Doxygen
private:

 /** Informs the application object that is window is about to close. */
  virtual void closeEvent ( QCloseEvent * );
/** Informs the Canvas to resize as well. */
  virtual void resizeEvent ( QResizeEvent * );
 /** Informs the application object that the window has been shown. */
  virtual void showEvent ( QShowEvent * );
   /** Informs the application object if this window has become the
      active one. 

      @attention The implementation of this function informs the
      singleton instance of the application that this window has
      become the active one.  Therefore, the implementation of the
      constructor must not cause this method to be called before the
      application object is full constructed.
  */
  virtual void windowActivationChange ( bool );




   /** Responds to "Clear" menu item.  Prompts the user with a warning
      dialog, then removes all canvas items or not depending on user
      response.  @sa clear()
  */
  virtual void editClear();
  virtual void editCopy();
 /** Removes selected items from canvas and add them to the
      pasteboard.
  */
  virtual void editCut();
  virtual void editPaste();
  /** Responds to the undo action. 
   */
  virtual void editUndo();
  /** Removes selected items from canvas without copying them to the
      pasteboard.
  */
  virtual void editDelete ();

  /** Re tiles the entire document.
   */
  virtual void editReTile ();
   /** Re tiles only the current page.
   */
  virtual void editReTilePage ();
  /** Sets all QCanvasItem objects to selected. 
   */
  virtual void editSelectAll ();




  /** Brings up standard window showing version, authors, etc. 
   */
  virtual void helpAbout();


  virtual void fileExportTextTuple();
  virtual void fileImportTextTuple();
 /* Implementation of functions declared in base class. */

  /** Prints the canvas to a Postscript file.  

      @todo Add the `.ps' subscript to the file name if missing.
  */
  virtual void filePrint();
  virtual void fileQuit();

 /** Saves the canvas document and any NTuple that were not read from
    or written to a file. The NTuple objects are saved in the same
    directory as the canvas document.  Their name becomes just the
    file name with index number with the suffix ".tnt".  The path is
    striped from the name.
  */
  virtual void fileSaveAll();

  /** Saves the canvas document and its NTuple.  This function is
      similar to fileSaveAll(), but it always prompts the user for a
      filename. 
  */
 virtual void fileSaveAllAs();
 



  /** Brings up Qt's about message box.
   */
  virtual void aboutQt();

  virtual void viewLock();

   /** Unlocks a canvas item.  Unlocks a canvas item so it can be moved
      and resized. 
  */
  virtual void viewUnlock();
 /** Enlarges the view of the canvas. */
  virtual void viewZoomIn();
  /** Reduces the view of the canvas. */
  virtual void viewZoomOut();
 /** Returns the view of the canvas to normal. 
   */
  virtual void viewZoomReset();

  /** Adds a page to the canvas */
  virtual  void viewAddPage();

 /** Shows the Inspector window.
   */
  virtual void viewShowInspector ();

  /** Shows the plot table. 
   */
  virtual void viewShowPlotTable ();

  /** Shows the pick table window.
   */
  virtual void viewShowPickTable ();





 /** Sets the canvas settings. 
   */
  virtual void settingCanvas ();

  /** Sets the printer settings. 
   */
  virtual void settingPrinter ();

  /** Sets the font settings.
   */
  virtual void settingFonts ();


protected:
  /** Informs the application object that the window has been hidden. */
  virtual void hideEvent ( QHideEvent * );

 protected slots:

  /** Responds to the Quit item in the File menu.
   */
 virtual void fileExit();

  /** Opens an existing file.  Opens an existing document or data file
      after prompting the user for the document's file name and/or
      type.
  */
  virtual void fileOpen();


public:

//   CanvasWindow ();
  CanvasWindow ( QWidget * /TransferThis/ =0,
		 const char * = 0, unsigned =WType_TopLevel );

   /** A copy constructor that does nothing except keep boost.python
      happy.  Without it, boost.python interface wouldn't compile
      because QMainWindow copy constructor is private. */
  CanvasWindow ( const CanvasWindow & );

  virtual ~CanvasWindow();

  /** Initializes this object from contents of file. */
  void initFromFile ( const QString &  );

   /** If @a yes is @c true, sets a flag so that this object will not
      accept close events. */
  void inhibitClose ( bool = true );


//   bool closeNoPrompt();
  /** Sets the window to allowed to be closed by user without
      prompting.
  */
  void setAllowClose ( bool = true );

 /** If document has changed and not saved, opens a dialog box for
      user to choose to save, discard or cancel.  Otherwise, return
      true.  Intended to be used for closeEvent. */
  bool allowClose ();

  /** Closes the window.  If @a no_prompt is @c true, the user will
      not be prompted, otherwise the user gets a dialog with which he
      can cancel the close operation.
  */
  virtual bool closeNoPrompt ();

 /** Sets the changed flag.  If the changed status has already set to
      @c true and @a flag is @c true, this member function does
      nothing.  Otherwise, updates the window's title bar to reflect
      the document's current status.
 */
  void setChanged ( bool = true );

  /** Sets the Windows title.  The windows title depends on an number
      of attributes.  They are collected to set the title. */
  void setCaption ();

  /** Update Action objects.  Sets enable status of QAction objects
      such as menu items and tool bar icons. 
  */
  void updateActions ();

  /** Creates a new document CanvasWindow. */
  virtual void fileNew();

  /** Saves the document.  If the document already corresponds to a
      saved document, prompts the user with a warning dialog before
      over writing the file.  If no file name has been assigned, then
      invokes the fileSaveAs() member function.
  */
  void fileSave ();

  /** Saves the document after display a file dialog to the user. */
  virtual void fileSaveAs ();

  /** Saves the selected ViewBase object as an image file. 
   */
  virtual void fileSaveSelectedImages ();

   /** Saves the selected ViewBase objects as an image file with
       the given filename.
   */
   virtual void fileSaveSelectedImages( const std::string & );

//    /** Returns the CanvasView object.
//    */
//   CanvasView * getCanvasView ();

  /** Adds a new plot to the canvas and selects it. */
  void addDisplay ( PlotterBase * );
  /** Adds function textual display to the canvas. */
  void addFuncDisplay ( PlotterBase *, const std::string & )
    throw ( FactoryException );

   /** Adds a new plot display to the canvas.  Adds a new plot display
      to the canvas and puts it in selected state if @a select is @c
      true.
  */
  void addPlotDisplay ( PlotterBase *, bool );
  
//   /** Adds a textual display to the canvas of type @a type. */
//   void addTextDisplay ( PlotterBase *, 
//                         const std::string & )
//     throw ( FactoryException );
//   /** Adds a textual display to the canvas of type @a type. This
//       method is used when the text type requires additional text. */
//   void addTextDisplay ( PlotterBase *, 
// 	                const std::string &, const std::string &  );
 
//   /** Adds a text display at the relative position (xrel, yrel) in
//        the selected item's coordinates.  This method returns the
//        relative coordinates of the lower left corner as a pair of
//        doubles.
//    */
//   std::pair<double, double> addTextDisplayAt ( PlotterBase *,
//                                                 const std::string & ,
//                                                 const std::string & ,
//                                                 double , double );

   /** Removes the @a plotter and its view from the canvas. 
   */
  void removeDisplay ( PlotterBase * );
    /** Returns the selected plotter. */
  PlotterBase * selectedPlotter ();

  /** Returns the QtView object that is the Observer of the @a
      plotter. */
  QtView * getViewFor ( const PlotterBase * );

//   /** Returns the list of ViewBase objects on the canvas. */
//   const std::vector < const ViewBase * > & views ();

//    /** Clears and fills @a plotter_list with all the PlotterBase
//       objects on the canvas. */
//   void fillPlotterList ( std::vector < PlotterBase * > &  );

   /** Sets the display interval feature to enabled if @a yes is @c
      true, otherwise sets it to disabled. */
  void setIntervalEnabled ( bool );
   /** Saves the document to the specified file. */
  void saveAs ( const std::string & );
  /** Saves the document along with its NTuple objects. */
  void saveAllAs ( const std::string & );
  /** Saves the @a plotter as image file.  The @a filename should have
      suffix appropriate for the image format desired.  If no suffix
      is given, then a file dialog may be opened.  If file exists, it
      will be over-written as this method is intended to be call from
      Python script.

      @attention Calling this method from a non GUI thread could be
      dangerous.  It would be better to post a SaveAsImageEvent custom
      event to the CanvasWindow.
  */
  void savePlotAsImage ( const PlotterBase *, std::string );
 /** Saves the @a plotter as image file.  The @a filename should have
      suffix appropriate for the image format desired.  If no suffix
      is given, then a file dialog may be opened.  If file exists, it
      will be over-written as this method is intended to be call from
      Python script.  It locks and locks the application object so it
      maybe called from a non-GUI thread.
  */
  void saveAsImage ( const PlotterBase *, std::string );
  /** Removes all items from the canvas.  @sa editClear() 
   */
  void clear ();
 
/** Responds to QCustomEvents.  Responds to SaveAsImageEvent custom
      events.  All others are ignored.
   */
  virtual void customEvent ( QCustomEvent * );

   /** Select or de-select all views in the window. */
   void setAllSelected ( bool = true );

   /** Select a specific view in the window. */
   void setSelected ( QtView * );

   /** Retrieve a vector (x, y, z) points from the next mouse event */
   const std::vector<double> & mouseEventData();

  /** Prints the view to Postscript file without prompting user.
   */
  void print ( const std::string &  );

  /** Sets the number of @a columns and @a rows of plots on each
      page.
  */
  void setPlotMatrix ( unsigned int , unsigned int  );

  /** Sets a flag so that plots will be in their selected stated when
      added to the canvas.
  */
  void setAddedSelected ( bool );

  
};
