#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TPad.h"

#include "data_handling.hpp"
#include "graphic_stuff.h"

void createSingleGraph(const char *source_file, const char *title,
                       const double minX, const double maxX,
                       const char *output_file) {
  GraphInitializers allData{};
  GraphInitializers fit{};
  GraphInitializers noFit{};

  // Leggo il csv e separo i dati
  readCsv(source_file, allData);
  filterData(allData, fit, noFit, minX, maxX);

  // Creo la canvas
  TCanvas c{"", "", 1600, 1080};
  c.Divide(1);

  // Creo i grafici
  TGraphErrors fitGraph{static_cast<Int_t>(fit.X.size()), fit.X.data(),
                        fit.Y.data(), fit.EX.data(), fit.EY.data()};
  TGraphErrors noFitGraph{static_cast<Int_t>(noFit.X.size()), noFit.X.data(),
                          noFit.Y.data(), noFit.EX.data(), noFit.EY.data()};

  // Configuro gli stili dei grafici
  fitGraph.SetMarkerStyle(1); // Style
  noFitGraph.SetMarkerStyle(1);

  fitGraph.SetMarkerSize(1); // Size
  noFitGraph.SetMarkerSize(1);

  fitGraph.SetMarkerColor(kBlue); // Color
  noFitGraph.SetMarkerColor(kRed);

  noFitGraph.SetTitle(title); // Title

  // Creo la funzione di fit in modo che mi dia V_A
  TF1 retta("caratteristica", "[0] * ( x - [1])", minX, maxX);
  retta.SetParameters(1, -2);
  retta.SetParNames("g (mA #upoint mV^{-1})", "V_{A} (mA)");
  retta.SetLineColor(kBlue);

  // Fitto con il range della funzione
  fitGraph.Fit(&retta, "R");

  // Disegno il grafico totale
  c.cd(1);
  noFitGraph.Draw("AP*");
  fitGraph.Draw("SAME P*");
  c.Draw();
  cutiefier(c.GetPad(1));

  // Salvo il risultato
  std::unique_ptr<TFile> file(TFile::Open(output_file, "RECREATE"));
  file->WriteObject(&c, "c");
  file->WriteObject(&retta, "fit");
}

int main() {
  setStyle();
  // FIXME Aggiustare i titoli (così fanno un po' schifo)
  createSingleGraph(
      "../dati_100uA.csv",
      "Caratteristica corrente di collettore - tensione con corrente "
      "di base 100 #muA;Tensione (mV);Corrente (mA)",
      0.9, 3.3, "fit_100uA.root");

  // TODO Testare con Chauvenier se il dato a 1.5 è rigettabile
  createSingleGraph(
      "../dati_200uA.csv",
      "Caratteristica corrente di collettore - tensione con corrente "
      "di base 200 #muA;Tensione (mV);Corrente (mA)",
      1.02, 3.1, "fit_200uA.root");
  // TODO Calcolare i parametri beta & co.
}
