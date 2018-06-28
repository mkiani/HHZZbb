// C++
#include <iostream>
#include <fstream>
#include <string>

// ROOT
#include "TApplication.h"
#include <TROOT.h>
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"

// My own files
#include <HHZZbb/AnalysisStep/test/Plotter_v2/include/Yields.h>
#include <HHZZbb/AnalysisStep/test/Plotter_v2/include/Variables.h>
#include <HHZZbb/AnalysisStep/test/Plotter_v2/src/setTDRStyle.cpp>

using namespace std;

int main( int argc, char *argv[] )
{
   setTDRStyle();
	
   TString path = "";
   TString file_name = "/ZZ4lAnalysis.root";
   TString file_name_FR = "/FakeRates_SS_Moriond18.root";
   
   //Data
   TString Data    = "AllData"        + file_name;
   TString FakeRates   = "../../data/FakeRates" + file_name_FR;
   
   // Signal
   TString ggH120      = path + "ggH120" + file_name;
   TString ggH124      = path + "ggH124" + file_name;
   TString ggH125      = path + "ggH125" + file_name;
   TString ggH126      = path + "ggH126" + file_name;
   TString ggH130      = path + "ggH130" + file_name;
               
   TString VBFH120     = path + "VBFH120" + file_name;
   TString VBFH124     = path + "VBFH124" + file_name;
   TString VBFH125     = path + "VBFH125" + file_name;
   TString VBFH126     = path + "VBFH126" + file_name;
   TString VBFH130     = path + "VBFH130" + file_name;
   
   TString WpH120      = path + "WplusH120" + file_name;
   TString WpH124      = path + "WplusH124" + file_name;
   TString WpH125      = path + "WplusH125" + file_name;
   TString WpH126      = path + "WplusH126" + file_name;
   TString WpH130      = path + "WplusH130" + file_name;
    
   TString WmH120      = path + "WminusH120" + file_name;
   TString WmH124      = path + "WminusH124" + file_name;
   TString WmH125      = path + "WminusH125" + file_name;
   TString WmH126      = path + "WminusH126" + file_name;
   TString WmH130      = path + "WminusH130" + file_name;
      
   TString ZH120       = path + "ZH120" + file_name;
   TString ZH124       = path + "ZH124" + file_name;
   TString ZH125       = path + "ZH125" + file_name;
   TString ZH126       = path + "ZH126" + file_name;
   TString ZH130       = path + "ZH130" + file_name;
   
   TString ttH120      = path + "ttH120" + file_name;
   TString ttH124      = path + "ttH124" + file_name;
   TString ttH125      = path + "ttH125" + file_name;
   TString ttH126      = path + "ttH126" + file_name;
   TString ttH130      = path + "ttH130" + file_name;
	
	TString bbH120      = path + "bbH120" + file_name;
   TString bbH124      = path + "bbH124" + file_name;
   TString bbH125      = path + "bbH125" + file_name;
   TString bbH126      = path + "bbH126" + file_name;
   TString bbH130      = path + "bbH130" + file_name;
	
   TString tqH125      = path + "tqH125" + file_name;

   // Backgrounds
   TString ZZTo4l      = path + "ZZTo4l" + file_name;
   TString ggZZ4e      = path + "ggTo4e_Contin_MCFM701" + file_name;
   TString ggZZ4mu     = path + "ggTo4mu_Contin_MCFM701" + file_name;
   TString ggZZ4tau    = path + "ggTo4tau_Contin_MCFM701" + file_name;
   TString ggZZ2e2mu   = path + "ggTo2e2mu_Contin_MCFM701" + file_name;
   TString ggZZ2e2tau  = path + "ggTo2e2tau_Contin_MCFM701" + file_name;
   TString ggZZ2mu2tau = path + "ggTo2mu2tau_Contin_MCFM701" + file_name;
   
   Yields *yields = new Yields( 41.53 );
   
//===============
// Produce plots 
//===============
   
   yields->MakeHistograms(Data);

   yields->MakeHistograms(ggH120);
   yields->MakeHistograms(ggH124);
   yields->MakeHistograms(ggH125);
   yields->MakeHistograms(ggH126);
   yields->MakeHistograms(ggH130);

   yields->MakeHistograms(VBFH120);
   yields->MakeHistograms(VBFH124);
   yields->MakeHistograms(VBFH125);
   yields->MakeHistograms(VBFH126);
   yields->MakeHistograms(VBFH130);

   yields->MakeHistograms(ZH120);
   yields->MakeHistograms(ZH124);
   yields->MakeHistograms(ZH125);
   yields->MakeHistograms(ZH126);
   yields->MakeHistograms(ZH130);

   yields->MakeHistograms(WpH120);
   yields->MakeHistograms(WpH124);
   yields->MakeHistograms(WpH125);
   yields->MakeHistograms(WpH126);
   yields->MakeHistograms(WpH130);

   yields->MakeHistograms(WmH120);
   yields->MakeHistograms(WmH124);
   yields->MakeHistograms(WmH125);
   yields->MakeHistograms(WmH126);
   yields->MakeHistograms(WmH130);

   yields->MakeHistograms(ttH120);
   yields->MakeHistograms(ttH124);
   yields->MakeHistograms(ttH125);
   yields->MakeHistograms(ttH126);
   yields->MakeHistograms(ttH130);

//	yields->MakeHistograms(bbH120);
   yields->MakeHistograms(bbH124);
   yields->MakeHistograms(bbH125);
   yields->MakeHistograms(bbH126);
   yields->MakeHistograms(bbH130);

   yields->MakeHistograms(tqH125);

   yields->MakeHistograms(ZZTo4l);
   yields->MakeHistograms(ggZZ4e);
   yields->MakeHistograms(ggZZ4mu);
   yields->MakeHistograms(ggZZ4tau);
   yields->MakeHistograms(ggZZ2e2mu);
   yields->MakeHistograms(ggZZ2e2tau);
   yields->MakeHistograms(ggZZ2mu2tau);

   yields->FillInclusive();

   yields->Save();
	
//==============
// Print Yields
//==============
	
   yields->GetHistos("Yields");
   yields->Calculate_SS_ZX_Yields( Data, FakeRates);
   yields->Print("Yields");

   yields->Print("Yields", 118., 130.);
   yields->Print("Yields", 105., 140.);

   yields->PrintLatexTables("Yields", 118., 130.);
   yields->FillGraphs("Yields", 105., 140., "Q");
   yields->PrepareYamlFiles("Yields", "13", 105., 140.);
	
//==========================================
// Produce data ROOT files for datacard maker
//==========================================
	yields->ProduceDataROOTFiles( Data, "DataROOTFiles" );
	
	
   delete yields;
}
