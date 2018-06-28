// C++
#include <iostream>
#include <fstream>
#include <string>

// ROOT
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"

// My own files
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/include/Plotter.h>
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/include/Settings.h>
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/src/setTDRStyle.cpp>

using namespace std;

int main( int argc, char *argv[] )
{
   setTDRStyle();
	
   TString path_2017 = "";
   TString path_2016 = "";
   TString file_name = "/ZZ4lAnalysis.root";
	
   TString Data_2017    = "AllData"        + file_name;
   TString Data_2016    = "AllData"        + file_name;
	
   TString ggH125_2017      = path_2017 + "ggH125"     + file_name;
   TString VBFH125_2017     = path_2017 + "VBFH125"    + file_name;
   TString WpH125_2017      = path_2017 + "WplusH125"  + file_name;
   TString WmH125_2017      = path_2017 + "WminusH125" + file_name;
   TString ZH125_2017       = path_2017 + "ZH125"      + file_name;
   TString ttH125_2017      = path_2017 + "ttH125"     + file_name;
   TString bbH125_2017      = path_2017 + "bbH125"     + file_name;
   TString tqH125_2017      = path_2017 + "tqH125"     + file_name;
	
   TString ZZTo4l_2017      = path_2017 + "ZZTo4l"                     + file_name;
   TString ggZZ4e_2017      = path_2017 + "ggTo4e_Contin_MCFM701"      + file_name;
   TString ggZZ4mu_2017     = path_2017 + "ggTo4mu_Contin_MCFM701"     + file_name;
   TString ggZZ4tau_2017    = path_2017 + "ggTo4tau_Contin_MCFM701"    + file_name;
   TString ggZZ2e2mu_2017   = path_2017 + "ggTo2e2mu_Contin_MCFM701"   + file_name;
   TString ggZZ2e2tau_2017  = path_2017 + "ggTo2e2tau_Contin_MCFM701"  + file_name;
   TString ggZZ2mu2tau_2017 = path_2017 + "ggTo2mu2tau_Contin_MCFM701" + file_name;
	
	TString ggH125_2016      = path_2016 + "ggH125"     + file_name;
   TString VBFH125_2016     = path_2016 + "VBFH125"    + file_name;
   TString WpH125_2016      = path_2016 + "WplusH125"  + file_name;
   TString WmH125_2016      = path_2016 + "WminusH125" + file_name;
   TString ZH125_2016       = path_2016 + "ZH125"      + file_name;
   TString ttH125_2016      = path_2016 + "ttH125"     + file_name;
   TString bbH125_2016      = path_2016 + "bbH125"     + file_name;
   TString tqH125_2016      = path_2016 + "tqH125"     + file_name;

   TString ZZTo4l_2016      = path_2016 + "ZZTo4l"                     + file_name;
   TString ggZZ4e_2016      = path_2016 + "ggTo4e_Contin_MCFM701"      + file_name;
   TString ggZZ4mu_2016     = path_2016 + "ggTo4mu_Contin_MCFM701"     + file_name;
   TString ggZZ4tau_2016    = path_2016 + "ggTo4tau_Contin_MCFM701"    + file_name;
   TString ggZZ2e2mu_2016   = path_2016 + "ggTo2e2mu_Contin_MCFM701"   + file_name;
   TString ggZZ2e2tau_2016  = path_2016 + "ggTo2e2tau_Contin_MCFM701"  + file_name;
   TString ggZZ2mu2tau_2016 = path_2016 + "ggTo2mu2tau_Contin_MCFM701" + file_name;

   Plotter *combination = new Plotter( );
	
	combination->FillHistograms(Data_2017,2017);
   combination->FillHistograms(ggH125_2017,2017);
   combination->FillHistograms(VBFH125_2017,2017);
   combination->FillHistograms(ZH125_2017,2017);
   combination->FillHistograms(ttH125_2017,2017);
   combination->FillHistograms(bbH125_2017,2017);
   combination->FillHistograms(tqH125_2017,2017);
   combination->FillHistograms(WpH125_2017,2017);
   combination->FillHistograms(WmH125_2017,2017);
   combination->FillHistograms(ZZTo4l_2017,2017);
   combination->FillHistograms(ggZZ4e_2017,2017);
   combination->FillHistograms(ggZZ4mu_2017,2017);
   combination->FillHistograms(ggZZ4tau_2017,2017);
   combination->FillHistograms(ggZZ2e2mu_2017,2017);
   combination->FillHistograms(ggZZ2e2tau_2017,2017);
   combination->FillHistograms(ggZZ2mu2tau_2017,2017);
	
	combination->FillHistograms(Data_2016,2016);
   combination->FillHistograms(ggH125_2016,2016);
   combination->FillHistograms(VBFH125_2016,2016);
   combination->FillHistograms(ZH125_2016,2016);
   combination->FillHistograms(ttH125_2016,2016);
   combination->FillHistograms(bbH125_2016,2016);
   combination->FillHistograms(tqH125_2016,2016);
   combination->FillHistograms(WpH125_2016,2016);
   combination->FillHistograms(WmH125_2016,2016);
   combination->FillHistograms(ZZTo4l_2016,2016);
   combination->FillHistograms(ggZZ4e_2016,2016);
   combination->FillHistograms(ggZZ4mu_2016,2016);
   combination->FillHistograms(ggZZ4tau_2016,2016);
   combination->FillHistograms(ggZZ2e2mu_2016,2016);
   combination->FillHistograms(ggZZ2e2tau_2016,2016);
   combination->FillHistograms(ggZZ2mu2tau_2016,2016);
	
	combination->FillInclusiveCombination();
	combination->Plot();
	
   delete combination;
}
