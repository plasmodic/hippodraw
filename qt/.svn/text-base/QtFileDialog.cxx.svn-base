/** @file

hippodraw::QtFileDialog implementation

Copyright (C) 2005, 2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtFileDialog.cxx,v 1.51 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
// for have root cfitsio
#include "config.h"
#endif

#include "QtFileDialog.h"

#include "CanvasWindow.h"
#include "WindowController.h"
#include "Inspector.h"
#include "ListDialog.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/DataSource.h"
#include "datasrcs/NTupleController.h"

#ifdef HAVE_CFITSIO
#include "fits/FitsController.h"
#endif

#ifdef HAVE_ROOT
#include "root/RootController.h"
#endif

#include <QMessageBox>
#include <QFileDialog>

#include <string>

using std::exception;
using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

/* Use `=' syntax so that Doxygen picks it up. */
const QString QtFileDialog::s_doc_suffix = ".hpo";
const QString QtFileDialog::s_data_suffix = ".tnt";
QString QtFileDialog::s_open_filter = "";

const QString QtFileDialog::s_textntuple_ext = ".tnt";
const QString QtFileDialog::s_fits_ext = ".fits";
const QString QtFileDialog::s_fitsgz_ext = ".fits.gz";
const QString QtFileDialog::s_root_ext = ".root";
const QString QtFileDialog::s_textntuple_filt =
  qApp->translate("QtFileDialog", "Text NTuples (*%1)").arg(s_textntuple_ext);
const QString QtFileDialog::s_fits_filt =
  qApp->translate("QtFileDialog", "FITS ntuples (*%1)").arg(s_fits_ext);
const QString QtFileDialog::s_fitsgz_filt =
  qApp->translate("QtFileDialog", "Compressed FITS ntuples (*%1)").arg(s_fitsgz_ext);
const QString QtFileDialog::s_root_filt =
  qApp->translate("QtFileDialog", "ROOT files (*%1)").arg(s_root_ext);

QtFileDialog::
QtFileDialog ()
{
}

const QString &
QtFileDialog::
createOpenFilter ()
{
  const QString doc ( s_doc_suffix );
  const QString xml ( ".xml" );

  s_open_filter.clear();

  s_open_filter = qApp->translate ("QtFileDialog",
				   "All files (*);;"
				   "Documents (*%1 *%2);;"       // doc, xml
				   "%3")         // tnt
    .arg(doc).arg(xml).arg(s_textntuple_filt);

#ifdef HAVE_ROOT
  s_open_filter += qApp->translate("QtFileDialog", ";;%1").arg(s_root_filt);
#endif

#ifdef HAVE_CFITSIO
  s_open_filter += qApp->translate("QtFileDialog",
				   ";;%1"
				   ";;%2")
    .arg(s_fits_filt).arg(s_fitsgz_filt);
#endif

  return s_open_filter;
}

const QString&
QtFileDialog::
createExportFilter ()
{
  s_open_filter.clear();
  s_open_filter += s_textntuple_filt;

#ifdef HAVE_CFITSIO
  const QString fits ( ".fits" );
  const QString gzfits ( ".fits.gz" );
  s_open_filter += qApp->translate("QtFileDialog",
				   ";;%1"
				   ";;%2")
    .arg(s_fits_filt).arg(s_fitsgz_filt);
#endif

  return s_open_filter;
}


const QString &
QtFileDialog::
createBrowseFilter ()
{
  const QString doc ( s_doc_suffix );
  const QString xml ( ".xml" );
  const QString tnt ( ".tnt" );

  s_open_filter.clear();
  s_open_filter = qApp->translate("QtFileDialog",
				  "Documents (*%1 *%2)"   // doc xml
				  ";;All files (*.*)")
    .arg(doc).arg(xml);

  return s_open_filter;
}

const QString &
QtFileDialog::
getDocSuffix ()
{
  return s_doc_suffix;
}

const QString &
QtFileDialog::
getTextSuffix ()
{
  return s_data_suffix;
}

bool
QtFileDialog::
isDocSuffix ( const QString & suffix )
{
  return suffix == s_doc_suffix || suffix == ".xml";
}

bool
QtFileDialog::
isTextSuffix ( const QString & suffix )
{
  return suffix == s_data_suffix;
}

bool
QtFileDialog::
isRootSuffix ( const QString & suffix )
{
  return suffix == s_root_ext;
}

bool
QtFileDialog::
isFitsSuffix ( const QString & suffix )
{
  return suffix == s_fits_ext;
}

bool
QtFileDialog::
isZippedFitsSuffix ( const QString & suffix)
{
  return suffix == ".gz";
}

QString
QtFileDialog::
getExportTupleFilename( QWidget * widget )
{
  const QString & filter = createExportFilter ();
  QString filename;

  QString extname = 0;
  filename = QFileDialog::getSaveFileName ( widget, qApp->translate(
					"QtFileDialog",
					"Save data source to file ..."),
					QString::null, // starting directory
					filter, &extname /* selected filter */);

  if ( filename == QString::null ) return QString();

  if (!filename.endsWith(".tnt") && !filename.endsWith(".fits")
	&& !filename.endsWith(".fits.gz")) {
	  // Add default suffix if not end with .tnt or .fits.
	  if ( extname == s_fits_filt )
	      filename += s_fits_ext;
	  else if (extname == s_fitsgz_filt)
	      filename += s_fitsgz_ext;
	  else
	      filename += s_textntuple_ext;
  }

  QFileInfo info ( filename );
  filename = info.absoluteFilePath();

  return filename;
}
      

const QString &
QtFileDialog::
createDocumentFilter ()
{
  s_open_filter.clear();
  s_open_filter = qApp->translate ( "QtFileDialog",
				    "Doucuments (*%1)" )
    .arg(s_doc_suffix);

  return s_open_filter;
}

void
QtFileDialog::
openTextTuple ( const QString &filename )
{
  NTupleController * controller = NTupleController::instance ();
  try {
    DataSource * tuple =
      controller -> createNTuple ( filename.toLatin1().data() );
  } catch ( const std::exception & e ) {

    QString message = qApp->translate( "QtFileDialog", "Unable to open file:\n" );
    message.append(e.what ());

    QMessageBox::critical ( 0, // parent
			    qApp->translate( "QtFileDialog", "HippoDraw" ),
			    message,
			    QMessageBox::Ok,
			    Qt::NoButton );
  }

}

bool
QtFileDialog::
isFitsFile ( const QString &fn )
{
  bool yes = false;
#ifdef HAVE_CFITSIO
  FitsController * controller = FitsController::instance ();
  FitsFile * fits_file = 0;
  try {
    fits_file = controller ->openFile ( fn.toLatin1().data() );
    if ( fits_file != 0 ) yes = true;
  }
  catch ( ... ) {
    // ignore, was not FITS file
  }
#endif

  return yes;
}

void
QtFileDialog::
checkDuplicateLabels ( const DataSource * source, QWidget * parent )
{
  const vector < std::string > & dups = source -> getDuplicatedLabels ();
  if ( ! dups.empty() ) {
    QString message = qApp->translate( "QtFileDialog",
				       "This data source contains two or more columns with\n"
				       "the same label.  The offending labels were\n\n" );
    for ( unsigned int i = 0; i < dups.size(); i++ ) {
      message += dups[i].c_str();
      message += "\n";
    }
    message += qApp->translate("QtFileDialog",
			       "\nThe first column encountered will be used");
    QMessageBox::information ( parent, // parent
			       qApp->translate("QtFileDialog",
					       "HippoDraw"), // caption
			       message,
			       QMessageBox::Ok,
			       Qt::NoButton );
  }
}

void
QtFileDialog::
openFitsTuple ( const QString & filename, QWidget * parent )
{
#ifdef HAVE_CFITSIO
  unsigned int index = 0;

  std::string fn = filename.toLatin1().data();

  FitsController * controller = FitsController::instance ();
  const vector < string > & names
    = controller -> getNTupleNames ( fn );

  if ( names.size () > 1 ) {
    if ( names.size () >= 2  ) {
      ListDialog * dialog = new ListDialog ( parent ); // take defaults for rest
      dialog -> setNames ( names );
      int retval = dialog -> exec ();

      if ( retval == QDialog::Accepted ) {
	index = dialog -> selectedItem ();
      }
      delete dialog;
      if ( retval == QDialog::Rejected ) return;
    }
    else { // was your typical empty image HDU
      index = 1;
    }
  }

  try {
    DataSource * ds = 
      controller -> createNTuple ( fn, names [ index ], index );
    checkDuplicateLabels ( ds, parent );
  } catch ( const runtime_error & e ) {
    QMessageBox::information ( parent, // parent
			       qApp->translate("QtFileDialog", "HippoDraw"), // caption
			       e.what (),
			       QMessageBox::Ok,
			       Qt::NoButton );
    return;
  }

#endif
}

void
QtFileDialog::
openRootTuple ( const QString & filename, QWidget * parent )
{
#ifdef HAVE_ROOT
  unsigned int index = 0;

  std::string fn = filename.toLatin1().data();

  RootController * controller = RootController::instance ();
  const vector < string > & tree_names
    = controller -> getNTupleNames ( fn );
  if ( tree_names.size () > 1 ) {
    ListDialog * dialog = new ListDialog ( parent ); // take defaults for rest
    dialog -> setNames ( tree_names );

    int retval = dialog -> exec ();

    if ( retval == QDialog::Rejected ) return;
    index = dialog -> selectedItem ();
    delete dialog;
  }
  if ( tree_names.empty () ) {
    QString message = qApp->translate( "QtFileDialog",
				       "There were no ntuple objects found in this file.\n"
				       "File not opened." );

    QMessageBox::information ( parent,
			       qApp->translate("QtFileDialog", "HippoDraw"), // caption
			       message,
			       QMessageBox::Ok,
			       Qt::NoButton );
    return;
  }

  try {
    DataSource * tuple =
      controller -> createNTuple ( fn, tree_names[index] );
    checkDuplicateLabels ( tuple, parent );
  } catch ( const runtime_error & e ) {
    QMessageBox::information ( parent,
			       qApp->translate("QtFileDialog", "HippoDraw"), // caption
			       e.what (),
			       QMessageBox::Ok,
			       Qt::NoButton );
    return;
  }

#endif
}

#ifdef HAVE_CFITSIO
void
QtFileDialog::
saveFitsTuple ( const QString & filename, QWidget * parent )
{
  const string tuple_name = 
    WindowController::instance () 
    -> getInspector() 
    -> getSelectedDataSourceName ();

  try {
    FitsController::instance ()
      ->writeNTupleToFile ( tuple_name, filename.toLatin1().data() );
  } catch ( const std::exception & e ) {
    QString message = qApp->translate( "QtFileDialog",
				       "An error occurred in writing file.\n\n" );
    message += e.what();
    QMessageBox::critical ( parent,
			    qApp->translate ( "QtFileDialog", "Write error" ), // caption
			    message,
			    QMessageBox::Ok,
			    QMessageBox::NoButton,
			    QMessageBox::NoButton );
  }
}
#endif

void
QtFileDialog::
saveTextTuple ( const QString & filename, QWidget * parent )
{
  const string tuple_name = 
    WindowController::instance () 
    -> getInspector() 
    -> getSelectedDataSourceName ();

  try {
    NTupleController::instance ()
      ->writeNTupleToFile ( tuple_name, filename.toLatin1().data() );
  } catch ( const std::exception & e ) {
    QString message = qApp->translate ( "QtFileDialog",
					"An error occurred in writing file.\n\n" );
    message += e.what();
    QMessageBox::critical ( parent,
			    qApp->translate ( "QtFileDialog", "Write error" ), // caption
			    message,
			    QMessageBox::Ok,
			    QMessageBox::NoButton,
			    QMessageBox::NoButton );
  }
}
