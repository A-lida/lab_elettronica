//
// Created by paolo on 23/12/2024.
//

#ifndef HEADER_HPP
#define HEADER_HPP

#include <vector>

struct GraphInitializers {
  std::vector<double> X{};
  std::vector<double> Y{};
  std::vector<double> EX{};
  std::vector<double> EY{};
};

void readCsv(const char *source_file, GraphInitializers &data);

void filterData(const GraphInitializers &allData, GraphInitializers &fit,
                GraphInitializers &noFit, const double minX,
                const double maxX);

#endif //HEADER_HPP
