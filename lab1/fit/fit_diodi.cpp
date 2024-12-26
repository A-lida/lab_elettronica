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

void create_graphs(const char *source_file, const char *title1, const char *title2, const char* output_file) {
  TCanvas c{"", "", 1920, 1080};
  c.Divide(2);

  TGraphErrors graph(source_file);
  graph.SetTitle(title1);

  c.cd(1);

  TF1 exp("esponenziale", "[0]*exp(x/[1])");
  exp.SetParameters(1E-6, 355. / 6);
  exp.SetParNames("I_{0} (mA)", "#eta V_{T} (mV)");
  exp.SetLineColor(kOrange);

  graph.SetMarkerSize(1);
  graph.SetMarkerStyle(1);
  graph.Fit(&exp);

  graph.Draw("AP*");
  c.Draw();
  cutiefier(c.GetPad(1));

  auto N = graph.GetN();
  auto y = new double[N];
  std::memcpy(y, graph.GetY(), N * sizeof(double));
  auto err_y = new double[N];
  std::memcpy(err_y, graph.GetEY(), N * sizeof(double));

  for (int i = 0; i < N; i++) {
    err_y[i] = 1. / y[i] * err_y[i];
    y[i] = TMath::Log(y[i]);
  }

  TGraphErrors graph_log{N, graph.GetX(), y, graph.GetEX(), err_y};

  TF1 retta("caratteristica", "[0]+ x/ [1]");
  retta.SetParameters(-3, 1. / 50);
  retta.SetParNames("ln(I_{0})", "#eta V_{T} (mV)");
  retta.SetLineColor(kBlue);
  graph_log.SetTitle(title2);
  graph_log.SetMarkerStyle(1);
  graph_log.SetMarkerSize(1);
  graph_log.Fit(&retta);

  c.cd(2);
  graph_log.Draw("AP*");
  c.Draw();
  cutiefier(c.GetPad(2));

  c.SaveAs(output_file);
}

int main() {
  // void fit_diodi() {
  setStyle();

  create_graphs("../diodo_silicio.csv",
                "Caratteristica corrente-tensione Silicio;V (mV);I (mA)", "Caratteristica corrente-tensione Silicio;V (mV);ln(I)", "silicio.root");

  create_graphs("../diodo_germanio.csv",
                "Caratteristica corrente-tensione Germanio;V (mV);I (mA)", "Caratteristica corrente-tensione Germanio;V (mV);ln(I)", "germanio.root");
}