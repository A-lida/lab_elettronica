//
// Created by pi on 12/24/24.
//

#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"

#include <iostream>
#include <memory>

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

int main() {
  calcIndividualParams("fit_100uA.root", "100");
  calcIndividualParams("fit_200uA.root", "200");

  // Calcolo il beta
  GraphInitializers data1{};
  GraphInitializers data2{};
  readCsv("../dati_100uA.csv", data1);
  readCsv("../dati_200uA.csv", data2);

  std::transform(data1.Y.begin(), data1.Y.end(), data2.Y.begin(),
                 data1.Y.begin(),
                 [](const double a, const double b) { return a - b; });
  std::transform(data1.EY.begin(), data1.EY.end(), data2.EY.begin(),
                 data1.EY.begin(),
                 [](const double a, const double b) { return a + b; });

  TGraphErrors graph{static_cast<Int_t>(data1.X.size()), data1.X.data(),
                        data1.Y.data(), data1.EX.data(), data1.EY.data()};
  graph.SaveAs("graph.root");
}