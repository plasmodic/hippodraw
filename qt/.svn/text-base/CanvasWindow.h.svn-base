/* -*- mode: c++ -*-  */

/** @file

hippodraw::CanvasWindow class interface.
 
Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

Original from $QTDIR/examples/canvas/canvas.cpp version 3.0

$Id: CanvasWindow.h,v 1.383 2006/12/08 22:11:39 pfkeb Exp $

*/

#ifndef CanvasWindow_H
#define CanvasWindow_H

#include "qtui/ui_CanvasWindowBase.h"

#include <string>
#include <vector>
#include <utility>

class QCloseEvent;
class QEvent;
class QHideEvent;
class QResizeEvent;
class QShowEvent;

namespace hippodraw {

class NTuple;
class CanvasView;
class QtFileDialog;
class PlotterBase;
class PlotTable;
class QtView;
class ViewBase;
class XmlController;

/** A concrete window class that contains the canvas and responds to
    menu item and tool bar events from the window and messages sent by
    the Inspector.

    @attention This class should implement those actions specific to
    HippoDraw as an application.  All changes to the CanvasView should
    be implemented in the that class so that other applications can
    benefit from the implementation.

    @bug @@ After using edit->paste, trying to selected the pasted
    plot leads to selecting the original.

    @bug @@@@@@ It appears that if one cuts a cut from the canvas,
    then the target doesn't get its cut removed.

    @bug If a document could not be opened, error messages displays,
    but one get a new empty canvas window.  It is not throwing an
    exception which is read to be caught.

    @bug The View/Cut empty pages button doesn't work and is not
    enabled.

    @bug @@@@ Opening a document that references a ROOT array variable
    doesn't work.

    @request Have a editable DataSource  table view.

    @request @@ Open documents read only.  This could be the default
    when the file Open menu item is used.

    @request A clear all button on the tool bar.

    @todo @@@@ Be able to ask canvas if it is saved from Python.

    @todo Make it a setting if the Inspector window comes up
    automatically or not.

    @todo Respond to Page Up and Page Down keys.

    @todo Needs something to display on the canvas for values only
    seen in the Inspector.  One example is the name of a DataSource file
    file.  This is so that the document when printed can be more
    useful as a stand alone.

    @todo Use Qt's network example ftpclient to open files via ftp.

    @author Jim Chiang <jchiang@slac.stanford.edu>
    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/

  class MDL_QTHIPPOPLOT_API CanvasWindow : public QMainWindow,
	private Ui::CanvasWindowBase
{
   Q_OBJECT

private:

  /** The controller object capable of serializing the canvas contents
      as XML and serializing them. */
  XmlController * m_xml_controller;

  /** The CanvasView object which is the central widget. */
  CanvasView * m_canvas_view;

  /** The CanvasView object that is being browsed.
   */
  CanvasView * m_browsed_canvas;

  /** The open file dialog for this window.
   */
  QtFileDialog * m_file_dialog;

  /** The prefix part of the window title. */
  QString m_prefix;

  /** The file name part of the window title. */
  QString m_filename;

  /** The changed status part of the window title. */
  QString m_changed;

  /** The singleton instance of PlotTable. 
   */
  static PlotTable * s_plot_table;

  /** The current changed status. If @c true, the document has been
      changed since last saved or opened.  If @c false the document
      has not been changed since saved or opened. */
  bool m_hasChanged;

  /** A flag set to @c true if the CanvasWindow should not accept a
      close event. */
  bool m_inhibit_close;

  /** A flag set to @c true to allow the window to close without
      prompting the user.
  */
  bool m_allow_close;

  /** True if filename has been assigned, false if not. */
  bool m_filenameExists;

  /** Returns the chosen file name from a save canvas dialog.
   */
  std::string  getSaveDocFilename ();

  /** Sets the file name component of the window's title. */
  void setTitleFileName ( const QString & name );

  /** Sets the file name of document.  Sets the file name of the
      canvas document from a QtFileDialog with the user.  Returns @c
      true if the user gave the dialog a file name.  Returns @c false
      if the user canceled.
  */
  bool setFilenameFromDialog ();

  /** Returns @c true if all the data sources used by the document
      have been saved to or read from a file.  If any data sources are
      only in memory, then inform the user with dialog and return
      false. */
  bool areDataSourcesSaved ();

  protected slots:

  /** Informs the application object if this window has become the
      active one. 

      @attention The implementation of this function informs the
      singleton instance of the application that this window has
      become the active one.  Therefore, the implementation of the
      constructor must not cause this method to be called before the
      application object is full constructed.
  */
  virtual void windowActivationChange ( bool oldActive );

  /** Informs the application object that is window is about to close. */
  virtual void closeEvent ( QCloseEvent * e );

  /** Informs the application object that the window has been hidden. */
  virtual void hideEvent ( QHideEvent * e );

  /** Informs the application object that the window has been shown. */
  virtual void showEvent ( QShowEvent * e );

  /** Informs the Canvas to re-size as well. */
  virtual void resizeEvent ( QResizeEvent * );

  /* Implementation of functions declared in base class. */

  /** Prints the canvas to a Postscript file.  

      @todo Add the `.ps' subscript to the file name if missing.
  */
  virtual void filePrint ();

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

  /** Responds to "Clear" menu item.  Prompts the user with a warning
      dialog, then removes all canvas items or not depending on user
      response.  @sa clear()
  */
  virtual void editClear ();

  virtual void editCopy ();
  /** Removes selected items from canvas and add them to the
      pasteboard.
  */
  virtual void editCut ();

  /** Removes selected items from canvas without copying them to the
      pasteboard.
  */
  virtual void editDelete ();

  virtual void editPaste ();

  /** Re tiles the entire document.
   */
  virtual void editReTile ();

  /** Re tiles only the current page.
   */
  virtual void editReTilePage ();

  /** Sets all QCanvasItem objects to selected. 
   */
  virtual void editSelectAll ();

  /** Responds to the undo action. 
   */
  virtual void editUndo ();

  virtual void fileExportTextTuple ();

  /** Create a new NTuple from selected cut and column.
   */
  virtual void fileCreateNTuple ();

  /** Brings up a QAssistant client.
   */
  virtual void helpHelp ();

  /** Brings up standard window showing version, authors, etc. 
   */
  virtual void helpAbout ();

  /** Brings up Qt's about message box.
   */
  virtual void aboutQt ();

  /** Locks a canvas item.  Locks a canvas item so it can not be moved
      or re-sized. 
  */
  virtual void viewLock ();

  /** Unlocks a canvas item.  Unlocks a canvas item so it can be moved
      and re-sized. 
  */
  virtual void viewUnlock ();

  /** Shows the Inspector window.
   */
  virtual void viewShowInspector ();

  /** Shows the plot table. 
   */
  virtual void viewShowPlotTable ();

  /** Shows the pick table window.
   */
  virtual void viewShowPickTable ();

  /** Switches View to the previous view for the selected item on the 
      canvas
   */
  void previousView ();

   /** Switches View to the next view for the selected item on the 
      canvas
   */
  void nextView ();

  /** Sets the zoom mode to true or false  
   */
  virtual void setZoomMode ( bool on );

  /** Sets the canvas settings. 
   */
  virtual void settingCanvas ();

  /** Sets the printer settings. 
   */
  virtual void settingPrinter ();

  /** Sets the font settings.
   */
  virtual void settingFonts ();

  /** Responds to change in fitter settings.
   */
  virtual void setFitter ( QAction * );

  /** Responds to open recent file.
   */
  virtual void openRecentFile ( QAction * );

  /** Enlarges the view of the canvas. */
  virtual void viewZoomIn();

  /** Reduces the view of the canvas. */
  virtual void viewZoomOut();

  /** Returns the view of the canvas to normal. 
   */
  virtual void viewZoomReset();

  /** Adds a page to the canvas */
  virtual  void viewAddPage();

  /** Sets the cut mode.  
   */
  virtual void setCutMode ( QAction * );

  /** Group the views.
   */
  virtual void groupView ();

  /** Ungroup the views.
   */
  virtual void ungroupView ();

public:

  /** constructor.  The arguments are defined by the inherited Qt
      QMainWindow class */
   CanvasWindow ( QWidget * parent = 0 );

  /** A copy constructor that does nothing except keep boost.python
      happy.  Without it, boost.python interface wouldn't compile
      because QMainWindow copy constructor is private. */
  CanvasWindow ( const CanvasWindow & );

  virtual ~CanvasWindow();

  /** Initializes this object from contents of file. */
  void initFromFile ( const QString & filename );

  /** Responds to QEvents.  Responds to SaveAsImageEvent custom
      events.  All others are ignored.
   */
  virtual void customEvent ( QEvent * event );

  /** If @a yes is @c true, sets a flag so that this object will not
      accept close events. */
  void inhibitClose ( bool yes = true );

  /** Sets the window to allowed to be closed by user without
      prompting.
  */
  void setAllowClose ( bool yes = true );

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
      the document's current status.  @sa #m_hasChanged. */
  void setChanged ( bool flag = true );

  /** Sets the Windows title.  The windows title depends on an number
      of attributes.  They are collected to set the title. */
  void setCaption ();

  /** Update Action objects.  Sets enable status of QAction objects
      such as menu items and tool bar icons. 
  */
  void updateActions ();

public slots:
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
  virtual void fileSaveSelectedImages( const std::string & filename );

  /** Saves the document to the specified file. */
  void saveAs ( const QString & filename );

  /** Saves the document along with its NTuple objects. */
  void saveAllAs ( const QString & filename );

public:
  /** Returns the CanvasView object.
   */
  CanvasView * getCanvasView ();

  /** Adds a textual display to the canvas of type @a type. */
  void addTextDisplay ( PlotterBase * plotter, 
			const std::string & type);

  /** Adds a textual display to the canvas of type @a type. This
      method is used when the text type requires additional text. */
  void addTextDisplay ( PlotterBase * plotter, 
			const std::string & type,
			const std::string & text);

   /** Adds a text display at the relative position (xrel, yrel) in
       the selected item's coordinates.  This method returns the
       relative coordinates of the lower left corner as a pair of
       doubles.
   */
   std::pair<double, double> addTextDisplayAt ( PlotterBase * plotter,
                                                const std::string & type,
                                                const std::string & text,
                                                double xrel, double yrel);

  /** Adds function textual display to the canvas. */
  void addFuncDisplay ( PlotterBase * plotter, const std::string & name );

  /** Adds a new plot display to the canvas.  Adds a new plot display
      to the canvas and puts it in selected state if @a select is @c
      true.
  */
  void addPlotDisplay ( PlotterBase * plotter, bool select );

  /** Adds a new plot to the canvas and selects it. */
  void addDisplay ( PlotterBase * plotter );

  /** Removes the @a plotter and its view from the canvas. 
   */
  void removeDisplay ( PlotterBase * plotter );

  /** Returns the selected plotter. */
  PlotterBase * selectedPlotter ();

  /** Removes all items from the canvas.  @sa editClear() 
   */
  void clear();

  /** Returns the list of ViewBase objects on the canvas. */
  const std::vector < const ViewBase * > views ();

  /** Clears and fills @a plotter_list with all the PlotterBase
      objects on the canvas. */
  void fillPlotterList ( std::vector < PlotterBase * > & plotter_list );

  /** Returns the QtView object that is the Observer of the @a
      plotter. */
  QtView * getViewFor ( const PlotterBase * plotter ) const;

  /** Sets the display interval feature to enabled if @a yes is @c
      true, otherwise sets it to disabled. */
  void setIntervalEnabled ( bool yes = true );

  /** Saves the @a plotter as image file.  The @a filename should have
      suffix appropriate for the image format desired.  If no suffix
      is given, then a file dialog may be opened.  If file exists, it
      will be over-written as this method is intended to be call from
      Python script.

      @attention Calling this method from a non GUI thread could be
      dangerous.  It would be better to post a SaveAsImageEvent custom
      event to the CanvasWindow.
  */
  void savePlotAsImage ( const PlotterBase * plotter, 
			 const std::string & filename );

  /** Saves the @a plotter as image file.  The @a filename should have
      suffix appropriate for the image format desired.  If no suffix
      is given, then a file dialog may be opened.  If file exists, it
      will be over-written as this method is intended to be call from
      Python script.  It locks and locks the application object so it
      maybe called from a non-GUI thread.
  */
  void saveAsImage ( const PlotterBase * plotter,
		     const std::string & filename );

   /** Select or deselect all views in the window. */
   void setAllSelected ( bool flag = true );

   /** Select a specific view in the window. */
   void setSelected ( QtView * view );

  /** Prints the view to Postscript file without prompting user.
   */
  void print ( const std::string & filename );

  /** Sets the number of @a columns and @a rows of plots on each
      page.
  */
  void setPlotMatrix ( unsigned int columns, unsigned int rows );

  /** Returns a reference the application's QSettings key.
   */
  const QString & getAppKey () const;

  /** Sets to flag to make plots in their selected state when added to
      the canvas. */
  void setAddedSelected ( bool yes );

  /** Gets the pick table the selected plotter.
   */
  NTuple * getPickTable () const;

  /** Gets the pick table of a plotter.
   */
  NTuple * getPickTable ( const PlotterBase * plotter ) const;

  /** Automatically save dock windows and recently opened files. This 
      method is called when choosing exit from menu or closing the window. 
   */
  void autosaveSettings ();

protected slots:

  /** Browse a document file, without opening a window for it.
   */
  virtual void fileBrowse();

   /** Responds to the Quit item in the File menu.
   */
  virtual void fileExit ();

  /** Opens an existing file.  Opens an existing document or data file
      after prompting the user for the document's file name and/or
      type.
  */
  virtual void fileOpen ();

};

} // namespace hippodraw

#endif // CanvasWindow_H
