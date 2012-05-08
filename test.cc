#include <iostream>
#include <vector>

#include <boost/foreach.hpp>

#include <TH2D.h>
#include <TFile.h>

#include <HistUtils.hxx>

int main()
{
  TFile *file = new TFile("../VeloMoniGui/histo-check-Vetra_histos-2011-2012.root", "read");
  std::vector<TH2D*> histvec;

  Hist::HistReader reader1(file);
  reader1.getHistVec("Overview.*", histvec);

  Hist::HistReader reader2(file, "Velo/PCNErrorMon");
  reader2.getHistVec("hperBeetle.*", histvec);

  BOOST_FOREACH(TH2D* hist, histvec) {
    hist->Print("all");
  }

  Hist::HistReader reader3(file, "Velo/PCNErrorMon");
  TH2D *hist = NULL;
  reader3.getHist("hBeetleMap", hist);

  hist->Print("all");

  return 0;
}
