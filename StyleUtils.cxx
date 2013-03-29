#include <iostream>
#include <vector>
#include <fstream>

#include "StyleUtils.hxx"


TStyle* Style::setStyle()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetPalette(1); // "rainbow" color palette
  gStyle->SetNumberContours(256); // smooth color palette
  gStyle->SetTitleOffset( 1.2, "xy");
  gStyle->SetCanvasPreferGL(true);
  return gStyle;
}
