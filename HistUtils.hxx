#ifndef __HISTUTILS_HXX
#define __HISTUTILS_HXX

#include <string>
#include <vector>

#include <TH1.h>

namespace Hist {

  /**
   * Rescale X-axis of the passed histogram.
   *
   * @param h Pointer to histogram
   * @param factor Scale factor. (default = 0.001, corresponds to GeV/MeV -> TeV/GeV)
   */
  void rescale(TH1 *hist, Double_t factor);
}

#endif	// __HISTUTILS_HXX
