#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TStyle.h"
#include <Rtypes.h>
#include <TMath.h>

inline void setStyle() {
  //   gROOT->SetStyle("Plain");
  gStyle->SetOptStat(2210);
  gStyle->SetOptFit(111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

int main() {
  setStyle();

  TCanvas c_s{"", "", 1920, 1080};
  c_s.Divide(2);
  // c_s.GetPad(1)->SetLogy();
  TGraphErrors graph_s_log("../fit/diodo_silicio.csv");
  graph_s_log.SetTitle(
      "Caratteristica corrente-tensione;Tensione (mV);Corrente (mA)");

  auto n_s = graph_s_log.GetN();
  auto x_s = graph_s_log.GetX();
  auto y_s = graph_s_log.GetY();

  for (int i = 0; i < n_s; i++) {
    y_s[i] = TMath::Log(y_s[i]);
  }

  TGraphErrors graph_s{n_s, x_s, y_s, graph_s_log.GetEX(), graph_s_log.GetEY()};

  TF1 retta("caratteristica", "[0] * x + [1]");
  retta.SetParameter(0, 1. / 420);
  retta.SetParameter(1, -5);
  retta.SetLineColor(kBlue);

  graph_s.SetMarkerStyle(1);
  graph_s.SetMarkerSize(1);
  graph_s.Fit(&retta);

  // TF1 exp("esponenziale", "[0]*exp(-x/[1])");
  // exp.SetParameters(TMath::Exp(-5), 1. / 420);
  // exp.SetLineColor(kOrange);

  // graph_s_log.SetMarkerSize(1);
  // graph_s_log.SetMarkerStyle(1);
  // graph_s_log.Fit(&exp);

  c_s.cd(1);
  graph_s_log.Draw("AP");

  c_s.cd(2);

  graph_s.Draw("AP");
  c_s.SaveAs("silicio.png");

  // TCanvas c_g{};
  // c_g.GetPad(1)->SetLogy();
  // TGraphErrors graph_g;
  // TGraphErrors graph_g_log("../fit/diodo_germanio.csv");
}