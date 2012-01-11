#ifndef Hippo_TextTTF_h
#define Hippo_TextTTF_h

#include <vector>
#include <string>

class TextTTF_Internal;

namespace hippodraw {

/**
 *  TextTTF permits to render text in the pixmap buffer 
 * by using the freetype fonts. It permits to have portable (X11, Win32)
 * rendering for scalable fonts.

    @attention This
    class was contributed by its author as an experiment to evaluate
    the core library classes with OpenGl.  It is not actively
    maintained.  This class and others in this directory require CMT
    for its build.

    @author Guy Barrand <barrand@lal.in2p3.fr>
*/
class TextTTF {
public:
  enum Justification {
    LEFT   = 0x01,
    RIGHT  = 0x02,
    CENTER = 0x03,
    BOTTOM = 0x04,
    MIDDLE = 0x05,
    TOP    = 0x06
  };
  enum ViewportMapping {
    NONE,
    RESCALE,
    ADJUST
  };
  TextTTF();
  virtual ~TextTTF();
  void setFileName(const std::string&);
  void setString(const std::string&);
  void setStrings(const std::vector<std::string>&);
  void setSize(float);
  void setJustification(Justification,Justification);
  void setRotated(bool);
  void render();
private:
  bool setup();
  void renderString(const std::string&);
  void renderCharacter(char);
  bool getTextSizePixels(short&,short&);
private:
  // Fields
  std::string m_fileName;
  std::vector<std::string> m_strings;
  float m_size;
  Justification m_horizontalJustification;
  Justification m_verticalJustification;
  ViewportMapping m_viewportMapping;

  TextTTF_Internal* fTTF;
  bool fStatus;
  void initFont(); 
  bool fInitFont;
  int fPointSize;
  int fViewportWidth;
  int fViewportHeight;
  bool fRotated;
};

} // end namespace hippodraw

#endif
