#ifndef __STYLEUTILS_HXX
#define __STYLEUTILS_HXX

#include <list>

#include <TObject.h>
#include <TStyle.h>


namespace Style {

  /**
   * Set global ROOT style and return the TStyle object.
   *
   * @return
   */
  TStyle* setStyle();

  // class RooLegend : public TObject {
  // public:
  //   RooLegend();
  //   ~RooLegend();


  // private:
  //   std::list entries;
  // };
}

#endif	// __STYLEUTILS_HXX
