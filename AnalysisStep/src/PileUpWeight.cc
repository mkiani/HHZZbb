/**
 * To prepare PU profile histograms, cf. utils/make_PU_weight_hist.py
 */


#include "HHZZbb/AnalysisStep/interface/PileUpWeight.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"


float PileUpWeight::weight(float input, PileUpWeight::PUvar var) {

  if (h_nominal != nullptr && var == PUvar::NOMINAL) {
    return h_nominal->GetBinContent(h_nominal->FindBin(input));
  } else if (h_down != nullptr && var == PUvar::VARDOWN) {
    return h_down->GetBinContent(h_down->FindBin(input));
  } else if (h_up != nullptr && var == PUvar::VARUP) {
    return h_up->GetBinContent(h_up->FindBin(input));
  } else {
    return -1.;
  }
}


PileUpWeight::PileUpWeight(int MC, int target) { 

 if (MC==2016 && target==2016)
 {
    edm::FileInPath fip("HHZZbb/AnalysisStep/data/PileUpWeights/puWeightsMoriond17_v2.root");

    TFile *fPUWeight = TFile::Open(fip.fullPath().data(),"READ");

    h_nominal.reset((TH1*)fPUWeight->Get("weights")->Clone());
    h_up.reset((TH1*)fPUWeight->Get("weights_varUp")->Clone());
    h_down.reset((TH1*)fPUWeight->Get("weights_varDn")->Clone());

    fPUWeight->Close();

 }

 else if (MC==2016 && target==2017)
 {
    edm::FileInPath fip("HHZZbb/AnalysisStep/data/PileUpWeights/puWeight_Spring2016MC_to_2017Data_294927-301141.root");

    TFile *fPUWeight = TFile::Open(fip.fullPath().data(),"READ");

    h_nominal.reset((TH1*)fPUWeight->Get("weights")->Clone());
    h_up.reset((TH1*)fPUWeight->Get("weights_varUp")->Clone());
    h_down.reset((TH1*)fPUWeight->Get("weights_varDn")->Clone());

    fPUWeight->Close();   
 }
	
 else if (MC==2017 && target==2017)
 {
		edm::FileInPath fip("HHZZbb/AnalysisStep/data/PileUpWeights/puWeightsMoriond18.root");
		
		TFile *fPUWeight = TFile::Open(fip.fullPath().data(),"READ");
		
		h_nominal.reset((TH1*)fPUWeight->Get("weights")->Clone());
		h_up.reset((TH1*)fPUWeight->Get("weights_varUp")->Clone());
		h_down.reset((TH1*)fPUWeight->Get("weights_varDn")->Clone());
		
		fPUWeight->Close();
 }
 
 else if (MC==2017 && target==2018)
 {
		edm::FileInPath fip("HHZZbb/AnalysisStep/data/PileUpWeights/puWeights_Fall2017MC_to_2018Data_314472-317591.root");
		
		TFile *fPUWeight = TFile::Open(fip.fullPath().data(),"READ");
		
		h_nominal.reset((TH1*)fPUWeight->Get("weights")->Clone());
		h_up.reset((TH1*)fPUWeight->Get("weights_varUp")->Clone());
		h_down.reset((TH1*)fPUWeight->Get("weights_varDn")->Clone());
		
		fPUWeight->Close();
 }
 
 if(h_nominal == nullptr) {
     edm::LogError("PU reweight") << "Did not find reweighting histogram to reweight MC=" << MC << " to data=" << target;
     edm::LogError("PU reweight") << "Legacy configurations are in PUReweight.cc";
 }
}