#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TROOT.h"
#include <Rtypes.h>
#include <TPaveStats.h>

void setStyle() {
  //   gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1110);
  gStyle->SetOptFit();
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

void cutifier(TVirtualPad* pad){
gPad->Update(); // Aggiorna il canvas per garantire che gli oggetti siano stati creati

TPaveStats *stats = (TPaveStats*)pad->GetPrimitive("stats");
if (stats) {
    stats->SetX1NDC(0.7); // Coordinata X (angolo in basso a sinistra)
    stats->SetY1NDC(0.7); // Coordinata Y (angolo in basso a sinistra)
    stats->SetX2NDC(0.9); // Coordinata X (angolo in alto a destra)
    stats->SetY2NDC(0.9); // Coordinata Y (angolo in alto a destra)
}
gPad->Modified();
gPad->Update();
}

int main() {
  setStyle();

  TCanvas c{"", "", 1080, 1080};
  c.Divide(1);
  TGraphErrors graph("../calibrazione.csv");
  graph.SetTitle("Calibrazione multimetro-oscilloscopio;V multimetro "
                 "(mV);V oscilloscopio (mV)");

  TF1 retta("retta", "[0] * x + [1]", 0, 810);
  retta.SetParameters(1, 1);
  retta.SetLineColor(kGreen);
  retta.SetParNames("Slope", "Offset (mV)");

  cutifier(c.GetPad(1));
  graph.SetMarkerStyle(1);
  graph.SetMarkerSize(1);
  graph.Fit(&retta);

  c.cd(1);

  graph.Draw("AP*");
  c.Draw();
  c.SaveAs("calibrazione.root");
}