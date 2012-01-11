//////////////////////////////////////////////////////////////////////////////
//
// Code extracted from freetype-1.3.1/test/ftview.c, ftstring.c.
//
//////////////////////////////////////////////////////////////////////////////

// this :
#include "TextTTF.h"

#include "OpenGL.h"

#include <iostream>

#ifdef HAVE_TTF
#include <freetype.h>

#define MAXIMUM(a,b) ((a)>(b)?a:b)
#define MINIMUM(a,b) ((a)<(b)?a:b)

class TextTTF_Internal {
public:
  TT_Error loadTrueTypeChar(int);
  bool fOpened;
  TT_Face fFace;
  TT_CharMap fCharMap;
  TT_Instance fInstance;
  TT_Instance_Metrics fInstanceMetrics;
  TT_Glyph fGlyph;
  TT_Raster_Map fBitmap;
  int fNumGlyphs;
  bool fHinted;
  bool fNewLine;
  TT_UShort fWidth;
  TT_UShort fTextWidth;
  TT_UShort fTextHeight;
  float fXJustifyTranslation;
  float fYJustifyTranslation;
};

static TT_Engine* fEngine = 0;

//////////////////////////////////////////////////////////////////////////////
TT_Error TextTTF_Internal::loadTrueTypeChar( 
 int aIndex
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //int  flags = TTLOAD_SCALE_GLYPH;
  //if (fHinted==true) flags |= TTLOAD_HINT_GLYPH;
  int  flags = TTLOAD_DEFAULT;
  return TT_Load_Glyph( fInstance, fGlyph, aIndex, flags );
}
#endif

//////////////////////////////////////////////////////////////////////////////
hippodraw::TextTTF::TextTTF(
)
:fStatus(false)
,fInitFont(true)
,fPointSize(0)
,fViewportWidth(0)
,fViewportHeight(0)
,fRotated(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef HAVE_TTF
  if(!fEngine) {
    fEngine = new TT_Engine;
    TT_Error error = TT_Init_FreeType(fEngine);
    if(error) {
      std::cout << "TextTTF : could not initialise FreeType." << std::endl;
      return;
    }
    // When finished , should do a : TT_Done_FreeType(fEngine);
  }
#else
  std::cout << "TextTTF : compiled without HAVE_TTF CPP macro" << std::endl;
#endif

  //m_fileName = "times";
  m_fileName = "helvetica";
  m_size = 64;
  m_horizontalJustification = LEFT;
  m_verticalJustification = BOTTOM;
  m_viewportMapping = NONE;
#ifdef HAVE_TTF
  fTTF = new TextTTF_Internal;
  fTTF->fOpened = false;
  fTTF->fBitmap.bitmap = 0;
#else
  fTTF = 0;
#endif
  
  fPointSize = (int)m_size;
  initFont();
  fInitFont = false;
}
//////////////////////////////////////////////////////////////////////////////
hippodraw::TextTTF::~TextTTF (
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fStatus = false;
#ifdef HAVE_TTF
  if(fTTF->fOpened==true) {
    free(fTTF->fBitmap.bitmap);
    TT_Done_Glyph(fTTF->fGlyph);
    TT_Done_Instance(fTTF->fInstance);
    TT_Close_Face(fTTF->fFace);
    fTTF->fOpened = false;
  }
  delete fTTF;
#endif
}
void hippodraw::TextTTF::setFileName(const std::string& aFileName){
  m_fileName = aFileName;
  fInitFont = true;
}
void hippodraw::TextTTF::setString(const std::string& aString) {
  m_strings.clear();
  m_strings.push_back(aString);
  fInitFont = true;
}
void hippodraw::TextTTF::setStrings(const std::vector<std::string>& aStrings) {
  m_strings = aStrings;
  fInitFont = true;
}
void hippodraw::TextTTF::setSize(float aSize){
  m_size = aSize;
  fInitFont = true;
}
void hippodraw::TextTTF::setJustification(Justification aH,Justification aV){
  m_horizontalJustification = aH;
  m_verticalJustification = aV;
  fInitFont = true;
}
void hippodraw::TextTTF::setRotated(bool aYesNo){
  fRotated = aYesNo;
}
//////////////////////////////////////////////////////////////////////////////
void hippodraw::TextTTF::render (
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef HAVE_TTF
  if(m_viewportMapping==TextTTF::NONE) {
    if(fInitFont) {
      fPointSize = (int)m_size;
      initFont();
      fInitFont = false;
    }
  } /*
      else if(m_viewportMapping==TextTTF::RESCALE) {
    SbViewportRegion vpr = SoViewportRegionElement::get(aAction->getState());
    const SbVec2s& win = vpr.getViewportSizePixels();
    if( fInitFont || (win[0]!=fViewportWidth) || (win[1]!=fViewportHeight) ) {
      if(win[0]<=win[1]) {
	fPointSize = (int)(m_size * win[0]);
      } else {
	fPointSize = (int)(m_size * win[1]);
      }
      initFont();
      fInitFont = false;
      fViewportWidth = win[0];
      fViewportHeight = win[1];
    }
  } else if(m_viewportMapping==TextTTF::ADJUST) {
    SbViewportRegion vpr = SoViewportRegionElement::get(aAction->getState());
    const SbVec2s& win = vpr.getViewportSizePixels();
    if( fInitFont || (win[0]!=fViewportWidth) || (fViewportHeight!=win[1]) ) {
      // Get size with current point size;
      SbVec2s sz = getTextSizePixels();
      //printf("debug : sz : %d %d : %d %d\n",win[0],win[1],sz[0],sz[1]);
      if( (sz[0]>0) && (sz[1]>0)) {
	if(win[0]<=win[1]) {
	  if(sz[0]<=sz[1]) {
	    fPointSize = 
	      (int)(((float)fPointSize) * ((float)win[1])/((float)sz[1]));
	  } else {
	    fPointSize = (int)(fPointSize * win[0]/((float)sz[0]));
	  }
	} else {
	  if(sz[0]<=sz[1]) {	
	    fPointSize = (int)(fPointSize * win[1]/((float)sz[1]));
	  } else {
	    fPointSize = (int)(fPointSize * win[0]/((float)sz[0]));
	  }
	}
	initFont();
	//sz = getTextSizePixels();
	//printf("debug : new : sz : %d %d : %d %d\n",win[0],win[1],sz[0],sz[1]);
      }
      fInitFont = false;
      fViewportWidth = win[0];
      fViewportHeight = win[1];
    }
  }
    */

  if(fStatus==false) return;

  float red = 0;
  float green = 0;
  float blue = 0;
  glPushAttrib( (GLbitfield)(GL_CURRENT_BIT | GL_ENABLE_BIT));
#ifdef WIN32  
  // Pb on Windows : depth test is out over bitmap !
  glDisable(GL_DEPTH_TEST);
#endif
  glDisable(GL_LIGHTING);
  glColor3f(red,green,blue);
#endif

#ifdef HAVE_TTF
  fTTF->fXJustifyTranslation = 0;
  fTTF->fYJustifyTranslation = 0;
  short w,h;
  if(m_horizontalJustification==LEFT) {
  } else if(m_horizontalJustification==CENTER) {
    getTextSizePixels(w,h);
    fTTF->fXJustifyTranslation = 0.5F * w;
  } else if(m_horizontalJustification==RIGHT) {
    getTextSizePixels(w,h);
    fTTF->fXJustifyTranslation = w;
  }    

  if(m_verticalJustification==BOTTOM) {
  } else if(m_verticalJustification==MIDDLE) {
    getTextSizePixels(w,h);
    fTTF->fYJustifyTranslation = 0.5F * h;
  } else if(m_verticalJustification==TOP) {
    getTextSizePixels(w,h);
    fTTF->fYJustifyTranslation =  h;
  }
#endif

  //  glRasterPos3f fixes also 
  // the color for rasterisation. Then
  // glColor3f should be done before it.
  //glRasterPos3f(0,0,0);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  // Do a push, pop to correct a deffect of Mesa-3.1. 
  // If not, further line drawing will have bad colors.
  // The glPopAttrib will compell a reinitialisation of
  // some internal Mesa state.
  //glPushAttrib(GL_ALL_ATTRIB_BITS);
  //glPopAttrib();

  int linen = m_strings.size();
  for(int count=0;count<linen;count++) {
    renderString(m_strings[count]);
  }

  glPopAttrib();

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void hippodraw::TextTTF::initFont(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fStatus = false;
#ifdef HAVE_TTF
  if(fPointSize<=0) return;

  if(fTTF->fOpened==true) {
    free(fTTF->fBitmap.bitmap);
    TT_Done_Glyph(fTTF->fGlyph);
    TT_Done_Instance(fTTF->fInstance);
    TT_Close_Face(fTTF->fFace); 
    fTTF->fOpened = false;
  }

  char* ttf_path = ::getenv("TTFPATH");
  std::string ttfpath = (!ttf_path ? "" : ttf_path);
  if(ttfpath=="") {
    std::string fullName = m_fileName;
    if(m_fileName.find(".ttf")==std::string::npos) fullName += ".ttf";
    TT_Error error = TT_Open_Face(*fEngine,fullName.c_str(),&(fTTF->fFace));
    if(error) {
      std::cout << "TextTTF::initFont : could not find or open file " 
		<< m_fileName << std::endl;
      return;
    }
  } else {

    char* path = (char*)ttfpath.c_str();
    std::vector<std::string> paths;
    char* token = strtok(path," ");
    do {
      paths.push_back(std::string(token));
    } while( (token = strtok(NULL," "))!=NULL);

    bool found = false;
    unsigned int index;
    for(index=0;index<paths.size();index++) {
      std::string fullName = paths[index];
#ifdef WIN32
      fullName += "\\";
#else
      fullName += "/";
#endif
      fullName += m_fileName;
      if(m_fileName.find(".ttf")==std::string::npos) fullName += ".ttf";
      TT_Error error = 
	TT_Open_Face(*fEngine,fullName.c_str(),&(fTTF->fFace));
      if(error) continue;
      found = true;
      break;
    }

    if(found==false) {
      std::cout << "TextTTF::initFont : could not find or open file "
                << m_fileName << std::endl;
      return;
    } 
  }
    

  TT_Face_Properties  properties;
  TT_Error error = TT_Get_Face_Properties(fTTF->fFace, &properties );
  if(error) {
    std::cout << "TextTTF::initFont : could not get face properties" << std::endl;
    return;
  }

  // Look for a Unicode charmap :
  unsigned short n = properties.num_CharMaps;
  unsigned short i;
  for ( i = 0; i < n; i++ ) {
    unsigned short  platform, encoding;
    TT_Get_CharMap_ID( fTTF->fFace, i, &platform, &encoding );
    if ( (platform == 3 && encoding == 1 )  ||
	 (platform == 0 && encoding == 0 ) ) {
        TT_Get_CharMap( fTTF->fFace, i, &(fTTF->fCharMap) );
        i = n + 1;
      }
  }
  if ( i == n ) {
    std::cout << "TextTTF::initFont : this font doesn't contain any Unicode mapping table" << std::endl;
    return;
  }
  //printf("debug : asc %d\n",properties.vertical->Ascender);

  fTTF->fNumGlyphs = properties.num_Glyphs;

  error = TT_New_Glyph( fTTF->fFace, &(fTTF->fGlyph) );
  if(error) {
    std::cout << "TextTTF::initFont : could not create glyph container" << std::endl;
    return;
  }

  error = TT_New_Instance( fTTF->fFace, &(fTTF->fInstance) );
  if(error) {
    std::cout << "TextTTF::initFont : could not create instance" << std::endl;
    return;
  }

  // Give screen resolution.
  // Device resolution = # of pixels/inch. 
  // Around 96 for screen (300 for printer).
  error = TT_Set_Instance_Resolutions(fTTF->fInstance, 96, 96 );
  if (error) {
    std::cout << "TextTTF::initFont : Could not set instance resolution" << std::endl;
    return;
  }

  // Give size of a character in "pt" :
  // 1 pt = 1/72 inch.
  // Si n pt the char will do : n * 96 /72 pixels on screen.
  // Exa : 64 pt = 64 * 96 /72 = 85 pixels on screen.
  error = TT_Set_Instance_PointSize(fTTF->fInstance,fPointSize);
  //error = TT_Set_Instance_CharSize(fTTF->fInstance,fPointSize * 64);
  if (error) {
    std::cout << "TextTTF::initFont : Could not set instance point size" << std::endl;
    return;
  }

  error = TT_Get_Instance_Metrics(fTTF->fInstance,
				  &(fTTF->fInstanceMetrics));
  if (error) {
    std::cout << "TextTTF::initFont : could not get instance metric" << std::endl;
    return;
  }

  // fInstanceMetrics.y_ppem, x_ppem # of screen pixels.
  fTTF->fBitmap.rows  = fTTF->fInstanceMetrics.y_ppem;
  fTTF->fBitmap.width = fTTF->fInstanceMetrics.x_ppem;
  // # of bytes that contains 'width'.
  fTTF->fBitmap.cols  = (fTTF->fBitmap.width + 7)/8;  
  fTTF->fBitmap.flow  = TT_Flow_Up;
  fTTF->fBitmap.size  = (long)(fTTF->fBitmap.rows * fTTF->fBitmap.cols);
  fTTF->fBitmap.bitmap = malloc( (int)fTTF->fBitmap.size );
  if(!fTTF->fBitmap.bitmap) return;

  fTTF->fHinted = true;

  /* printf("debug : i char size %d pt ; ppem %d %d ; res %d %d\n",
	 fInstanceMetrics.pointSize,
	 fInstanceMetrics.x_ppem,fInstanceMetrics.y_ppem,
	 fInstanceMetrics.x_resolution,fInstanceMetrics.y_resolution);
  */

  //printf("debug : size rows %d cols %d\n",fBitmap.rows,fBitmap.cols);

  //printf("debug : TextTTF : update\n");

  fTTF->fOpened = true;

  fStatus = true;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void hippodraw::TextTTF::renderString(
 const std::string& aString 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aString=="") return;
#ifdef HAVE_TTF
  fTTF->fNewLine = false;
  fTTF->fWidth = 0;
  int l = aString.size();
  for(int count=0;count<l;count++) { 
    if(count==l-1) fTTF->fNewLine = true;
    renderCharacter(aString[count]);
  }
#endif
}
//////////////////////////////////////////////////////////////////////////////
void hippodraw::TextTTF::renderCharacter(
 char aChar 
)
//////////////////////////////////////////////////////////////////////////////
/* Convert an ASCII string to a string of glyph indexes.              */
/*                                                                    */
/* IMPORTANT NOTE:                                                    */
/*                                                                    */
/* There is no portable way to convert from any system's char. code   */
/* to Unicode.  This function simply takes a char. string as argument */
/* and "interprets" each character as a Unicode char. index with no   */
/* further check.                                                     */
/*                                                                    */
/* This mapping is only valid for the ASCII character set (i.e.,      */
/* codes 32 to 127); all other codes (like accentuated characters)    */
/* will produce more or less random results, depending on the system  */
/* being run.                                                         */

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef HAVE_TTF
  short index = TT_Char_Index( fTTF->fCharMap, (short)aChar);

  if((index<0)||(index>=fTTF->fNumGlyphs)) return;

  //postInfo("TextTTF::renderCharacter","debug 001");

  TT_Error error = fTTF->loadTrueTypeChar(index);
  if (error) {
    std::cout << "TextTTF::renderCharacter." << std::endl;
    return;
  }

  // Metric infos are given in "Char_PointSize * 64 units".
  TT_Big_Glyph_Metrics  metrics;
  error = TT_Get_Glyph_Big_Metrics( fTTF->fGlyph, &metrics );
  if (error) {
    std::cout << "TextTTF::renderCharacter : could not get glyph metrics"
	      << std::endl;
    return;
  }

  /* printf("debug : bb xmn %d ymn %d xmx %d ymx %d ; bX %d bY %d ; adv %d %d\n",
	 metrics.bbox.xMin,metrics.bbox.yMin,
	 metrics.bbox.xMax,metrics.bbox.yMax,
	 metrics.horiBearingX,metrics.horiBearingY,
	 metrics.horiAdvance,metrics.vertAdvance);
  */
  
  // To bring the whole glyph in the bitmap.
  TT_F26Dot6 xmin, ymin;
  xmin = metrics.bbox.xMin & -64;
  ymin = metrics.bbox.yMin & -64;

  //printf("debug : xx %d %d\n",xmin,ymin);

  memset( fTTF->fBitmap.bitmap, 0, fTTF->fBitmap.size );
  error = TT_Get_Glyph_Bitmap( fTTF->fGlyph, 
			       &(fTTF->fBitmap), 
			       -xmin, -ymin);
  if (error) {
    std::cout << "TextTTF::renderCharacter : could not get glyph bitmap"
	      << std::endl;
  } else {

    TT_F26Dot6 xmove,ymove;
    if(fTTF->fNewLine==true) {
      xmove = -fTTF->fWidth;
      //ymove = -(fTTF->fInstanceMetrics.y_ppem + 10);
      ymove = - (metrics.vertAdvance/64);
      fTTF->fNewLine = false;
    } else {
      //xmove = fInstanceMetrics.x_ppem + 3;
      xmove = metrics.horiAdvance/64;
      ymove = 0;
      fTTF->fWidth += (TT_UShort)xmove;
    }

    float xorig = fTTF->fXJustifyTranslation;
    float yorig = (float)(-ymin/64)+fTTF->fYJustifyTranslation;

    if(fRotated) {
      GLubyte* pfrom = (GLubyte*)fTTF->fBitmap.bitmap;
      GLsizei bwidth = fTTF->fBitmap.rows;
      GLsizei bheight = fTTF->fBitmap.width;
      GLubyte* bptr = new GLubyte[bwidth * bheight];
      int irow;
      for(irow=0;irow<fTTF->fBitmap.rows;irow++) {
        int icol = 0;
        for(int ibyte=0;ibyte<fTTF->fBitmap.cols;ibyte++) {
          GLubyte byte = *pfrom; //Contains 8 pixels.
          pfrom++;
          GLubyte pixel8 = (byte >> 0) & 0x1;
          GLubyte pixel7 = (byte >> 1) & 0x1;
          GLubyte pixel6 = (byte >> 2) & 0x1;
          GLubyte pixel5 = (byte >> 3) & 0x1;
          GLubyte pixel4 = (byte >> 4) & 0x1;
          GLubyte pixel3 = (byte >> 5) & 0x1;
          GLubyte pixel2 = (byte >> 6) & 0x1;
          GLubyte pixel1 = (byte >> 7) & 0x1;

          int bicol = fTTF->fBitmap.rows-irow-1; // To have + 90 deg.

          int birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel1;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel2;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel3;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel4;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel5;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel6;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel7;
          icol++;

          birow = icol;
          if(icol<fTTF->fBitmap.width) *(bptr+birow*bwidth+bicol) = pixel8;
          icol++;          
        }
      }
      GLubyte* ptr = bptr;
      int bcols = (bwidth + 7)/8;  
      GLubyte* bptr2 = new GLubyte[bheight * bcols];
      GLubyte* ptr2 = bptr2;
      for(irow=0;irow<bheight;irow++) {
        int icol = 0;
        for(int ibyte=0;ibyte<bcols;ibyte++) {
          GLubyte byte = 0;
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 7);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 6);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 5);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 4);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 3);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 2);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 1);
            ptr++;icol++;
          }
          if(icol<bwidth) {
            byte = byte | ((*ptr) << 0);
            ptr++;icol++;
          }
          *ptr2 = byte;ptr2++;
        }
      }
      glBitmap(bwidth,bheight,xorig+bwidth,yorig,(float)ymove,(float)xmove,
               (GLubyte*)bptr2);
      delete [] bptr;
      delete [] bptr2;
    } else {
      glBitmap(fTTF->fBitmap.width,fTTF->fBitmap.rows,
               xorig,yorig,(float)xmove,(float)ymove,
               (GLubyte*)fTTF->fBitmap.bitmap);
    }
  }
#else
  aChar = 0;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool hippodraw::TextTTF::getTextSizePixels(
 short& aWidth
,short& aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aWidth = 0;
  aHeight = 0;
  if(fStatus==false) return false;
#ifdef HAVE_TTF
  int linen = m_strings.size();
  fTTF->fTextWidth = 0;
  fTTF->fTextHeight = 0;
  for(int count=0;count<linen;count++) {
    const std::string& s = m_strings[count];
    if(s=="") continue; // Must be coherent with GLRender.
    fTTF->fNewLine = false;
    fTTF->fWidth = 0;
    int l = s.size();
    TT_F26Dot6 yMax = 0;
    TT_F26Dot6 yMin = 0;
    for(int i=0;i<l;i++) { 
      if(i==l-1) fTTF->fNewLine = true;
      char c = s[i];
      short index = TT_Char_Index(fTTF->fCharMap,(short)c);
      if((index<0)||(index>=fTTF->fNumGlyphs)) continue; //?
      TT_Error error = fTTF->loadTrueTypeChar(index);
      if (error) {
	std::cout << "TextTTF::getTextSizePixels" << std::endl;
	continue; // Same as GLRender.
      }
      // Metric infos are given in "Char_PointSize * 64 units".
      TT_Big_Glyph_Metrics  metrics;
      error = TT_Get_Glyph_Big_Metrics( fTTF->fGlyph, &metrics );
      if (error) {
	std::cout << "TextTTF::getTextSizePixels : could not get glyph metrics"
		  << std::endl;
	continue; //Same as GLRender.
      }
      //TT_F26Dot6 xmin, ymin;
      //xmin = metrics.bbox.xMin & -64;
      //ymin = metrics.bbox.yMin & -64;
      TT_F26Dot6 xmove,ymove;
      if(fTTF->fNewLine==true) {
	fTTF->fWidth += (TT_UShort)metrics.horiAdvance/64;
	xmove = -fTTF->fWidth;
	//ymove = -(fTTF->fInstanceMetrics.y_ppem + 10);
	ymove = - (metrics.vertAdvance/64);
	fTTF->fNewLine = false;
	//
	fTTF->fTextWidth = MAXIMUM(fTTF->fTextWidth,fTTF->fWidth);
	if(count==0) fTTF->fTextHeight += yMax; // First line.
	if(count!=linen-1) fTTF->fTextHeight += (TT_UShort)-ymove;
	if(count==linen-1) fTTF->fTextHeight += -yMin; // Last line.
      } else {
	//xmove = fInstanceMetrics.x_ppem + 3;
	xmove = metrics.horiAdvance/64;
	ymove = 0;
	fTTF->fWidth += (TT_UShort)xmove;
      }
      yMax = MAXIMUM(yMax,(metrics.bbox.yMax & -64)/64);
      yMin = MINIMUM(yMin,(metrics.bbox.yMin & -64)/64);
    }
  }
  aWidth = fTTF->fTextWidth;
  aHeight = fTTF->fTextHeight;
  return true;
#else
  return false;
#endif
}
