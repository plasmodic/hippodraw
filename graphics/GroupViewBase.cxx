/** @file

hippodraw::GroupViewBase class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: GroupViewBase.cxx,v 1.3 2006/05/04 23:09:40 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "GroupViewBase.h"

#include <algorithm>

using std::vector;

using namespace hippodraw;

GroupViewBase::GroupViewBase ()
  :ViewBase(0)
{
}

GroupViewBase::GroupViewBase ( const std::vector < ViewBase * > & inViews )
{
  mViews = inViews;
}

GroupViewBase::GroupViewBase ( const GroupViewBase & src )
{
}

GroupViewBase::~GroupViewBase ()
{
}

const vector < PlotterBase * >
GroupViewBase::
getPlotters () const
{
  vector < PlotterBase * > plotters;
  for ( unsigned int i = 0; i<mViews.size(); i++ ) {
    ViewBase * view = mViews[i];
    PlotterBase * plotter = view -> getPlotter ();
    if ( plotter !=0 ) {
      plotters.push_back ( plotter );
    }
  }
  return plotters;
}
  

const vector < ViewBase * > & 
GroupViewBase::
getViews ()
{
  return mViews;
}

void GroupViewBase::addView ( ViewBase * inView )
{
  mViews.push_back( inView );
}

void GroupViewBase::removeView ( ViewBase * inView )
{
  vector < ViewBase * >::iterator last
    = std::remove( mViews.begin(), mViews.end(), inView );
  mViews.erase(last, mViews.end());
}


