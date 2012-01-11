/** @file

hippodraw::PlotterBaseXML class implementation

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PlotterBaseXML.cxx,v 1.135 2007/02/09 00:32:28 xiefang Exp $

*/

// for iterator member defect
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "PlotterBaseXML.h"

#include "AxisModelXML.h"
#include "DataRepXML.h"
#include "FontXML.h"
#include "PointRepXML.h"
#include "TransformXML.h"
#include "TupleCutXML.h"
#include "XmlController.h"
#include "XmlDocument.h"
#include "XmlElement.h"

#include "axes/AxisModelLog.h"
#include "controllers/DisplayController.h"
#include "controllers/FunctionController.h"
#include "datareps/DataRepException.h"
#include "datareps/FunctionRep.h"
#include "datasrcs/TupleCut.h"

#include "plotters/Cut1DPlotter.h"
#include "plotters/Cut2DPlotter.h"
#include "plotters/PlotterFactory.h"
#include "plotters/TextPlotter.h"
#include "plotters/XyPlotter.h"

#include "projectors/NTupleProjector.h"

#include "pattern/string_convert.h"

#include <typeinfo>

#include <cassert>

using std::list;
using std::string;

using namespace hippodraw;

PlotterBaseXML::PlotterBaseXML ( XmlController * controller )
  : BaseXML ( "PlotterBase", controller ),
    m_axis ( "axis" ),
    m_title ( "title" ),
    m_x_label ( "xlabel" ),
    m_y_label ( "ylabel" ),
    m_z_label ( "zlabel" ),
    m_pindex ( "pindex" )
{
  m_axismodel = new AxisModelXML ( controller );
  m_datarep = new DataRepXML ( controller );
  m_font = new FontXML ( controller );
  m_pointrep = new PointRepXML ( controller );
  m_transform_xml = new TransformXML ( controller );
  m_tuple_cut_xml = new TupleCutXML ( controller );
}

PlotterBaseXML::
PlotterBaseXML ( const std::string & name, 
		 XmlController * controller )
  : BaseXML ( name, controller ),
    m_axis ( "axis" )
{
  m_axismodel = new AxisModelXML ( controller );
  m_datarep = new DataRepXML ( controller );
  m_font = new FontXML ( controller );
}

PlotterBaseXML::
~PlotterBaseXML ()
{
  delete m_axismodel;
  delete m_datarep;
  delete m_font;
  delete m_pointrep;
  delete m_transform_xml;
  delete m_tuple_cut_xml;
}

bool PlotterBaseXML::areDataSourcesSaved ( const PlotterBase & plotter )
{
  DisplayController * controller = DisplayController::instance ();

  return controller->areDataSourcesSaved ( & plotter );
}

void PlotterBaseXML::createChildren ( XmlElement &tag, 
				      const PlotterBase & plotter )
{
  createAxisModel ( tag, plotter, Axes::X );
  createAxisModel ( tag, plotter, Axes::Y );
  if ( plotter.hasAxis ( Axes::Z ) ) createAxisModel ( tag, plotter, Axes::Z );

  try {
    const XyPlotter & xyplotter 
      = dynamic_cast < const XyPlotter & > ( plotter );
    createFontElements ( tag, xyplotter );
  }
  catch ( ... ) {
    // do nothing
  }

  TransformBase * transform = plotter.getTransform ();
  if ( transform != 0 ) {
    XmlElement * element = m_transform_xml->createElement ( *transform );
    tag.appendChild ( *element );
    delete element;
  }

  int number = plotter.getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    DataRep * rep = plotter.getDataRep ( i );

    XmlElement * element = m_datarep->createElement ( *rep );

    tag.appendChild ( *element );
    delete element;
  }

  if ( typeid ( plotter ) == typeid ( Cut1DPlotter ) ||
       typeid ( plotter ) == typeid ( Cut2DPlotter ) ) {
    try {
      const CutPlotter & cut_plotter 
	= dynamic_cast < const CutPlotter & > ( plotter );

      createCutChildren ( tag, cut_plotter );
    }
    catch ( ... ) {
      // not a cut, so do nothing
    }
  }

  if ( plotter.name() == "TextPlotter" ) {
    try {
      const TextPlotter & text_plotter 
      = dynamic_cast < const TextPlotter & > ( plotter );
      createTextChildren ( tag, text_plotter );
    }
    catch ( ... ) {
      // not a text plotter, so do nothing
    }
  }
  return;
}

void PlotterBaseXML::createCutChildren ( XmlElement & tag,
					 const CutPlotter & plotter )
{
  const vector < TupleCut > & cuts = plotter.getCuts ();
  for ( unsigned int i = 0; i < cuts.size (); i++ ) {
    XmlElement * element = m_tuple_cut_xml -> createElement ( i, cuts[i] );
    tag.appendChild ( *element ); // cut properties
    delete element;
  }

  const list < DataRep * > & targets = plotter.getCutTargets ();
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
    list < DataRep * >::const_iterator first = targets.begin();

  for ( ; first != targets.end(); ++first ) {
    DataRep * rep = *first;
    const void * addr = reinterpret_cast < const void * > ( rep );
    int id = m_controller -> getId ( addr );
    XmlElement * element 
      = XmlController::m_xml_doc->createElement ( "CutTarget" );
    element->setAttribute ( "id", id );
    tag.appendChild ( *element );
    delete element;
  }
}

void
PlotterBaseXML::
createTextChildren ( XmlElement & tag,
		     const TextPlotter & plotter )
{
  const DataRep * rep = plotter.getParentDataRep ();

  const void * addr = reinterpret_cast < const void * > ( rep );
  int id = m_controller -> getId ( addr );
  XmlElement * element 
    = XmlController::m_xml_doc->createElement ( "TextTarget" );
  element->setAttribute ( "id", id );

  tag.appendChild ( *element );
  delete element;
}

void
PlotterBaseXML::
createFontElements ( XmlElement & tag,
		     const XyPlotter & plotter )
{
  const FontBase * font = plotter.titleFont ();
  if ( font != 0 ) {
    XmlElement * element = m_font -> createElement ();
	const string t ( "t" );
    element -> setAttribute ( m_axis, t );
    m_font -> setAttributes ( *element, *font );
    tag.appendChild ( *element );
    delete element;
  }

  for ( unsigned int i = 0; i < 3; i++ ) {
    Axes::Type type = Axes::convert ( i );
    createFontElement ( tag, plotter, type );
  }
}

void
PlotterBaseXML::
createFontElement ( XmlElement & tag,
		    const XyPlotter & plotter,
		    hippodraw::Axes::Type axis )
{
  const FontBase * font = plotter.labelFont ( axis );
  if ( font != 0 ) {
    XmlElement * element = m_font -> createElement ();
    string s;
    switch ( axis )
      {
      case Axes::X : 
	s = "x";
	break;
      case Axes::Y :
	s = "y";
	break;
      case Axes::Z :
	s = "z";
	break;
      default:
	assert ( false );
	break;
      }
    element -> setAttribute ( m_axis, s );
    m_font -> setAttributes ( *element, *font );

    tag.appendChild ( *element );
    delete element;
  }
}

void
PlotterBaseXML::
createAxisModel ( XmlElement & tag, 
		  const PlotterBase & plotter,
		  hippodraw::Axes::Type axis )
{
    const AxisModelBase * model = plotter.getAxisModel ( axis );
    if ( model == 0 ) return;

    XmlElement * element = m_axismodel->createElement ();
    string tmp;
    if ( axis == Axes::X ) {
      tmp = "x";
    }
    else if ( axis == Axes::Y ) {
      tmp = "y";
    }
    else if ( axis == Axes::Z ) {
      tmp = "z";
    }
    element -> setAttribute ( m_axis, tmp );
    m_axismodel->setAttributes ( *element, *model );
    tag.appendChild ( *element );
    delete element;
}

XmlElement * PlotterBaseXML::createElement ( const PlotterBase & plotter )
{
  XmlElement * tag = BaseXML::createElement ();
  const void * addr = reinterpret_cast < const void * > ( & plotter );
  int id = m_controller -> getId ( addr );
  setId ( *tag, id );

  tag->setAttribute ( m_type, plotter.name () );

  const string & title = plotter.getInternalTitle ();
  const string & x_label = plotter.getInternalLabel ( Axes::X );
  const string & y_label = plotter.getInternalLabel ( Axes::Y );
  const string & z_label = plotter.getInternalLabel ( Axes::Z );

  tag -> setAttribute ( m_title, title );
  tag -> setAttribute ( m_x_label, x_label );
  tag -> setAttribute ( m_y_label, y_label );
  tag -> setAttribute ( m_z_label, z_label );

  PlotterBase * parent = plotter.getParentPlotter ();
  if ( parent != 0 ) {
    const void * addr = reinterpret_cast < const void * > ( parent );
    int ref = m_controller -> getId ( addr );
    tag -> setAttribute ( "ref", ref );
    int index = plotter.getParentDataRepIndex ( );
    tag -> setAttribute ( m_pindex, index );
  }

  createChildren ( *tag, plotter );

  return tag;
}

PlotterBase * PlotterBaseXML::getObject ( const XmlElement * plot_element )
{
  PlotterBase * plotter = createPlotter ( plot_element );

  if ( plotter == 0 ) return 0;

  const XmlElement * element = m_transform_xml->getNode ( plot_element );
  if ( element != 0 ) {
    TransformBase * transform = m_transform_xml->createObject ( element );
    plotter->setTransform ( transform );
  }

  createAxisModels ( plot_element, plotter );
  createFontObjects ( plot_element, plotter );

  list < XmlElement * > nodelist;
  m_datarep->fillNodeList ( plot_element, nodelist );

  FunctionController * controller = FunctionController::instance();
  DataRep * rep = 0;

#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  list < XmlElement * > ::const_iterator first = nodelist.begin ();
  for ( ; first != nodelist.end(); ++first ) {
    XmlElement * element = *first;
    int id = element->getID ();
    rep = m_controller->getDataRep ( id );
    if ( rep == 0 ) {
      string what ( "Unable to find data representation" );
      throw DataRepException ( what );
    }

    assert ( rep );
    handleFunction ( element, rep );
    controller->addDataRep ( plotter, rep );
  }

  CutPlotter * cutplotter = dynamic_cast < CutPlotter * > ( plotter );
  if ( cutplotter != 0 ) {
    cutplotter -> addTupleCut ( rep );
    handleCutPlotter ( plot_element, cutplotter );
  }

  TextPlotter * textplotter = dynamic_cast < TextPlotter * > ( plotter );
  if ( textplotter != 0 ) {
    if ( ! ( handleTextPlotter ( plot_element, textplotter ) ) ){
      return 0;
    }
  }

  return plotter;
}

void
PlotterBaseXML::
createFontObjects ( const XmlElement * plot_element, PlotterBase * plotter )
{
  list < XmlElement * > nodelist;
  m_font -> fillNodeList ( plot_element, nodelist );
  if ( nodelist.empty () == false ) {
    XyPlotter * xypl = dynamic_cast < XyPlotter * > ( plotter );
    assert ( xypl );

    list < XmlElement * > ::const_iterator first = nodelist.begin ();

    while ( first != nodelist.end () ) {
      XmlElement * element = *first++;
      FontBase * font = m_controller -> createFont ();
      m_font -> setAttributes ( element, font );
      Axes::Type axis = m_font -> getAxis ( element, m_axis );
      if ( axis == Axes::T ) {
	xypl -> setTitleFont ( font );
      }
      else {
	xypl -> setLabelFont ( font, axis );
      }
    }
  }
}

void 
PlotterBaseXML::
createAxisModels ( const XmlElement * pl_element, PlotterBase * plotter )
{
  list < XmlElement * > nodelist;
  m_axismodel->fillNodeList ( pl_element, nodelist );
  if ( nodelist.empty () == false  ) {

#ifdef ITERATOR_MEMBER_DEFECT
    std::
#endif
    list < XmlElement * > :: const_iterator first = nodelist.begin();
    for ( ; first != nodelist.end (); ++first ) {
      XmlElement * element = *first;
      Axes::Type axis = m_axismodel->getAxis ( element, m_axis );
      if ( axis == Axes::Z ) plotter -> setEnableZ ( true );
      AxisModelBase * model = plotter->getAxisModel ( axis );

      if ( m_axismodel->isLog ( element ) == true ) {
	AxisLoc label = model->getLabelLocation();
	AxisLoc scale = model->getScaleLocation ();
	AxisModelBase * tmp = new AxisModelLog ( label, scale );
	std::swap ( tmp, model );
	delete tmp;
	plotter->setAxisModel ( model, axis );
      } // log case

      m_axismodel->setAttributes ( model, element );
    }
  }
}

PlotterBase * PlotterBaseXML::createPlotter ( const XmlElement * element )
{
  string type;
  bool ok = element->attribute ( m_type, type );
  assert ( ok );

  bool has_Z = type == "XYColorPlotter";
  if ( type == "XYPlotter" ||
       type == "XYColorPlotter" ) { // old names of current class
    type = "XyPlotter";
  }
  PlotterBase * plotter = 0;
  PlotterFactory * factory = PlotterFactory::instance ();
  try {
    plotter = factory->create ( type );
    if ( has_Z ) plotter ->setEnableZ ();
    int id = element -> getID ();
    m_controller -> registerPlotter ( id, plotter );

    string value;
    bool needMargin;

    ok = element -> attribute ( m_title, value );
    plotter -> setTitle ( value );
    needMargin = String::ci_find(value, "tex:")==0;
    plotter->setTopMargin(needMargin?10.0:0.0);

    ok = element -> attribute ( m_x_label, value );
    plotter -> setLabel ( Axes::X, value );
    needMargin = String::ci_find(value, "tex:")==0;
    plotter->setBottomMargin(needMargin?8.0:0.0);

    ok = element -> attribute ( m_y_label, value );
    plotter -> setLabel ( Axes::Y, value );
    needMargin = String::ci_find(value, "tex:")==0;
    plotter->setLeftMargin(needMargin?0.0:0.0);

    ok = element -> attribute ( m_z_label, value );
    plotter -> setLabel ( Axes::Z, value );
    needMargin = String::ci_find(value, "tex:")==0;
    plotter->setZMargin(needMargin?7.0:0.0);

    int index;
    ok = element -> attribute ( m_pindex, index );
    if ( ok ) {
      plotter -> setParentDataRepIndex ( index );
    }
  }
  catch ( const FactoryException & ) {
    assert ( false );
  }

  return plotter;
}

void 
PlotterBaseXML::
getCutTargets ( const XmlElement * plot_element, CutPlotter * plotter )
{
  list < XmlElement * > nodelist;
  plot_element->fillNodeList ( "CutTarget", nodelist );
  if ( nodelist.empty() ) return;

#ifdef ITERATOR_MEMBER_DEFECT
    std::
#endif
    list < XmlElement * > :: const_iterator first = nodelist.begin();
    for ( ; first != nodelist.end(); ++first ) {
        XmlElement * element = *first;
	int ref = element->getID ();
	DataRep * target = m_controller->getDataRep ( ref );
	if ( target != 0 ) { // may not be part of copy/paste
	  ProjectorBase * pb = target -> getProjector ();
	  NTupleProjector * projector 
	    = dynamic_cast < NTupleProjector * > ( pb );

	  const vector < TupleCut > & cuts = plotter -> getCuts ();
	  for ( unsigned int i = 0; i < cuts.size(); i++ ) {
	    projector -> addCut ( &cuts[i] );
	  }
	  target -> setDirty ( true );
	  plotter->addCutTarget ( target );
	}
    }
}

/** @todo Should this be handled by FunctionProjectorXML?
 */
void PlotterBaseXML::handleFunction ( const XmlElement * dr_element,
				      DataRep * rep )
{
  FunctionRep * frep = dynamic_cast < FunctionRep * > ( rep );
  if ( frep == 0 ) return;

  XmlElement * element = dr_element->getNode ( "FunctionTarget" );
  int id = element->getID ();
  DataRep * target = m_controller->getDataRep ( id );
  assert ( target );
  frep->setTarget ( target );
}

/** @bug Old documents, before intro of function cuts, can not be read
    as they will be missing the cut representation.
 */
void 
PlotterBaseXML::
handleCutPlotter ( const XmlElement * plot_element, CutPlotter * plotter )
{
  vector < const TupleCut * > cuts;
//   const XmlElement * element = m_tuple_cut_xml->getNode ( plot_element );

  list < XmlElement * > node_list;
  m_tuple_cut_xml -> fillNodeList ( plot_element, node_list );
  list < XmlElement * >::const_iterator first = node_list.begin();

  while ( first != node_list.end() ) {
    const XmlElement * element = *first++;
    unsigned int a = 0;
  // bool ok = 
    element -> attribute ( "axis", a );
    Axes::Type axis = Axes::convert ( a );
    const string & label = plotter -> getLabel ( axis );

    int id = element->getID ();
    const TupleCut * tuplecut = m_controller->getTupleCut ( id );
    TupleCut * tc = const_cast < TupleCut * > ( tuplecut );
    tc -> setLabel ( label );
    cuts.push_back ( tuplecut );
    tuplecut = m_controller -> getTupleCut ( -id ); // old multiDim
    if ( tuplecut != 0 ) {
      cuts.push_back ( tuplecut );
    }
  }
  plotter->setCuts ( cuts );

  getCutTargets ( plot_element, plotter );
}

int
PlotterBaseXML::
handleTextPlotter ( const XmlElement * plot_element, TextPlotter * plotter )
{

  XmlElement * element = plot_element->getNode ( "TextTarget" );
  int id = element->getID ();
  DataRep * target = m_controller->getDataRep ( id );
  
  if ( !target ){
    return 0;
  }

  assert ( target );
  plotter->setParentDataRep ( target );

  return 1;
}
