//
// Created by pi on 12/24/24.
//

#include "ROOT/RCsvDS.hxx"
#include "TVirtualPad.h"
#include <algorithm>
#include <iostream>

#include "data_handling.hpp"

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
                  if (x > 0.61) {
                    GraphInitializers &selectedVectors =
                        minX <= x && x <= maxX ? fit : noFit;
                    selectedVectors.X.push_back(x);
                    selectedVectors.Y.push_back(allData.Y[i]);
                    selectedVectors.EX.push_back(allData.EX[i]);
                    selectedVectors.EY.push_back(allData.EY[i]);
                  }
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
