#ifndef PUReweight_h
#define PUReweight_h

/** \class PUReweight
 *
 *  Implement PU reweighting.
 *
 */

#include <TH1F.h>
#include <string>

class PUReweight {
public:
  enum Type {OLDICHEP=0, ICHEP=1, HCP=2, MORIOND=3, LEGACY=4, RUN2ANALYSIS=5};

  /// Constructor. 
  /// type=OLDICHEP are the weights to be used for the original ICHEP samples
  ///      ICHEP   = for HCP samples to reproduce ICHEP conditions
  ///      HCP     =         "                    HCP   
  ///      MORIOND =         "                    MORIOND " 
  ///      LEGACY  =         "                    LEGACY  " 
  PUReweight(Type type=RUN2ANALYSIS); 

  ~PUReweight();

  /// Get weights for a sample of MC=2011 or 2012, 
  /// to reproduce conditions of target = 2011 or 2012;
  float weight(int MC, int target, float input);


public:
  Type theType;

  // ICHEP Samples
  TH1F* hT2012; 
  TH1F* hT2011;
  TH1F* hT2011To2012;

  // HCP Samples
  TH1F* hTPuV07ToIchep52X;
  TH1F* hTPuV10ToIchep53X;
  TH1F*	hTPuV07ToHcp53X;
  TH1F* hTPuV10ToHcp53X;
  TH1F* hTPuToMoriond13;
  TH1F* hTPuToLegacy13;
  
  // 2015
  TH1F* hTPuTo2015;
  TH1F* hTPuTo201576X;

  // 2016
  TH1F* hTPuToIchep16;
   
  // 2017
  TH1F* hTPuToMoriond17;
};
#endif
