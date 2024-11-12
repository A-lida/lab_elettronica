#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TROOT.h"

inline void setStyle() {
//   gROOT->SetStyle("Plain");
  gStyle->SetOptStat(2210);
  gStyle->SetOptFit(111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);

}

int main() {
  setStyle();

  TCanvas c1{};
  TGraphErrors graph("../fit/calibrazione.csv");
  graph.SetTitle("Calibrazione multimetro-oscilloscopio;Multimetro "
                 "(mV);Oscilloscopio (mV)");

  TF1 retta("retta", "[0] * x + [1]", 0, 810);
  retta.SetParameter(0, 1);
  retta.SetParameter(1, 1);
  retta.SetLineColor(kBlue);

  graph.SetMarkerStyle(1);
  graph.SetMarkerSize(1);
  graph.Fit(&retta);

  graph.Draw("AP");
  c1.SaveAs("calibrazione.png");
}