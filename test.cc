#include <iostream>
#include <vector>

#include <boost/foreach.hpp>

#include <TH2D.h>
#include <TFile.h>

#include <HistUtils.hxx>

int main()
{
  TFile *file = new TFile("all-histos-Vetra_histos-2011-2012-2.root", "read");
  std::vector<TH2D*> histvec;

  Hist::HistReader reader1(file);
  reader1.getHistVec("Overview.*", histvec);

  Hist::HistReader reader2(file, "Velo/PCNErrorMon");
  reader2.getHistVec("hperBeetle.*_104_(4|5|6|7)", histvec);

  BOOST_FOREACH(TH2D* hist, histvec) {
    hist->Print();
  }

  Hist::HistReader reader3(file, "Velo/PCNErrorMon");
  TH2D *hist = NULL;
  reader3.getHist("hBeetleMap", hist);

  hist->Print();

  return 0;
}
