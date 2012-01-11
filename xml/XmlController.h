/* -*- mode: c++ -*- */

/** @file
 
XmlController class interface

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XmlController.h,v 1.81 2006/02/16 18:31:06 xiefang Exp $

*/

#ifndef _XmlController_H_
#define _XmlController_H_

#include "pattern/libhippo.h"


#include <list>
#include <map>
#include <string>
#include <vector>

namespace hippodraw {
class DataRep;
class DataRepXML;
class DataSource;
class FontBase;
class HiNTupleXML;
class PlotterBase;
class TupleCut;
class TupleCutXML;
class ViewBase;
class ViewBaseXML;
class ViewFactory;
class XmlDocument;
class XmlElement;

/** A base class that is the interface between GUI and the XML
    serialization and deserialization.  

    It implements the application logic which should not be in the
    GUI.  Following the template pattern, several pure virtual member
    functions must be implemented in a derived class to provide a
    concrete implementation.  The derived class will probably use an
    extern XML C++ tool kit, while this class is independent of any
    XML implementation.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/
class MDL_HIPPOPLOT_API XmlController
{

public:

  /** Return codes of opening file. */
  enum Status { Success, OpenError, ParseError, NTupleError };

private:

  /** The factory to be used for creating views. */
  ViewFactory * m_view_factory;

  /** A private copy constructor in order to avoid copying. */
  XmlController ( const XmlController & );

  /** A list of missing NTuples.
   */
  std::list < std::string > m_missing_tuples;
  
  /** A vector of the ids in the Xml file of the missing NTuples.
   */
  std::vector < int > m_missing_ids;

  /** Deletes each element in the map and clears the map.
   */
  void clearDataSourceMap ();

  /** Fills the NTuple map with NTuple objects currently in memory. */
  void fillNTupleMap ();

  /** Fills the TupleCut map with TupleCut objecs contained in one in
      the list of views. */
  void fillTupleCutMap ( const std::vector < const ViewBase * > & views );

  /** Save the ViewBase objects in @a selview to a XML in memory
      document.  If an ViewBase objects in @a allviews is an Observer
      of of one in @a selviews, then save it as well. */
  void saveToDocument ( const std::vector < const ViewBase * > & selviews );

  /** Restores the views in the current document. */
  const std::list < ViewBase * > &
  getFromDocument ( ViewFactory * facotry,
		    const std::vector < const ViewBase * > & views );

  /** The pasteboard object. */
  XmlDocument * m_pasteboard;

  /** A copy of a selection. */
  XmlDocument * m_selectboard;

  /** A list of reconstructed views. */
  std::list < ViewBase * > m_views;

  /** The map of data sources to XmlElements for the current document. */
  std::map < const DataSource *, XmlElement * > m_data;

  /** The map of memory address to unique integer identifier.
   */
  std::map < const void *, int > m_addr_map;

  /** The iterator for memory address to unique integer identifier.
   */
  typedef std::map < const void *, int >::iterator AddrMapIterator_t;

  /** The next integer identifier to be generated.
   */
  int m_id;

  /** A mapping from XML id to created data sources. */
  std::map < int, const DataSource * > m_tuple_map;

  /** A mapping from XML id to created TupleCut. */
  std::map < int, const TupleCut * > m_tuple_cut_map;

  /** A mapping from DataRep Id to created DataRep. */
  std::map < int, DataRep * > m_datarep_map;

  /** The mapping from PlotterBase Id to created PlotterBase. */
  std::map < int, PlotterBase * > m_plotter_map;

  /** The singleton instance of a ViewBaseXML object. */
  ViewBaseXML * m_view;

  /** The singleton instance of the DataRepXML object. */
  DataRepXML * m_datarep_xml;

  /** The singleton instance of the XML handler for data source. */
  HiNTupleXML * m_ntuple;

  /** The singleton instance of the XML handler for TupleCut objects. */
  TupleCutXML * m_tuple_cut_xml;

  /** The XmlElement object used to parse the DOM tree. */
  XmlElement * m_element;

  /** Creates a new document.  A derived class should set the member
      #m_xml_doc to point to a concrete class derived from the
      abstract class XmlDocument. */
  virtual void newDocument ( const std::string & name ) = 0;

  /** Opens an existing DOM Document file @a filename.  The
      application's current directory will be set to the path to the
      file. 
      @return XmlController::Success if successful, otherwise
      returns another member of the Status enumeration. */

  virtual Status openDocument ( const std::string & filename );

  /** Attempts to open ntuple files in the current document and put
      them into the map. Returns Success if successful, otherwise
      returns NTupleError. */
  virtual Status openNTuples ( );


  /** Creates all the DataRep objects and fills the reference map. */
  void getDataReps ();

  /** Creates all the TupleCut objects and fills the reference map. */
  void getTupleCuts ();

  /** De-serializes the ViewBase objects. */
  virtual void getViews ( );

  /** Creates the ViewBase objects described in the root XML element. */
  void getViews ( const XmlElement * root );

protected:

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  XmlController();

public:

  /** The destructor. */
  virtual ~XmlController();

  /** The current document being generated or read.  This member is
      made public so that element creator that need it do not need to
      have it passed as argument.  */
  static XmlDocument * m_xml_doc;

  /** Creates an DOM Document with name @a doc_name.  The document
      becomes the current document. */
  virtual void createDocument ( const std::string & doc_name );

  /** Returns @c true if the specified file exists. */
  virtual bool fileExists ( const std::string & filename ) const = 0;

  /** Attempts to open and parse the specified HippoDraw document.
      Returns Success if successful, otherwise returns another member
      of the OpenStatus enumeration. */
  Status openFile ( const std::string & filename );

  /** Returns the list of views in the current document. The views are
      created using the specified view factory. */
  virtual const std::list < ViewBase * > & getViews ( ViewFactory * );

  /** Returns @c true if all the NTuple objects used by the @a views
      have been saved to or read from a file. */
  bool areDataSourcesSaved ( const std::vector < const ViewBase * > & views );

  /** Serializes the ViewBase objects in @a views and their appropriate
      subcomponents.   Returns @c true if
      successful, otherwise returns @c false. */
  virtual bool 
  serializeViews ( const std::vector < const ViewBase * > & views );

  /** Returns @c true if global pasteboard is empty. */
  bool isPasteboardEmpty ();

  /** Creates and returns a list of objects that was copied to the
      pasteboard.  The parameter @a views is used when the an object
      makes a reference to object already on the canvas.  For example,
      if an object was a DataRep with a TupleCut applied, then newly
      created object needs to have a reference to that TupleCut. */
  const std::list < ViewBase * > & 
  getFromPasteboard ( ViewFactory * factory,
		      const std::vector < const ViewBase * > & views );

  /** Creates and returns objects that were copied to the selection
      copy.  @sa getFromPasteboard. */
  const std::list < ViewBase * > & 
  getFromSelectionCopy ( ViewFactory * factory,
			 const std::vector < const ViewBase * > & views );

  /** Saves the document represented by the list of ViewBase objects
      to the pasteboard.  If any ViewBase objects in @a allviews is an
      Observer of @a selviews, then save it as well.  Returns the list
      of ViewBase objects actually saved to pasteboard. */
  void saveToPasteboard ( const std::vector < const ViewBase * > & );

  /** Saves the document represented by the list of ViewBase objects
      to the selection copy.  If any ViewBase objects in @a allviews is an
      Observer of @a selviews, then save it as well.  Returns the list
      of ViewBase objects actually saved to pasteboard. */
  void saveToSelectionCopy ( const std::vector < const ViewBase * > & views );

  /** Saves the document represented by list of ViewBase objects to
      file with name @a filename.   Returns 0 if successful. */
  Status saveToFile ( const std::vector < const ViewBase * > & views,
		      const std::string & filename );


  /** Creates a new empty display and makes it the current one. */
  PlotterBase *  createDisplay ( );

  /** Adds ViewBase elements to the root of the document. */
  void addViews ( const std::vector < const ViewBase * > & views );

  /** Creates a new view for the current PlotterBase object and adds
      it to the list of views.
  */
  ViewBase * createView ( PlotterBase * );

  /** Creates a empty QtFont object.
   */
  FontBase * createFont () const;

  /** Adds the DataSource to the document if it doesn't already exist.
      Returns the Dom element corresponding to the DataSource. */
  virtual XmlElement * addDataSource ( const DataSource * ntuple );

  /** Returns pointer to data source from the xml reference ID. */
  const DataSource * getDataSource ( int ref_id );

  /** Returns pointer to the TupleCut from the xml reference Id.
   */
  const TupleCut * getTupleCut ( int ref_id );

  /** Returns a pointer to the DataRep from the xml reference Id. */
  DataRep * getDataRep ( int ref );

  /** Returns a pointer to the PlotterBase object from the XML
      refereed Id.
  */
  PlotterBase * getPlotter ( int ref );

  /** Registers the plotter with @a id.
   */
  void registerPlotter ( int id, PlotterBase * plotter );

  /** Returns the unique integer identifier for given address.
   */
  int getId ( const void * address );

  /** Return a list of missing tuples. Will be updated by the GUI
      and try to open them again.
  */
  std::list < std::string > & getMissingTuples ();

  /** Open the missing tuples updated by GUI.
   */
  virtual Status openUpdatedNTuples ( );

};

} // namespace hippodraw

#endif // _XmlController_H_
