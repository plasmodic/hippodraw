/* -*- mode: c++ -*- */

/** @file

hippodraw::CanvasView class interface

Original taken from QTDIR/examples/canvas/canvas.h version 3.0

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CanvasView.h,v 1.210 2007/06/22 21:15:23 pfkeb Exp $

*/

#ifndef CanvasView_H
#define CanvasView_H

#include "FigureEditor.h"

#include <QtGui/QMouseEvent>

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <list>
#include <map>

class QFileDialog;
class QMainWindow;
class QActionGroup;
class QFont;
class QMainWindow;
class QAction;

namespace hippodraw {

class Inspector;
class PlotterBase;
class QtView;
class ViewBase;
class XmlController;
class NTuple;
class PickTable;
class CreateNTuple;
class QtGroupView;

/** This class implements additional FigureEditor functionality
    particular to QtView objects.

    @attention Methods that only need to know about QCanvasItem
    objects should be implemented in the base class.

    @bug @@@@@@ The following sequence causes a crash: Run
    histogram.py, edit->copy, edit->paste, select with mouse, and
    edit->paste again.

    @bug @@@@@@ A copy followed by past of GroupView cause crash.
    Other crashes are related to this one.

    @request @@@@ Scale the view so that full page appears in Window.

    @request @@@@ Coordinates should show when mouse is over plot.
    Should be no need for right mouse down.

    @request Need remove empty pages menu item.

    @bug @@@@@ Only text rep box tracks the target plot when it is moved.


    @bug Canvas size doesn't automatically enlarge when size of a view is
    dragged to be larger than canvas.

    @todo Should implemented method hasSelectedViews() since in a
    number of places the implementation calls selectedViews only to ask
    if vector is empty.

    @author Jim Chiang <jchiang@slac.stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
*/
class MDL_QTHIPPOPLOT_API CanvasView : public FigureEditor
{

Q_OBJECT

private:

  /** List of menu items of recently opened files. */
  std::list <QAction *> m_recent_list;

  /** A map from a view to the PickTable. */
  std::map < const QtView *, PickTable * > m_table_map; 

  /** The XML controller. */
  static XmlController * s_xml_controller;

  /** The Inspector object.  @sa #setInspector. */
  Inspector * m_inspector;

  /** The application name. 
   */
  static const std::string s_app_name;

  /** A QFileDialog filters string for supported image types. */
  static std::string s_image_filters;

  /** Rectangle  showing the area which is to be magnified.
   */
   QGraphicsRectItem * m_drag_rect;

  /** The QFileDialog for saving images.  The file dialog for saving
      images.  It contains the supported image format file filters.
      each CanvasView has its own, if needed, because the dialog must
      have a parent widget.
   */
  QFileDialog * m_image_dialog;

   /** Fill the picked vector with the event data. */
   bool fillPickedPoint(QMouseEvent * e, std::vector<double> & picked);

   void updatePickTable(QtView * qtview, std::vector<double> & picked);

   void setPlotZoom(const std::vector<double> & start_picked, 
                    const std::vector<double> & picked);

  /** Number of default sized plots horizontally.
   */
  int m_num_width;

  /** Number of default sized plots vertically.
   */
  int m_num_height;

  /** Set to @c true if displays should be selected when added.
   */
  bool m_add_selected;
  
  /** Cut mode: 1: normal; 2: click; 3: drag.
   */
  int m_cut_mode;

  /** Set to the current range of the selected plotter
   */
  std::vector<double> m_current_range;
  
  /** Initialize the settings save from previous session. 
   */
  void initSettings ();

  /** Saves the settings for use in future session. 
   */
  void saveSettings () const;

  /** Sets the default fitter.
   */
  void initFitterDefault ();

  /** Sets the default fitter in the FitterFactory.
   */
  void setFitterDefault ( const std::string & name );

  /** Initializes the fitter setting menu items.
   */
  void initFitterMenu ( QActionGroup * group );

  /** Creates the image filters string.
   */
  static void createImageFilters ();

  /** Returns @c true if suffix string is one for valid image
      format.
  */
  static bool isValidImageFormat ( const std::string & suffix );

  /** Creates the QFileDialog with supported filters string for
      supported image types.
  */
  QFileDialog * createImageDialog ();
 
  /** Sets the canvas as being changed. */
  void setChanged ( bool yes = true );

  /** Adds a QtView object to the CanvasView.  If @a select is @c
      true, then sets added object as selected item. */
  void addView ( QtView * view, bool select );

  /** Places each item in @a items on the canvas.
   */
  void placeItems ( QList<QGraphicsItem *> &items );

  /** Sets the default size and position for the @a view.
   */
  virtual void calcDefaultSize ( QtView * view, double aspect_ratio = 0.0);

  /** Moves the @a view to a nice position with respect to the
      plotter's QCanvasItem.
  */
  void moveToPlotter ( const PlotterBase * plotter, QtView * view );

  /** Calculates the default size and position on the canvas.
   */
  void sizeAndPlace ( QtView * view );

  /** Moves the @a view to the specified position with respect to the
      currently view for @a plotter.
  */
  std::pair<double, double> moveToPlotterAt ( const PlotterBase * plotter,
					       QtView * view,
                                               double x, double y );

  /** Fills the input vector with selected items and their
      observers. 
  */
  void fillSelectedWithObservers ( std::vector < const ViewBase * > & );

public:
 
   CanvasView ( QGraphicsScene *, QWidget * parent = 0, 
		const char * name = 0, Qt::WFlags f = 0 );

  void initFrom ( const std::list < ViewBase * > & views );

  /** Initializes canvas from data in file @a filename.*/
  void initFromFile ( const QString & filename );

  /** Initializes the fitter settings menu items.  Initializes the
      fitter settings menu items by adding them to @a group.  Looks in
      user's settings for fitter name.  If found sets the
      FitterFactory default.
   */
  void initFitterSettings ( QActionGroup * group);

  /** Initializes the recently opened files menu items.
   */
  void initRecentFiles ( QActionGroup * ag );

  /** Initializes the positions of dock windows.
   */
  void initDockWindows ( QMainWindow * mw );

  /** Add recently opened files */
  void addRecentFile (const QString & filename, QActionGroup * ag );

  /** Sets the default fitter setting and the FitterFactory default.
   */
  void setFitterDefault ( const QString & name );

  /** Copies selected items to pasteboard.  */
  void copySelectedToPasteboard ();

  /** Copies from the pasteboard.  Copies from pasteboard and pastes
      the views to the canvas. */
  void copyFromPasteboard ();

  /** Adds from pasteboard.  Copies from the pasteboard and adds the
      plot to next available space.
  */
  void addFromPasteboard ();

  /** Removes the selected items from canvas and copies them to the
      pasteboard. if @a paste is @c true.
  */
  void deleteSelected ( bool paste );

  /** Adds the views to the canvas.  Adds the views slightly displayed
      from their original position. */
  void pasteViews ( const std::list < ViewBase * > & views );

  /** Restores the selected views from the selection copy. */
  void restoreFromSelectCopy ();

  /** Saves the current range of the selected plotter*/
  void setCurrentRange();

  /** Returns the current range of the selected plotter*/
  std::vector<double>& getCurrentRange() ;

  /** Sets the Inspector object.  The Inspector object will receive
      notification of changes to the canvas. */
  void setInspector ( Inspector * inspector );

  /** Gets the inspector object. */
  Inspector * getInspector ();

  /** Returns the selected PlotterBase object. */
  PlotterBase * selectedPlotter () const;

  /** Returns a list of selected plotters.
   */
  std::vector < PlotterBase * > getSelectedPlotters () const;

  /** Creates a QtView object for the @a display and inserts it onto
      the canvas in the next available free place. Selects the created
      view if @a select is true. */
  void addPlotDisplay ( PlotterBase * plotter, bool select);

  /** Save the recently opened file list. */
  void autosaveSettings ( QMainWindow * mw);
public:

  /** Add a display of text Adds a display of text to that will be
      associated with @a plotter.  Uses the optional @a text if needed
      by the display. */
  void addTextDisplay ( PlotterBase * plotter, 
			const std::string & s, 
			const std::string & text = std::string () );

   /** Adds a text display at the relative position (xrel, yrel) in
       the selected item's coordinates.  This method returns the relative
       coordinates of the lower left corner as a pair of doubles.
   */
   std::pair<double, double> addTextDisplayAt ( PlotterBase * plotter,
                                                const std::string & type,
                                                const std::string & text,
                                                double xrel, double yrel);


  /** Add a text display which displays functions parameters. */
  void addFuncDisplay ( PlotterBase * plotter, const std::string & s );

  /** Removes the display from the canvas. 
   */
  void removeDisplay ( PlotterBase * plotter );

  /** Returns temporary list of selected QtView objects on the canvas.
      The list is valid until the next time this function or #views
      is called. */
  const std::vector<const ViewBase *> selectedViews () const;

  /** Returns temporary list of all QtView objects on the canvas.  The
      list is valid until the next time this function or selectedViews
      is called.
  */
  const std::vector<const ViewBase *> views () const;

  /** Clears and fills @a plotter_list with all the PlotterBase
      objects on the canvas.
  */
  void fillPlotterList ( std::vector < PlotterBase * > & plotter_list );

  /** Returns the QtView object that is the Observer of the @a
      plotter. */
  QtView * getViewFor ( const PlotterBase * plotter ) const;

  /** Switches View to the previous view for the selected item on the 
      canvas
   */
  void previousView ();

  /** Prints the canvas.  Prints the QtView items on the canvas. 
   */
  virtual void print ();

  /** Prints the view to Postscript file @a filename.
   */
  virtual void print ( const std::string & filename );

  /** Re-tiles the canvas.  Removes all items from the canvas and
      adding them back with the default size and positions. */
  void reTile ();

  /** Re-tiles the current page.
   */
  void reTilePage ();

  /** Sets the display interval feature of all views to enabled if @a
      yes is @c true, otherwise sets the feature to disable. */
  void setIntervalEnabled ( bool yes = true );

  /** Removes the selected objects from the canvas. */
  void removeSelected ();

  /** Saves the selected QCanvasItem objects to a image file.

      @request @@@@ Add SVG format to types available.
 */
  void saveSelectedImages ();

   /** Save the selected objects to an image file, overwriting the file */
   void saveSelectedImages ( const std::string & filename );

  /** Saves the plotter as an EPS image file.
   */
  void savePlotAsEps ( const PlotterBase * plotter,
		       const std::string & filename ) const;

  /** Saves the plotter as an FITS image file.

      @note This method is only available if application was build
      with optional FITS support.
   */
  void savePlotAsFits ( const PlotterBase * plotter,
			const std::string & filename ) const;

  /** Saves all the QtView canvas item to a XML file.
   */
  void saveAs ( const QString & filename ) const;

  /** Saves all the @a views  to a XML file.
   */
  static void saveAs ( const std::vector < const ViewBase * > & views,
		       const QString & filename );

  /** Wraps the @a plotters with a QtView then saves them to XML file.
   */
  static void saveAs ( const std::vector < PlotterBase * > & plotters,
		       const QString & filename );

  /** Saves the DataSource as a text NTuple.  Save the DataSource as a
      text NTuple.  Saves the file with a file name suffix @a
      suffix.
   */
  void exportTextTuple ( const std::string & suffix );

  /** Show the Inspector window.
   */
  void showInspector ();

  /** Show the pick table. */
  void viewShowPickTable();

  /** Sets the number of @a columns and @a rows of plots displayed on
      each page.
  */
  void setPlotMatrix ( unsigned int columns, unsigned int rows );

  /** Raise the create NTuple dialog. */
  void createNTuple();

  /** Raises the settings dialog. */
  void setup ();
 
  /** Sets the fonts for the views */
  void setFonts( const QFont & font );
    
  /** Brings up dialog version numbers and contributors. */
  void helpAbout ();

  /** Returns the name of the application.
   */
  const std::string & applicationName () const;

  /** Sets to flag to make plot in its selected state when added to
      the canvas.*/
  void setAddedSelected ( bool yes );

  void setCutMode ( int mode );

  /** Gets the pick table of selected plotter.
   */
  NTuple * getPickTable ();

  /** Gets the pick table of a plotter.
   */
  NTuple * getPickTable ( const PlotterBase * plotter );

  /** Create a GroupView object containing selected views. */
  void groupView ();

  /** Ungroup the GroupView object to separated Views. */
  void ungroupView ();

private:

  /** Raises dialog box for error in image format.
   */
  void invalidImageFormatError ( const std::string & filename );

  /** Saves the selected objects as EPS image file @a filename. */
  void saveSelectedAsEps ( const std::string & filename );


  /** Saves the selected objects as FITS image file @a filename.

      @note This method is only available if application was built
      with optional FITS support.
   */
  void saveSelectedAsFits ( const std::string & filename );
public:
  /** Raises a dialog box indicating that FITS support is not
      available.
   */
  void showFitsError () const;
private:

  /** Sets visible state of QCanvasItem objects that are not also
      QtView objects to @a visible. 
  */
  void setVisibleNonViews ( bool visible );

  /** Converts the string to lower case.
   */
  static void toLower ( std::string & upper );

public slots: 
  /** Notifies observers of a change in CanvasView.  @attention This
      method is implemented using QApplication::postEvent mechanism.
      It appears that under Microsoft Windows, that when code
      supporting one window wants to call a method in code supporting
      another window that the event mechanism must be used.
      Otherwise, the application freezes solid.
  */
  void notifyObservers ();

  /** Creates a QtView object for the @a display and inserts it onto
      the canvas in the next available free place. Selects the created
      view if the user settings for this is @c true.
  */
  void addPlotDisplay ( PlotterBase * plotter );

  /** Removes all the QCanvasItem objects from the canvas. */
  void clear ();

  /** Saves the @a plotter as image file @a filename.  If @a filename
      has a suffix, then use the suffix to select the image format.
      If no suffix is given, use the last selected suffix, or "jpg" if
      one had not previously been selected.
   */
  void savePlotAsImage ( const PlotterBase * plotter,
			 const std::string & filename );

  /** Swaps the canvas' orientation from portrait to landscape or visa verse.
   */
  void swapOrientation ();

  /** Wraps the @a plotter with a new QtView object.
   */
  static QtView * wrapPlotter ( PlotterBase * plotter );

};

} // namespace hippodraw

#endif // CanvasView_H
