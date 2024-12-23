//
// Created by paolo on 23/12/2024.
//

#include "TPaveStats.h"
#include "TStyle.h"
#include <iostream>

#include "graphic_stuff.h"

void setStyle() {
  //   gROOT->SetStyle("Plain");
  gStyle->SetOptStat(2210);
  gStyle->SetOptFit();
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

void cutiefier(TVirtualPad *pad) {
  if (!pad) {
    std::cerr << "Pad is null" << std::endl;
    return;
  }
  gPad->Update();

  if (TPaveStats *statbox =
          static_cast<TPaveStats *>(pad->GetPrimitive("stats"))) {
    // Cambia le coordinate NDC (x1, y1, x2, y2)
    statbox->SetX1NDC(
        0.486422); // Nuova posizione X1 (vicino al bordo sinistro)
    statbox->SetX2NDC(0.986075); // Nuova posizione X2
    statbox->SetY1NDC(0.840136); // Altezza inferiore
    statbox->SetY2NDC(0.944496); // Altezza superiore
    gPad->Modified();
  } else {
    std::cerr << "Statbox non trovata!\n";
  }
}
