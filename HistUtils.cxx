#include <iostream>
#include <vector>
#include <fstream>

#include "HistUtils.hxx"


namespace Hist {

  void rescale(TH1 *hist, Double_t factor)
  {
    // change X scale
    Double_t xmin = hist->GetXaxis()->GetXmin();
    Double_t xmax = hist->GetXaxis()->GetXmax();
    hist->GetXaxis()->SetLimits(xmin*factor,xmax*factor);
    return;
  }
}
