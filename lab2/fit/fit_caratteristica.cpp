#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TPaveStats.h"
#include "TROOT.h"
#include "TStyle.h"
#include <Rtypes.h>
#include <TMath.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <cstring>
#include <iostream>


void setStyle() {
  //   gROOT->SetStyle("Plain");
  gStyle->SetOptStat(2210);
  gStyle->SetOptFit();
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

void cutiefier(TVirtualPad* pad) {
  gPad->Update();
  TPaveStats *statbox = (TPaveStats *)pad->GetPrimitive("stats");

  if (statbox) {
    // Cambia le coordinate NDC (x1, y1, x2, y2)
    statbox->SetX1NDC(0.486422 ); // Nuova posizione X1 (vicino al bordo sinistro)
    statbox->SetX2NDC(0.986075); // Nuova posizione X2
    statbox->SetY1NDC( 0.840136); // Altezza inferiore
    statbox->SetY2NDC(0.944496); // Altezza superiore
    gPad->Modified();
  } else {
    std::cerr << "Statbox non trovata!\n";
  }
}

void create_graphs(const char *source_file, const char *title, const char* output_file) {
  TCanvas c{"", "", 1920, 1080};
  c.Divide(2);

  TGraphErrors graph_data(source_file);
  graph_data.SetTitle(title);

  c.cd(1);


  graph_data.SetMarkerSize(1);
  graph_data.SetMarkerStyle(1);

  graph_data.Draw("AP*");
  c.Draw();
  cutiefier(c.GetPad(1));

  TGraphErrors graph_data{graph_data.GetN(), graph_data.GetX(), graph_data.GetX(), graph_data.GetEX(), graph_data.GetEY()};

  TF1 retta("caratteristica", "[0]+ x/ [1]");
  retta.SetParameters(-3, 1. / 50);
  retta.SetParNames("ln(I_{0})", "#eta V_{T}");
  retta.SetLineColor(kBlue);

  graph_fit.SetTitle(title);
  graph_fit.SetMarkerStyle(1);
  graph_fit.SetMarkerSize(1);
  graph_fit.Fit(&retta);

  c.cd(2);
  graph_fit.Draw("AP*");
  c.Draw();
  cutiefier(c.GetPad(2));

  c.SaveAs(output_file);
}

int main() {
  // void fit_diodi() {
  setStyle();

  create_graphs("../fit/dati_100uA.csv",
                "Caratteristica corrente-tensione silicio;Tensione (mV);Corrente (mA)", "fit_100uA.root");

  create_graphs("../fit/dati_200uA.csv",
                "Caratteristica corrente-tensione germanio;Tensione (mV);Corrente (mA)", "fit_200uA.root");
}