#ifndef CATEGORY_H
#define CATEGORY_H



//---------- RunI categorization 

enum CategoryLegacy {
  ZeroOneJet = 0,
  Dijet      = 1
};

extern "C" int categoryLegacy( int nCleanedJetsPt30 );



//---------- Moriond 2016 categorization 

enum CategoryMor16 {
  UntaggedMor16  = 0,
  VBFTaggedMor16 = 1
};

extern "C" int categoryMor16(
			     int nCleanedJetsPt30,
			     float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal
			     );



//---------- ICHEP 2016 categorization

enum CategoryIchep16 {
  UntaggedIchep16     = 0,
  VBF1jTaggedIchep16  = 1,
  VBF2jTaggedIchep16  = 2, 
  VHLeptTaggedIchep16 = 3, 
  VHHadrTaggedIchep16 = 4, 
  ttHTaggedIchep16    = 5
};

//int category(
extern "C" int categoryIchep16(
			       int nExtraLep,
			       int nExtraZ,
			       int nCleanedJetsPt30, 
			       int nCleanedJetsPt30BTagged_bTagSF,
			       float* jetQGLikelihood,
			       float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
			       float p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
			       float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
			       float p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
			       float pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
			       float p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
			       float p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
					 float p_HadWH_mavjj_JECNominal,
					 float p_HadWH_mavjj_true_JECNominal,
					 float p_HadZH_mavjj_JECNominal,
					 float p_HadZH_mavjj_true_JECNominal,
			       float* jetPhi,
			       float ZZMass,
			       bool useQGTagging = false
			       );



//---------- Moriond 2017 categorization

enum CategoryMor17 {
  UntaggedMor17     = 0,
  VBF1jTaggedMor17  = 1,
  VBF2jTaggedMor17  = 2, 
  VHLeptTaggedMor17 = 3, 
  VHHadrTaggedMor17 = 4,
  ttHTaggedMor17    = 5,
  VHMETTaggedMor17  = 6
};

//int category(
extern "C" int categoryMor17(
			     int nExtraLep,
			     int nExtraZ,
			     int nCleanedJetsPt30, 
			     int nCleanedJetsPt30BTagged_bTagSF,
			     float* jetQGLikelihood,
			     float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
			     float p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
			     float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
			     float p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
				  float p_HadWH_mavjj_JECNominal,
				  float p_HadWH_mavjj_true_JECNominal,
				  float p_HadZH_mavjj_JECNominal,
				  float p_HadZH_mavjj_true_JECNominal,
			     float* jetPhi,
			     float ZZMass,
			     float PFMET,
			     bool useVHMETTagged = true,
			     bool useQGTagging = false
			     );

//---------- Moriond 2018 categorization

enum CategoryMor18 {
  UntaggedMor18      = 0,
  VBF1jTaggedMor18   = 1,
  VBF2jTaggedMor18   = 2,
  VHLeptTaggedMor18  = 3,
  VHHadrTaggedMor18  = 4,
  ttHLeptTaggedMor18 = 5,
  ttHHadrTaggedMor18 = 6,
  VHMETTaggedMor18   = 7
};

//int category(
extern "C" int categoryMor18(
			     int nExtraLep,
			     int nExtraZ,
			     int nCleanedJetsPt30,
			     int nCleanedJetsPt30BTagged_bTagSF,
			     float* jetQGLikelihood,
			     float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
			     float p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
			     float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
			     float p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
			     float p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
				  float p_HadWH_mavjj_JECNominal,
				  float p_HadWH_mavjj_true_JECNominal,
				  float p_HadZH_mavjj_JECNominal,
				  float p_HadZH_mavjj_true_JECNominal,
			     float* jetPhi,
			     float ZZMass,
			     float PFMET,
			     bool useVHMETTagged = true,
			     bool useQGTagging = false
			     );



#endif
