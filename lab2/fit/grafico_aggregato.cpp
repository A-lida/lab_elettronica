//
// Created by pi on 12/24/24.
//

#include "TCanvas.h"
#include "TGraphErrors.h"

#include "data_handling.hpp"
#include "graphic_stuff.h"
#include "TLegend.h"

void createMultiGraph(const std::vector<const char *> &source_files,
                      const char *title, const char *output_file) {
  // Leggo tutti i csv
  std::vector<GraphInitializers> singleDataSets{};
  singleDataSets.reserve(source_files.size());
  for (const auto source_file : source_files) {
    GraphInitializers data{};
    readCsv(source_file, data);
    singleDataSets.emplace_back(data);
  }

  // Creo i grafici
  std::vector<TGraphErrors> graphs{};
  graphs.reserve(singleDataSets.size());
  for (auto &singleDataSet : singleDataSets) {
    graphs.emplace_back(static_cast<Int_t>(singleDataSet.X.size()),
                        singleDataSet.X.data(), singleDataSet.Y.data(),
                        singleDataSet.EX.data(), singleDataSet.EY.data());
    graphs.back().SetMarkerSize(1);
    graphs.back().SetMarkerStyle(1);
  }

  auto legend = new TLegend(0.8,0.8,0.9,0.9);
  graphs[0].SetMarkerColor(kOrange+7);
  graphs[1].SetMarkerColor(kSpring-1);
//  legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
  legend->AddEntry("graphs[0]","-100#muA");
  legend->AddEntry("graph[1]","-200#muA");

  graphs[0].SetTitle(title);

  TCanvas c{"", "", 1600, 1080};

  graphs[0].Draw("AP*");
  graphs[1].Draw("SAME P*");
  legend->Draw();

  c.SaveAs(output_file);
}

int main() {
  setStyle();

  createMultiGraph({"../dati_200uA.csv", "../dati_100uA.csv"},
                   "Caratteristiche a -100#muA e -200#muA a confronto;-V_{CE} (V);-I_{C} (mA)",
                   "aggregato.root");
}
