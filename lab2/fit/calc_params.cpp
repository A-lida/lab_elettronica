//
// Created by pi on 12/24/24.
//

#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"

#include <algorithm>
#include <iostream>

#include "data_handling.hpp"

struct MeasureAndError {
  double measure;
  double error;

  void print() const {
    std::cout << measure << "\t+/-\t" << error << std::endl;
  }
};

struct ParamsSet {
  MeasureAndError Va;
  MeasureAndError g;

  [[nodiscard]] MeasureAndError b() const {
    const double measure = 1. / g.measure;
    const double error = measure * g.error / g.measure;
    return {measure, error};
  }

  void print() const {
    std::cout << "\tVa:\t";
    Va.print();
    std::cout << "\tb:\t";
    b().print();
    std::cout << "\tg:\t";
    g.print();
  }
};

void calcIndividualParams(const char *sourceFile, const char *current) {
  ParamsSet p{};
  std::unique_ptr<TFile> file1(TFile::Open(sourceFile, "READ"));
  std::unique_ptr<TF1> funzione1(file1->Get<TF1>("fit"));

  p.Va.measure = funzione1->GetParameter(1);
  p.Va.error = funzione1->GetParError(1);
  p.g.measure = funzione1->GetParameter(0);
  p.g.error = funzione1->GetParError(0);

  std::cout << "Estimated parameters for " << current << " uA:\n";
  p.print();
  std::cout << std::endl;
}

double multimeterError(const double measure) {
  return static_cast<double>(static_cast<int>((measure * 0.01 + 0.03) * 100.)) /
         100.;
}

int main() {

  const double err1 = multimeterError(0.1);
  const double err2 = multimeterError(0.2);
  std::cout << "Errore su 100 uA:\t" << err1 << std::endl;
  std::cout << "Errore su 200 uA:\t" << err2 << std::endl << std::endl;

  calcIndividualParams("fit_100uA.root", "100");
  calcIndividualParams("fit_200uA.root", "200");

  // Leggo i dati per calcolare il beta
  GraphInitializers data1{};
  GraphInitializers data2{};
  readCsv("../dati_100uA.csv", data1);
  readCsv("../dati_200uA.csv", data2);

  // Calcolo il beta per tutti i voltaggi che abbiamo acquisito
  std::transform(data1.Y.begin(), data1.Y.end(), data2.Y.begin(),
                 data1.Y.begin(),
                 [](const double a, const double b) { return (-a + b) / 0.1; });
  std::transform(data1.EY.begin(), data1.EY.end(), data2.EY.begin(),
                 data1.EY.begin(),
                 [&, i = 0](const double a, const double b) mutable {
                   const double collectorDifferenceErr = a + b;
                   const double collectorDifferenceRelErr =
                       collectorDifferenceErr / (data1.Y[i] * 0.1);
                   const double baseDifferenceErr = err1 + err2;
                   const double baseDifferenceRelErr = baseDifferenceErr / 0.1;

                   const double totalRelErr =
                       collectorDifferenceRelErr + baseDifferenceRelErr;
                   const double totalErr = totalRelErr * data1.Y[i];

                   i++;
                   return totalErr;
                 });

  // Stampo il beta
  const auto chosenBetaIndex =
      std::find(data1.X.begin(), data1.X.end(), 2.) - data1.X.begin();
  std::cout << "beta:\t" << data1.Y[chosenBetaIndex] << "\t+/-\t"
            << data1.EY[chosenBetaIndex] << std::endl;

  // Creo il grafico dei beta (non che ci serva ma possiamo metterlo)
  TGraphErrors graph{static_cast<Int_t>(data1.X.size()), data1.X.data(),
                           data1.Y.data(), data1.EX.data(), data1.EY.data()};

  graph.SetTitle("Guadagno di corrente tra 100 e 200 #muA;V_{CE} (V);#beta");
  graph.SaveAs("graph.root");
}