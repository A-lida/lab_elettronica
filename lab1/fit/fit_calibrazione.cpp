#include "TGraphErrors.h"
#include "TCanvas.h"


int main() {
    TCanvas c1{};
    TGraphErrors graph {"../lab1/fit/calibrazione.csv"};
    graph.Draw("AP");
    c1.SaveAs("calibrazione.png");
}