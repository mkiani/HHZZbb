#include "TROOT.h"
#include "TSystem.h"

#include <iostream>


void lib(){
gSystem->Load("libRooFit.so");
gSystem->Load("libZZMatrixElementMELA.so");
gSystem->Load("libHiggsAnalysisCombinedLimit.so");
gSystem->Load("libHHZZbbAnalysisStep.so");
gSystem->AddIncludePath("-I$ROOFITSYS");
gSystem->AddIncludePath("-I$CMSSW_BASE");
}
void lib();

