/** @file

hippodraw::ContourPointRep class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ContourPointRep.cxx,v 1.66 2007/04/09 18:21:58 pfkeb Exp $
 
*/
// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ContourPointRep.h"

#include "axes/AxisModelBase.h"
#include "colorreps/BinToColor.h"
#include "colorreps/BinToColorFactory.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/NTupleSorter.h"
#include "graphics/DataView.h"
#include "projectors/NTupleProjector.h"
#include "transforms/PeriodicBinaryTransform.h"

#include <cassert>
#include <cmath>

using namespace hippodraw;

using std::max;
using std::min;
using std::vector;

ContourPointRep::ContourPointRep ( float size )
  : LinePointRep ( size )
{
  init ();
}

ContourPointRep::ContourPointRep ( )
  : LinePointRep ( )
{
  init();
}

void
ContourPointRep::
init ()
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  m_bin_to_color = factory -> create ( "Rainbow" );
  m_name = "Contour"; // need to override what base class did
  m_numContours = 6;
  m_usingUserValues = false;
  m_values.clear();
  m_values.reserve ( m_numContours );
}

ContourPointRep::ContourPointRep( const ContourPointRep & rep )
  : LinePointRep ( rep )
{
  BinToColor * btc = rep.m_bin_to_color;
  m_bin_to_color = btc -> clone ();

  m_numContours = rep.getNumContours();
  m_usingUserValues = rep.getUsingUserValues();
  m_values.clear();
  m_values.reserve ( m_numContours );
}

ContourPointRep::~ContourPointRep()
{
  delete m_bin_to_color;
}

RepBase * ContourPointRep::clone()
{
  return new ContourPointRep ( *this );
}

const BinToColor *
ContourPointRep::
getValueTransform ( ) const
{
  return m_bin_to_color;
}

void
ContourPointRep::
setValueTransform ( BinToColor * btc )
{
  delete m_bin_to_color;
  m_bin_to_color = btc;
}

void ContourPointRep::drawValuesWithStyle ( const TransformBase & tf,
					    ViewBase & vb )
{
  // Draw Contour Ticks

  drawContourTicks ( tf, vb, m_values );
  
  // If number of contour level > 20, simply draw segments
  if (m_numContours>20){
    try { 	 
      const BinaryTransform & t 	    
	= dynamic_cast < const BinaryTransform & > ( tf ); 	     
      
      t.transform ( m_x, m_y ); 	 
      const Rect & user_rect = vb.getUserRect ();
      user_rect.makeInBounds ( m_x, m_y ); 	 
      
      vb.drawColorLines ( m_x, m_y, m_line_style, m_colorvec, m_size ); 	 
    
    }
    catch ( ... ) {
      assert ( false );
    }
  }

  else {

  // Temporary vectors.
  vector <double> temp_x;
  vector <double> temp_y; 
  vector <Line::Style> temp_s;

  // Current point. ( for line style, only one color used )
  Color curC=m_colorvec[0]; 
  Line::Style curS;
  double curX, curY;

  // This flag is set to true when a following segment is found.
  bool found=false;

  // Create a vectors for each loop (each contour level may have
  // more than one loops), and draw the polyline
  while (!m_x.empty()){
    temp_x.clear();
    temp_y.clear();
    temp_x.push_back(m_x[0]);
    temp_x.push_back(m_x[1]);
    temp_y.push_back(m_y[0]);
    temp_y.push_back(m_y[1]);
    curX = m_x[1];
    curY = m_y[1];
    curS = m_stylevec[1];

    m_x.erase(m_x.begin(), m_x.begin()+2);
    m_y.erase(m_y.begin(), m_y.begin()+2);
    m_stylevec.erase(m_stylevec.begin(), m_stylevec.begin()+2);
   

    // Exit the when a loop is formed.
    while(curX!=temp_x[0] || curY!=temp_y[0]){
      for (unsigned int i=0; i<m_x.size(); i++) {
	// A following segment is found.
	if (m_x[i]==curX && m_y[i]==curY && m_stylevec[i]==curS){
	  found=true;
	  // Add the segment to the current loop
	  if (i%2==0){
	    curX=m_x[i+1];
	    curY=m_y[i+1];
	    temp_x.push_back(curX);
	    temp_y.push_back(curY);
	    m_x.erase(m_x.begin()+i, m_x.begin()+i+2);
	    m_y.erase(m_y.begin()+i, m_y.begin()+i+2);
	    m_stylevec.erase(m_stylevec.begin()+i, m_stylevec.begin()+i+2);
	  } else {
	    curX=m_x[i-1];
	    curY=m_y[i-1];
	    temp_x.push_back(curX);
	    temp_y.push_back(curY);
	    m_x.erase(m_x.begin()+i-1, m_x.begin()+i+1);
	    m_y.erase(m_y.begin()+i-1, m_y.begin()+i+1);
	    m_stylevec.erase(m_stylevec.begin()+i-1, m_stylevec.begin()+i+1);
	  }
	  break;
	}
      }
      // If a following segment cannot be found, break to avoid infinite loop
      if (found) {found=false; continue;}
      else break;
    }
  

    // Draw Lines.
    try {
      const BinaryTransform & t 
	= dynamic_cast < const BinaryTransform & > ( tf );
      t.transform (temp_x, temp_y);
      const Rect & user_rect = vb.getUserRect ();
      user_rect.makeInBounds (temp_x, temp_y);
      vb.drawPolyLine(temp_x, temp_y, curS, curC, m_size);
      
    }
    catch ( ... ) {
      assert ( false );
    }

  } // Back to while(!m_x.empty()), draw next polyline.

  }

}

void ContourPointRep::drawValues ( const TransformBase & tf, 
				   ViewBase & vb )
{
  // Draw Contour Ticks

  drawContourTicks ( tf, vb, m_values );

  // If number of contour level > 20, simply draw segments
  if (m_numContours>20){
    try { 	 
      const BinaryTransform & t 	    
	= dynamic_cast < const BinaryTransform & > ( tf ); 	     
      
      t.transform ( m_x, m_y ); 	 
      const Rect & user_rect = vb.getUserRect ();
      user_rect.makeInBounds ( m_x, m_y ); 	 
      
      vb.drawColorLines ( m_x, m_y, m_line_style, m_colorvec, m_size ); 	 
    
    }
    catch ( ... ) {
      assert ( false );
    }
  }

  else {

  // Temporary vectors.
  vector <double> temp_x;
  vector <double> temp_y; 
  vector <Color> temp_c;

  // Current point.
  Color curC;
  double curX, curY;

  // This flag is set to true when a following segment is found.
  bool found=false;

  // Create a vectors for each loop (each contour level may have
  // more than one loops), and draw the polyline
  while (!m_x.empty()){
    temp_x.clear();
    temp_y.clear();
    temp_x.push_back(m_x[0]);
    temp_x.push_back(m_x[1]);
    temp_y.push_back(m_y[0]);
    temp_y.push_back(m_y[1]);
    curX = m_x[1];
    curY = m_y[1];
    curC = m_colorvec[1];

    m_x.erase(m_x.begin(), m_x.begin()+2);
    m_y.erase(m_y.begin(), m_y.begin()+2);
    m_colorvec.erase(m_colorvec.begin(), m_colorvec.begin()+2);
   

    // Exit the when a loop is formed.
    while(curX!=temp_x[0] || curY!=temp_y[0]){
      for (unsigned int i=0; i<m_x.size(); i++) {
	// A following segment is found.
	if (m_x[i]==curX && m_y[i]==curY && m_colorvec[i]==curC){
	  found=true;
	  // Add the segment to the current loop
	  if (i%2==0){
	    curX=m_x[i+1];
	    curY=m_y[i+1];
	    temp_x.push_back(curX);
	    temp_y.push_back(curY);
	    m_x.erase(m_x.begin()+i, m_x.begin()+i+2);
	    m_y.erase(m_y.begin()+i, m_y.begin()+i+2);
	    m_colorvec.erase(m_colorvec.begin()+i, m_colorvec.begin()+i+2);
	  } else {
	    curX=m_x[i-1];
	    curY=m_y[i-1];
	    temp_x.push_back(curX);
	    temp_y.push_back(curY);
	    m_x.erase(m_x.begin()+i-1, m_x.begin()+i+1);
	    m_y.erase(m_y.begin()+i-1, m_y.begin()+i+1);
	    m_colorvec.erase(m_colorvec.begin()+i-1, m_colorvec.begin()+i+1);
	  }
	  break;
	}
      }
      // If a following segment cannot be found, break to avoid infinite loop
      if (found) {found=false; continue;}
      else break;
    }
  

  // Draw Lines.
    try {
      const BinaryTransform & t 
	= dynamic_cast < const BinaryTransform & > ( tf );
      t.transform (temp_x, temp_y);
      const Rect & user_rect = vb.getUserRect ();
      user_rect.makeInBounds (temp_x, temp_y);
      vb.drawPolyLine(temp_x, temp_y, m_line_style, curC, m_size);
      
    }
    catch ( ... ) {
      assert ( false );
    }

  } // Back to while(!m_x.empty()), draw next polyline.

  }
}


/** @todo This method should be in AxisRepColor class.
 */
void
ContourPointRep::
drawContourTicks ( const TransformBase & tb,
		   ViewBase & base,
		   const std::vector < double > & values )
{
  vector< double > xv;
  vector< double > yv;
  
  unsigned int size = values.size ();
  xv.reserve ( size );
  yv.reserve ( size );

  DataView & view = dynamic_cast < DataView & > ( base );
  const Range & range = view.getRange ( Axes::Z );

  const BinaryTransform & bt 
    = dynamic_cast < const BinaryTransform & > ( tb );

  const Rect & view_rect = view.getMarginRect ();
 
  double tick_length = 6.0;
  double y_base =  view_rect.getY() - 15.0;


  for ( unsigned int i = 0; i < values.size(); i++ ) {
    
    double user_z = values[i];

    if ( range.excludes ( user_z ) ) continue;

    bt.transformZ ( user_z );
    
    double view_x = view.userToDrawColor ( user_z );

    xv.push_back ( view_x );
    yv.push_back ( y_base );
    xv.push_back ( view_x );
    yv.push_back ( y_base + tick_length );
  
  }

  view.drawViewLines ( xv, yv, Line::Solid, false, 0 );
}

namespace dp = hippodraw::DataPoint3DTuple;

void
ContourPointRep::
createContours (  const DataSource * ntuple,
		  const TransformBase * transform )
{
  int size = m_values.size();
  assert ( size == m_numContours );

  if ( ntuple -> rows () == 0 ) return;

  double      h [5];
  int         sh [5];
  double      xh [5];
  double      yh [5];
  int         m1;
  int         m2;
  int         m3;
  int         case_value;
  double      dmin;
  double      dmax;
  double      x1 = 0.0;
  double      x2 = 0.0;
  double      y1 = 0.0;
  double      y2 = 0.0;
  unsigned int i,j;
  int         k,m;
       
  int im[4] = {0,1,1,0};
  int jm[4] = {0,0,1,1};
        
  // Castab = Case Table.
  
  // castab[0][0][0] ==> all three points of the triangle are below
  // the plane of the contour.
  // 1 ==> in the plane of the contour.
  // 2 ==> above the plane of the contour.
  
  int castab [3][3][3] =
    {
      {
	{0,0,8},{0,2,5},{7,6,9}
      },
      {
	{0,3,4},{1,0,1},{4,3,0}
      },
      {
	{9,6,7},{5,2,0},{8,0,0}
      }
    };
  const vector < unsigned int > & shape = ntuple -> getShape ();
  unsigned int i_size = shape[0];  
  unsigned int j_size = shape[1];  
  vector < unsigned int > index ( 3 );

  for ( i = 0; i < i_size - 1; i++ ) {

    for ( j = 0; j < j_size - 1; j++ ) {
      // For box given by (i,j), (i,j+1), (i+1,j), (i+1,j+1),
      // find max and min value.

      double temp1,temp2;
      index[0] = i;
      index[1] = j;
      index[2] = dp::Z;
      double tempij = ntuple -> operator [] ( index );

      index[1] = j+1;
      double tempij1 = ntuple -> operator [] ( index );

      temp1 = min ( tempij, tempij1 );

      index[0] = i+1;
      index[1] = j;
      double tempi1j = ntuple -> operator[] ( index );

      index[1] = j+1;
      double tempi1j1 = ntuple -> operator [] ( index );
      temp2 = min ( tempi1j, tempi1j1 );

      dmin  = min ( temp1, temp2 );
      
      temp1 = max ( tempij, tempij1 );
      temp2 = max ( tempi1j, tempi1j1 );
      dmax  = max ( temp1, temp2 );
  
      if ( dmax >= m_values [0] &&
	   dmin <= m_values [m_numContours-1] ) {
	
	for ( k = 0; k < m_numContours; k++ ) {
	  
	  double curContour = m_values[k];
	  
	  if ( curContour >= dmin &&
	       curContour <= dmax ) {

	    // Contour level k passes through the box.
	    
	    for ( m = 4; m >= 0; m--) {
	      
	      if ( m > 0 ) {
		
		index[0] = i + im[m-1];
		index[1] = j + jm[m-1];
		index[2] = dp::Z;
		h[m] = ntuple -> operator [] ( index ) - curContour;

		index[2] = dp::X;
		xh[m] = ntuple -> operator [] ( index );

		index[2] = dp::Y;
		yh[m] = ntuple -> operator [] ( index );

	      } 
	      
	      else {
		
		h[0]  = 0.25 * ( h[1] + h[2] + h[3] + h[4] );
		index[0] = i;
		index[1] = j;
		index[2] = dp::X;
		double xij = ntuple -> operator [] ( index );

		index [0] = i+1;
		double xi1j = ntuple -> operator [] ( index );

		index[0] = i;
		index[1] = j;
		index[2] = dp::Y;
		double yij = ntuple -> operator [] ( index );

		index [1] = j+1;
		double yij1 = ntuple -> operator [] ( index );

		xh[0] = 0.50 * ( xij + xi1j );
		yh[0] = 0.50 * ( yij + yij1 );
	      }
	      
	      if ( h[m] > 0.0 ) {
		sh[m] = 1;
	      } 

	      else if ( h[m] < 0.0 ) {
		sh[m] = -1;
	      } 

	      else {
		sh[m] = 0;
	      }
	    
	    }

// 	    // Note: at this stage the relative heights of the corners and the
// 	    // centre are in the h array, and the corresponding coordinates are
// 	    // in the xh and yh arrays. The centre of the box is indexed by 0
// 	    // and the 4 corners by 1 to 4 as shown below.
// 	    // Each triangle is then indexed by the parameter m, and the 3
// 	    // vertices of each triangle are indexed by parameters m1,m2,and
// 	    // m3.
// 	    // It is assumed that the centre of the box is always vertex 2
// 	    // though this isimportant only when all 3 vertices lie exactly on
// 	    // the same contour level, in which case only the side of the box
// 	    // is drawn.
// 	    //
// 	    //
// 	    //      vertex 4 +-------------------+ vertex 3
// 	    //               | \               / |
// 	    //               |   \    m=3    /   |
// 	    //               |     \       /     |
// 	    //               |       \   /       |
// 	    //               |  m=4    0   m=2   |       the centre is vertex 0
// 	    //               |       /   \       |
// 	    //               |     /       \     |
// 	    //               |   /    m=1    \   |
// 	    //               | /               \ |
// 	    //      vertex 1 +-------------------+ vertex 2
	    
	    for (m=1;m<=4;m++) {
	      
	      m1 = m;
	      m2 = 0;
	      if (m!=4) {
		m3 = m+1;
	      } else {
		m3 = 1;
	      }
	      
	      case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1];
	      
	      if (case_value!=0) {
		switch (case_value) {
		case 1: // Line between vertices 1 and 2
		  x1=xh[m1];
		  y1=yh[m1];
		  x2=xh[m2];
		  y2=yh[m2];
		  break;
		case 2: // Line between vertices 2 and 3
		  x1=xh[m2];
		  y1=yh[m2];
		  x2=xh[m3];
		  y2=yh[m3];
		  break;
		case 3: // Line between vertices 3 and 1
		  x1=xh[m3];
		  y1=yh[m3];
		  x2=xh[m1];
		  y2=yh[m1];
		  break;
		case 4: // Line between vertex 1 and side 2-3
		  x1=xh[m1];
		  y1=yh[m1];
		  x2=intersect(m2,m3,h,xh);
		  y2=intersect(m2,m3,h,yh);
		  break;
		case 5: // Line between vertex 2 and side 3-1
		  x1=xh[m2];
		  y1=yh[m2];
		  x2=intersect(m3,m1,h,xh);
		  y2=intersect(m3,m1,h,yh);
		  break;
		case 6: //  Line between vertex 3 and side 1-2
		  x1=xh[m3];
		  y1=yh[m3];
		  x2=intersect(m1,m2,h,xh);
		  y2=intersect(m1,m2,h,yh);
		  break;
		case 7: // Line between sides 1-2 and 2-3
		  x1=intersect(m1,m2,h,xh);
		  y1=intersect(m1,m2,h,yh);
		  x2=intersect(m2,m3,h,xh);
		  y2=intersect(m2,m3,h,yh);
		  break;
		case 8: // Line between sides 2-3 and 3-1
		  x1=intersect(m2,m3,h,xh);
		  y1=intersect(m2,m3,h,yh);
		  x2=intersect(m3,m1,h,xh);
		  y2=intersect(m3,m1,h,yh);
		  break;
		case 9: // Line between sides 3-1 and 1-2
		  x1=intersect(m3,m1,h,xh);
		  y1=intersect(m3,m1,h,yh);
		  x2=intersect(m1,m2,h,xh);
		  y2=intersect(m1,m2,h,yh);
		  break;
		default:
		  break;
		}
		
		const Range r = m_bin_to_color->getRange();
		double val = curContour;
		
		const BinaryTransform * bt 
		  = dynamic_cast < const BinaryTransform * > ( transform );
		bt -> transformZ ( val );

		if ( ! r.excludes ( val ) ) {
		  
		  m_x.push_back ( x1 );
		  m_x.push_back ( x2 );
		  m_y.push_back ( y1 );
		  m_y.push_back ( y2 );
		
		  // Pushing twice just to ensure symmetry.
		  Color color = m_color;
		  if ( m_desel ) { // deselected
		    color = s_desel_color;
		  }
		  else {
		    m_bin_to_color -> doubleToColor ( val, color );
		  }
		  m_colorvec.push_back ( color );
		  m_colorvec.push_back ( color );

		  // For drawing different contour level with different
		  // line style. Use 3 line styles for now.
		  if ( m_bin_to_color -> name() == "Line Style" ){
		    unsigned int style = ( m_numContours - 1 - k ) % 3;
		    Line::Style lineStyle = Line::convert(style);
		    m_stylevec.push_back ( lineStyle );
		    m_stylevec.push_back ( lineStyle );
		  }

		}
	      }
	    }
	  }
	}
      }
    }
  } 
}

/** @todo Test contour when low end of range is negative. 
 */
double ContourPointRep::getContour ( int i, const TransformBase * transform  )
{
  const BinaryTransform * t 
    = dynamic_cast < const BinaryTransform * > ( transform );

  bool zLinear=t->isLinearInZ();
  double high=m_maxValue;
  double low;

  if (zLinear) {
    low = m_minValue;
    double width = high - low;
    low = low + 0.05 * width;
    high = high - 0.05 * width;
  }
  else{
    high = m_maxValue;
    low = m_minPos;
  }

  t -> transformZ ( high );
  t -> transformZ ( low );

  double value = low + ( 
  			( high - low ) * 
  		((double)(i)) /
  		((double)(m_numContours)) 
  		);

  t -> inverseTransformZ ( value );

  return value;

}

double ContourPointRep::intersect ( int p1, int p2, double * h, double * xh )
{
  return (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1]);
}

int ContourPointRep::getNumContours() const
{
  return m_numContours;
}

void ContourPointRep::setNumContours ( int i )
{
  m_numContours = i;
}

void ContourPointRep::setUsingUserValues ( bool flag )
{
  m_usingUserValues = flag;
}

bool ContourPointRep::getUsingUserValues () const
{
  return m_usingUserValues;
}

void
ContourPointRep::
setContourValues ( std::vector < double > & values,
		   ProjectorBase * proj )
{
  
  m_usingUserValues = true;
  m_numContours = values.size();
  m_values.clear();
  m_values.reserve ( m_numContours );

  AxisModelBase * a = proj->getAxisModel ( Axes::Z );
  assert ( a );
  
  double sf = a->getScaleFactor();
  
  vector < double > :: iterator iter = values.begin ();
  for ( ; iter != values.end (); iter++ ) {
    m_values.push_back ( (*iter)/sf );
  }
  
}

void ContourPointRep::setContourVector ( const TransformBase * transform )
{
  
  if ( m_usingUserValues ) return;
  
  m_values.clear();
  m_values.reserve ( m_numContours );

  for ( int i = 0; i < m_numContours; i++ ){
    m_values.push_back ( getContour ( i, transform ) );
  }

}

void
ContourPointRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  const BinaryTransform * bt
    = dynamic_cast < const BinaryTransform * > ( transform );

  const Range & range = view -> getRange ( Axes::Z );
  double high = range.high();
  double low = range.low(); //(bt->isLinearInZ())?range.low():range.pos();

  bt -> transformZ ( high );
  bt -> transformZ ( low );

  Range newrange ( low, high );

  m_bin_to_color->setRange ( newrange );

  m_x.clear();
  m_y.clear();
  m_colorvec.clear();

  unsigned int size = ntuple -> rows () * ntuple -> columns ();
  m_x.reserve ( size );
  m_y.reserve ( size );
  m_colorvec.reserve ( size );
  
  // m_stylevec doesn't work with other color map.
  if ( m_bin_to_color -> name() == "Line Style" ){
    m_stylevec.clear();
    m_stylevec.reserve(size);
  }


  // If periodic transform, process rotation.
  bool isPeriodic = bt->isPeriodic();


  double beta = 0;
  double gamma = 0;
  double max_x = 0;

  if ( isPeriodic )
    { 
      const PeriodicBinaryTransform * pbt
        = dynamic_cast < const PeriodicBinaryTransform * > ( bt );
      
      // Do rotation
      NTuple * newntuple = new NTuple ( ntuple );

      unsigned int rowsize = newntuple -> rows();
      unsigned int columnsize = newntuple -> columns();

      for (unsigned int i = 0; i < rowsize; i++) {
	const vector < double > & row = newntuple -> getRow (i);
	vector <double> newrow;
	vector <double> rowdata(columnsize);
	
	for (unsigned int j=0; j<columnsize; j++) {
	  rowdata[j]=row[j];
	}

	pbt->rotateByOffsets ( rowdata[1], rowdata[0] );
	for (unsigned int j=0; j<columnsize; j++) {
	  newrow.push_back(rowdata[j]);
	}
	
        newntuple -> replaceRow ( i, newrow );
      }
    
      // Sort the X column of ntuple.
      
      NTupleSorter * xsorter = new NTupleSorter(newntuple);
      xsorter->sort();
      NTuple * tempntuple = new NTuple (newntuple);

      // Sort the Y column of ntuple.
      const vector < unsigned int > & shape = newntuple -> getShape ();
      unsigned int ynum = shape [ 1 ];
      unsigned int xnum = shape [ 0 ];

      for (unsigned int i=0; i<xnum; i++){

	tempntuple -> clear();

	for (unsigned int j=0; j<ynum; j++){

	  const vector < double > & row = newntuple -> getRow (i*ynum+j);
	  tempntuple -> insertRow ( j, row );

	}

	NTupleSorter * ysorter = new NTupleSorter(tempntuple);
	ysorter -> setSorting(1);
	ysorter -> sort();

	for (unsigned int j=0; j<ynum; j++){

	  const vector < double > & row2 = tempntuple -> getRow (j);
	  newntuple -> replaceRow(i*ynum+j, row2);

	}
      }



      /*  Sort Y column alternative, actully it's not ynum*xnum matrix now.

      tempntuple->clear();
      unsigned int ptr=0;
      const vector < double > & first = newntuple -> getRow(0);
      double tmp=first[0];

      for (unsigned int i=0; i<rowsize; ){
	const vector < double > & row = newntuple -> getRow (i);
	if (row[0]==tmp){
	  tempntuple->insertRow(ptr,row);
	  ptr++;
	  i++;
	}
	if (row[0]>tmp) {
	  //tempntuple->clear();
	  NTupleSorter * ysorter = new NTupleSorter(tempntuple);
	  ysorter->setSorting(1);
	  ysorter->sort();
	  for (unsigned int j=0; j< ptr-1; j++){
	    const vector <double> & row2 = tempntuple->getRow(j);
	    newntuple -> replaceRow(i-ptr+j, row2);
	  }
	  tempntuple->clear();
	  tmp=row[0];
	  ptr=0;
	  
	}
      }
	*/


      
      setMinMax ( newntuple );
      setContourVector ( bt );
      createContours ( newntuple, bt );
      
  }
  else
  {
  // Set the min and max values.
  setMinMax ( ntuple );

  // Set the contour values.
  setContourVector ( bt );
  
  // Do the contouring algorithm.
    createContours ( ntuple, bt );
  // Now you are ready to draw when you get endplot.

  }

  if (m_bin_to_color->name()=="Line Style") {
    drawValuesWithStyle( *bt, *view );
  } else {
    drawValues ( *bt, *view );
  }

}

void
ContourPointRep::
setMinMax ( const DataSource * ntuple )
{
  const vector < double > & values = ntuple -> getColumn ( dp::Z );
  Range range ( values );

  m_minValue = range.low ();
  m_maxValue = range.high ();
  m_minPos = range.pos();
}

