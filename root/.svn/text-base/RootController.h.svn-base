/* -*- c++ -*- */

/** @file

hippodraw::RootController class interface.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RootController.h,v 1.31 2007/01/10 19:29:01 xiefang Exp $

*/

#ifndef RootController_H
#define RootController_H

#include "pattern/Observer.h"

#include <map>
#include <string>
#include <vector>

class TFile;
class TTree;

namespace hippodraw {

  class DataSource;
  class RootNTuple;

/** A Controller class for ROOT files 

    @bug @@@@@@ testsuite/towercut.py needs fixing.

    @request @@@@ Be able to chain ROOT files.

    @request Read TH1 and TH2 histograms in ROOT file and convert to
    corresponding static histograms.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
  class MDL_HIPPOPLOT_API RootController : private Observer
{

 private:

  /** The list of opened ROOT files. */
  std::map < std::string, TFile * > m_file_map;

  /* The type of container to map RootNTuple to its ROOT file.
   */
  typedef std::map < const DataSource *, std::string > TupleToFileMap_t;

  /** A map to find which ROOT file was used to create RootNTuple.
   */
  TupleToFileMap_t m_tuple_map;

  /** Temporary list of DataSource names in the file.
   */
  std::vector < std::string > m_ntuple_names;

  /** The version of ROOT being used. */
  mutable std::string m_version;

  /** The singleton instance of the RootController.
   */
  static RootController * s_instance;

  /** Attempts to open the file @a name. Throws a std::runtime_error
      exception if file not found.
   */
  TFile * openFile ( const std::string & name );

  /** Closed the named file, if found in list of opened files.
   */
  void closeFile ( const std::string & name );

 public:

  /** Returns the singleton instance of the RootController.
   */
  static RootController * instance ();

  /** The destructor. 
   */
  virtual ~RootController();


  /** Returns the version of ROOT being used.
   */
  const std::string & version () const;

  /** Returns the names of the DataSource  objects contained in the file @a
      name.   For ROOT files, this is the names of the TTree.

      @bug Returns all top level names instead of only the ones that
      could be a DataSource.
  */
  const std::vector < std::string > & getNTupleNames ( const std::string & );

  /** Creates a RootNTuple object named @a name.  @name can be of the
      form "filename.root: tree", in which case the part left of `:'
      is taken as the filename and the part right of the `:' is taken
      as the tree name.  If a `:' does not exist in @a name, then the
      first tree is used.
   */
  DataSource * createNTuple ( const std::string & name );

  /** Creates a RootNTuple from the file @a filename and TTree
      named @a treename.
  */
  DataSource * createNTuple ( const std::string & filename,
			      const std::string & treename );

  /** Initialized and registers new DataSource.
   */
  DataSource * initNTuple ( DataSource * source,
			    const std::string & filename,
			    const std::string & treename );

  /** Returns the named @a tree from @a file in the ROOT file.
   */
  TTree * getTree ( const std::string & file,
		    const std::string & tree );

  /** Fills the vector @a dims.  If the DataSource is a RootNTuple,
      fills the vector @a dim with the dimension of he column labeled
      @a column, otherwise does nothing.
  */
  void fillDimSize ( std::vector < int > & dims,
		     const DataSource * source,
		     const std::string & column );

  /** Expands the array column.  If the DataSource is a RootNTuple,
      expands the column with array TLeaf and returns @c true,
      otherwise returns @c false.
  */
  bool smartExpandRootNTuple ( DataSource * source, std::string & column );

  /** Does nothing, but satisfies the pure virtual function in base.
   */
  virtual void update ( const Observable * );

  /** Closes the file that was used to create the observed RootNTuple.
      Closes the file that was used to create the observed RootNTuple
      if it was the only RootNTuple created from that file.
   */
  virtual void willDelete ( const Observable * obs );

};

} // namespace hippodraw

#endif // RootController_H
