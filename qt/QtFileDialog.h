/* -*- mode: c++ -*- */

/** @file

hippodraw::QtFileDialog class interface

Copyright (C) 2005, 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtFileDialog.h,v 1.25 2007/06/21 22:52:44 pfkeb Exp $

*/

#ifndef _QtFileDialog_H_
#define _QtFileDialog_H_

#include <QString>

class QWidget;

namespace hippodraw {

  class DataSource;

/** A class to handle various file types.

    @bug The dialog should remember the suffix of the last file
    selected.

    @bug @@@@ Mac OS X only.  Doesn't show the available file types
    and odesn't open SVAC ROOT file with Qt 4.3, ok with Qt 3.

    @request @@@@@@ Make sure second use of this dialog, starts in
    previously used directory.  Make sure previously selected file
    type is used on second use.

    @request @@@@ All file types should be shown and information dialog
    raised if application was not built to support it.

    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */

class QtFileDialog
{

private:

  /** The file name suffix used for the application's document
      files.
  */
  static const QString s_doc_suffix;

  /** The file name suffix used for the application's text data source
      files.
  */
  static const QString s_data_suffix;

  /** The open file filter.
   */
  static QString s_open_filter;

  /** Checks for duplicate column labels in the DataSource @a source.
      Brings up a information dialog if duplicate column labels are
      found.
  */
  void checkDuplicateLabels ( const DataSource * source, QWidget * );

  static const QString s_textntuple_ext;
  static const QString s_textntuple_filt;
  static const QString s_fits_ext;
  static const QString s_fits_filt;
  static const QString s_fitsgz_ext;
  static const QString s_fitsgz_filt;
  static const QString s_root_ext;
  static const QString s_root_filt;

public:

  /** The constructor.
   */
  QtFileDialog ();

  /** Creates the open file filter.
   */
  static const QString & createOpenFilter ();

  /** Creates the open file filter.
   */
  static const QString & createBrowseFilter ();

  /** Creates a filter for saving a document.
   */
  static const QString & createDocumentFilter ();

  /** Creates a filter for exporting DataSource to file.
   */
  static const QString & createExportFilter ();

  /** Returns the suffix for a document.
   */
  static const QString & getDocSuffix ();

  /** Returns the suffix for a text data source files.
   */
  static const QString & getTextSuffix ();

  /** Returns @c true if @a suffix is one corresponding to document file.
   */
  static bool isDocSuffix ( const QString & suffix );

  /** Returns @c true if @a suffix is one corresponding to FITS
      DataSource file.
   */
  static bool isFitsSuffix ( const QString & suffix );

  /** Returns @c true if @a suffix is one corresponding to ROOT
      DataSource files.
  */
  static bool isRootSuffix ( const QString & suffix );

  /** Returns @c true if @a suffix is one corresponding to text NTuple file.
   */
  static bool isTextSuffix ( const QString & suffix );

  /** Opens a text NTuple file.
   */
  static void openTextTuple ( const QString & filename );

  /** Opens a ROOT DataSource file.
   */
  void openRootTuple ( const QString & filename, QWidget * parent );

  /** Returns @c true if the file @a filename is a FITS file.
   */
  static bool isFitsFile ( const QString & filename );

  /** Returns @c true if the file @a filename is a compressed FITS file.
   */
  static bool isZippedFitsSuffix ( const QString & suffix);

  /** Opens a FITS DataSource file.
   */
  void openFitsTuple ( const QString & filename, QWidget * parent );


#ifdef HAVE_CFITSIO
  /** Save a FITS file.

      @note This method is only available if application is built with
      optional FITS support.
   */
  void saveFitsTuple ( const QString & filename, QWidget * parent );
#endif

  /** Save a text file.
   */
  void saveTextTuple ( const QString & filename, QWidget * parent );

  /** Returns a filename for exporting a DataSource. Displays a file
      dialog for the user to select a filename for exporting a
      ntuple. The QFileDialog will be child of @a widget.
  */
  static QString getExportTupleFilename ( QWidget * widget);

};

} // namespace hippodraw

#endif // _QtFileDialog_H_
