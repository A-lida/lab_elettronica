#include "ROOT/RCsvDS.hxx"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include <algorithm>
#include <iostream>

#include "graphic_stuff.h"
#include "header.hpp"

void readCsv(const char *source_file, GraphInitializers &data) {
  auto dataFrame = ROOT::RDF::FromCSV(
      source_file, false, '\t', -1,
      {{"Col0", 'D'}, {"Col1", 'D'}, {"Col2", 'D'}, {"Col3", 'D'}});
  data.X = *dataFrame.Take<double>("Col0");
  data.Y = *dataFrame.Take<double>("Col1");
  data.EX = *dataFrame.Take<double>("Col2");
  data.EY = *dataFrame.Take<double>("Col3");
}

void filterData(const GraphInitializers &allData, GraphInitializers &fit,
                GraphInitializers &noFit, const double minX,
                const double maxX) {
  std::for_each(allData.X.begin(), allData.X.end(),
                [&, i = 0](const double x) mutable {
                  GraphInitializers &selectedVectors =
                      minX <= x && x <= maxX ? fit : noFit;
                  selectedVectors.X.push_back(x);
                  selectedVectors.Y.push_back(allData.Y[i]);
                  selectedVectors.EX.push_back(allData.EX[i]);
                  selectedVectors.EY.push_back(allData.EY[i]);
                  i++;
                });
  assert(fit.X.size() == fit.Y.size() && fit.X.size() == fit.EX.size() &&
         fit.X.size() == fit.EY.size());
  assert(noFit.X.size() == noFit.Y.size() &&
         noFit.X.size() == noFit.EX.size() &&
         noFit.X.size() == noFit.EY.size());
  std::cout << "Info: trimmed " << allData.X.size() - fit.X.size()
            << " elements\n\n";
}

void create_graphs(const char *source_file, const char *title,
                   const double minX, const double maxX,
                   const char *output_file) {
  GraphInitializers allData{};
  GraphInitializers fit{};
  GraphInitializers noFit{};

  // Leggo il csv e separo i dati
  readCsv(source_file, allData);
  filterData(allData, fit, noFit, minX, maxX);

  // Creo la canvas
  TCanvas c{"", "", 1280, 1080};
  c.Divide(2);

  // Creo i grafici
  TGraphErrors dummyFitGraph{static_cast<Int_t>(fit.X.size()), fit.X.data(),
                             fit.Y.data(), fit.EX.data(), fit.EY.data()};
  TGraphErrors noFitGraph{static_cast<Int_t>(noFit.X.size()), noFit.X.data(),
                          noFit.Y.data(), noFit.EX.data(), noFit.EY.data()};

  // Configuro gli stili dei grafici
  dummyFitGraph.SetMarkerStyle(1); // Style
  noFitGraph.SetMarkerStyle(1);

  dummyFitGraph.SetMarkerSize(1); // Size
  noFitGraph.SetMarkerSize(1);

  dummyFitGraph.SetMarkerColor(kBlue); // Color
  noFitGraph.SetMarkerColor(kRed);

  noFitGraph.SetTitle(title); // Title

  // Disegno il grafico totale
  c.cd(1);
  noFitGraph.Draw("AP*");
  dummyFitGraph.Draw("SAME P*");

  // Genero il grafico del fit
  auto fitGraph = dynamic_cast<TGraphErrors *>(dummyFitGraph.Clone());
  TF1 retta("caratteristica", "[0] + x * [1]");
  retta.SetParameters(1, .5);
  // FIXME V_{A} sarebbe l'intercetta con l'asse X, non so come chiamare
  //  l'intercetta con l'asse Y
  retta.SetParNames("V_{A}", "g");
  retta.SetLineColor(kBlue);

  fitGraph->Fit(&retta);

  // Disegno il grafico del fit
  c.cd(2);
  fitGraph->Draw("AP*");
  c.Draw();
  cutiefier(c.GetPad(2));

  // Salvo il risultato
  c.SaveAs(output_file);

  // Cleanup
  delete fitGraph;
}

int main() {
  setStyle();
  // FIXME Aggiustare i titoli (così fanno un po' schifo)
  // TODO Mettere i titoli anche ai grafici dei fit (dopo [23/12] lo faccio io
  //  [Paolo])
  create_graphs("../dati_100uA.csv",
                "Caratteristica corrente di collettore - tensione con corrente "
                "di base 100 #muA;Tensione (mV);Corrente (mA)",
                0.9, 3.3, "fit_100uA.root");

  // TODO Testare con Chauvenier se il dato a 1.5 è rigettabile
  create_graphs("../dati_200uA.csv",
                "Caratteristica corrente di collettore - tensione con corrente "
                "di base 200 #muA;Tensione (mV);Corrente (mA)",
                1.02, 3.1, "fit_200uA.root");
  // TODO Calcolare i parametri beta & co.
  // TODO Fare il grafico unito (non dovrebbe essere difficile con la funzione
  //  readCSV)
}
