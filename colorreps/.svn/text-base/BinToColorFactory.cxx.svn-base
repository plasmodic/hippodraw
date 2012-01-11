/** @file

BinToColorFactory class implementation

Copyright (C) 2002-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToColorFactory.cxx,v 1.28 2007/02/16 18:46:52 pfkeb Exp $

*/

#ifdef _MSC_VER
// A long identifier warning.
#include "msdevstudio/MSconfig.h"
#endif

#include "BinToColorFactory.h"

// A list of default BinToColors.
#include "BinToBeach.h"
#include "BinToBlack.h"
#include "BinToBluePastel.h"
#include "BinToBlueRed.h"
#include "BinToBlueWhite.h"
#include "BinToEosA.h"
#include "BinToEosB.h"
#include "BinToBWLinear.h"
#include "BinToRainbow.h"
#include "BinToHardCandy.h"
#include "BinToKamaeScale.h"
#include "BinToHaze.h"
#include "BinToHueSatLight1.h"
#include "BinToHueSatLight2.h"
#include "BinToHueSatValue1.h"
#include "BinToHueSatValue2.h"
#include "BinTo3ColorScale.h"
#include "BinToGreyScale.h"
#include "BinToMovingBrkPt.h"
#include "BinToColorScaleMovingBrkPt.h"
#include "BinToFullColorScale.h"
#include "BinToGRBW.h"
#include "BinToMacStyle.h"
#include "BinToNature.h"
#include "BinToOcean.h"
#include "BinToPastels.h"
#include "BinToPeppermint.h"
#include "BinToRedPurple.h"
#include "BinToRedTemp.h"
#include "BinToStern.h"
#include "BinToUsrDefinedScale.h"
#include "BinToGamma.h" 
#include "BinToGWExpon.h"
#include "BinToGWLinear.h"
#include "BinToGPink.h"
#include "BinToLineStyle.h"
#include "BinToPrism.h"
#include "BinToPurpleRed.h"
#include "BinToRainbowIDL.h"
#include "BinToBGRY.h"
#include "BinToStdGamma2.h"

namespace hippodraw {

BinToColorFactory * BinToColorFactory::s_instance = 0;

BinToColorFactory::BinToColorFactory ()
{
}

BinToColorFactory * BinToColorFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new BinToColorFactory ();
    s_instance->initialize ();
  }
  return s_instance;
}

void BinToColorFactory::initialize ()
{
  add ( new BinToBeach ( "Beach" ) );
  add ( new BinToRainbow ( "Rainbow" ) );
  add ( new BinToEosA (  "Eos A" ) );
  add ( new BinToEosB (  "Eos B" ) );
  add ( new BinToHardCandy ( "Hard Candy" ) );
  add ( new BinToKamaeScale ( "Kamae" ) );
  add ( new BinTo3ColorScale ( "3 Color" ) );
  add ( new BinToGreyScale ( "Grey scale" ) );
  add ( new BinToColorScaleMovingBrkPt ( "Variable rainbow" ) );
  add ( new BinToMovingBrkPt ( "Variable 3 color" ) );
  add ( new BinToFullColorScale ( "Full Rainbow" ) );
  add ( new BinToGRBW ( "Grn-Red-Blue-White" ) );
  add ( new BinToBlack ( "All black" ) );
  add ( new BinToBluePastel ( "Blue- Pastel-Red" ) );
  add ( new BinToBlueRed ( "Blue-Red" ) );
  add ( new BinToBWLinear ( "Black-White" ) );
  add ( new BinToBlueWhite ( "Blue-White" ) );
  add ( new BinToPrism ( "Prism" ) );
  add ( new BinToGamma ( "Gamma Correction" ) );
  add ( new BinToHaze ( "Haze" ) );
  add ( new BinToPastels ( "Pastels" ) );
  add ( new BinToPeppermint ( "Peppermint" ) );
  add ( new BinToOcean ( "Ocean" ) );
  add ( new BinToGPink ( "Green-Pink" ) );
  add ( new BinToGWExpon ( "Green-White Exp." ) );
  add ( new BinToGWLinear ( "Green-White Lin." ) );
  add ( new BinToHueSatLight1 ( "Hue Sat. Lightness 1" ) );
  add ( new BinToHueSatLight2 ( "Hue Sat. Lightness 2" ) );
  add ( new BinToHueSatValue1 ( "Hue Sat. Value 1" ) );
  add ( new BinToHueSatValue2 ( "Hue Sat. Value 2" ) );
  add ( new BinToMacStyle ( "Mac Style" ) );
  add ( new BinToNature ( "Nature" ) );
  add ( new BinToLineStyle ( "Line Style") );
  add ( new BinToPurpleRed ( "Purple - Red + Strips" ) );
  add ( new BinToRainbowIDL ( "Rainbow IDL" ) );
  add ( new BinToRedPurple ( "Red-Purple" ) );
  add ( new BinToRedTemp ( "Red Temperature" ) );
  add ( new BinToStern ( "Stern Special" ) );
  add ( new BinToBGRY ( "Blue-Green-Red-Yellow" ) );
  add ( new BinToStdGamma2 ("Std Gamma II" ) );
}

} // namespace hippodraw
