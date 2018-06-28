// Include classes
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/include/Plotter.h>

// Constructor
//============================================================
Plotter::Plotter( double lumi ):Tree()
{
   unblinded_histos = new Histograms(lumi, "Unblinded");
   blinded_histos = new Histograms(lumi, "Blinded");
   histo_map["Unblinded"] = unblinded_histos;
   histo_map["Blinded"] = blinded_histos;
      
   _lumi = lumi;
   _merge_2e2mu = true;
   _current_process = -999;
   _k_factor = 1;
   _current_final_state = -999;
   _current_category = -999;

   
   // Z+X SS factors
   _fs_ROS_SS.push_back(1.00868);//4e
   _fs_ROS_SS.push_back(1.04015);//4mu
   _fs_ROS_SS.push_back(1.00823);//2e2mu
   _fs_ROS_SS.push_back(1.0049);//2mu2e
   
   vector<float> temp;
   for ( int i_fs = 0; i_fs < num_of_final_states; i_fs++ )
   {
      for ( int i_cat = 0; i_cat < num_of_categories; i_cat++ )
      {
         temp.push_back(0);
      }
      _expected_yield_SR.push_back(temp);
      _number_of_events_CR.push_back(temp);
   }
}
//============================================================

// Constructor
//============================================================
Plotter::Plotter():Tree()
{
   combination_histos = new Histograms("Combination");
	
}
//============================================================




// Destructor
//====================
Plotter::~Plotter()
{
}
//====================



//=====================================================
void Plotter::MakeHistograms( TString input_file_name )
{

   input_file = new TFile(input_file_name);
   
   hCounters = (TH1F*)input_file->Get("ZZTree/Counters");
   n_gen_events = (Long64_t)hCounters->GetBinContent(1);
   gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
   
   input_tree = (TTree*)input_file->Get("ZZTree/candTree");
   Init( input_tree, input_file_name );
   
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
   
      // Check number of leptons in event
      if ( LepEta->size() != 4 )
      {
         cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
         continue;
      }

      if ( !(ZZsel >= 90) ) continue;
		

      // Find current process
      gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
      _n_gen_assoc_lep = CountAssociatedLeptons();
      _current_process = find_current_process( input_file_name, genExtInfo , _n_gen_assoc_lep);
      gen_assoc_lep_id_.clear();
   
      // Final states
      _current_final_state = FindFinalState();
   
      // Find current category
      for ( int j = 0; j < nCleanedJetsPt30; j++)
      {
         jetPt[j] = JetPt->at(j);
         jetEta[j] = JetEta->at(j);
         jetPhi[j] = JetPhi->at(j);
         jetMass[j] = JetMass->at(j);
         jetQGL[j] = JetQGLikelihood->at(j);
         jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
      }

      _current_category = categoryMor18(nExtraLep,
													 nExtraZ,
													 nCleanedJetsPt30,
													 nCleanedJetsPt30BTagged_bTagSF,
													 jetQGL,
                                        p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
													 p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
													 p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
                                        p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
													 pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
													 p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
                                        p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
													 p_HadWH_mavjj_JECNominal,
													 p_HadWH_mavjj_true_JECNominal,
													 p_HadZH_mavjj_JECNominal,
													 p_HadZH_mavjj_true_JECNominal,
													 jetPhi,
													 ZZMass,
													 PFMET,
													 false,// Use VHMET category
													 false);// Use QG tagging
   
      // K factors
      _k_factor = calculate_K_factor(input_file_name);
   
      // Final event weight
      _event_weight = (_lumi * 1000 * xsec * _k_factor * overallEventWeight) / gen_sum_weights;
      if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS
      
      // Calculate kinematic discriminants
      KD = p_GG_SIG_ghg2_1_ghz1_1_JHUGen / ( p_GG_SIG_ghg2_1_ghz1_1_JHUGen + p_QQB_BKG_MCFM*getDbkgkinConstant(Z1Flav*Z2Flav,ZZMass) );
		
      DVBFDEC = (nCleanedJetsPt30>=2) ? D_bkg_VBFdec( p_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	p_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	p_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	p_JJVBF_BKG_MCFM_JECNominal,
     																	p_HadZH_BKG_MCFM_JECNominal,
																		p_HadWH_BKG_MCFM_JECNominal,
     																	p_JJQCD_BKG_MCFM_JECNominal,
																		p_HadZH_mavjj_JECNominal,
     																	p_HadZH_mavjj_true_JECNominal,
     																	p_HadWH_mavjj_JECNominal,
     																	p_HadWH_mavjj_true_JECNominal,
     																	pConst_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	pConst_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	pConst_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	pConst_JJVBF_BKG_MCFM_JECNominal,
     																	pConst_HadZH_BKG_MCFM_JECNominal,
     																	pConst_HadWH_BKG_MCFM_JECNominal,
     																	pConst_JJQCD_BKG_MCFM_JECNominal,
	    																Z1Flav*Z2Flav,
     																	ZZMass) : -2;
		
      DVHDEC = (nCleanedJetsPt30>=2) ?   D_bkg_VHdec( p_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	p_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	p_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	p_JJVBF_BKG_MCFM_JECNominal,
     																	p_HadZH_BKG_MCFM_JECNominal,
																		p_HadWH_BKG_MCFM_JECNominal,
     																	p_JJQCD_BKG_MCFM_JECNominal,
																		p_HadZH_mavjj_JECNominal,
     																	p_HadZH_mavjj_true_JECNominal,
     																	p_HadWH_mavjj_JECNominal,
     																	p_HadWH_mavjj_true_JECNominal,
     																	pConst_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	pConst_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	pConst_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	pConst_JJVBF_BKG_MCFM_JECNominal,
     																	pConst_HadZH_BKG_MCFM_JECNominal,
     																	pConst_HadWH_BKG_MCFM_JECNominal,
     																	pConst_JJQCD_BKG_MCFM_JECNominal,
	    																Z1Flav*Z2Flav,
     																	ZZMass) : -2;
		
      D2jet = ( nCleanedJetsPt30 >= 2)  ? DVBF2j_ME(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass) : -2;
      D1jet = ( nCleanedJetsPt30 == 1 ) ? DVBF1j_ME(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass) : -2;
      DWH =   ( nCleanedJetsPt30 >= 2 ) ? DWHh_ME(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass) : -2;
      DZH =   ( nCleanedJetsPt30 >= 2 ) ? DZHh_ME(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass) : -2;
      
      
      float oldCConstD2jet = getDVBF2jetsConstant(ZZMass);
      float oldCConstD1jet = getDVBF1jetConstant(ZZMass);
      float oldCConstDWH = getDWHhConstant(ZZMass);
      float oldCConstDZH = getDZHhConstant(ZZMass);
      float newCConstD2jet = getDVBF2jetsConstant_shiftWP(ZZMass,false,NEWWP2J);
      float newCConstD1jet = getDVBF1jetConstant_shiftWP(ZZMass,false,NEWWP1J);
      float newCConstDWH = getDWHhConstant_shiftWP(ZZMass,false,NEWWPWH);
      float newCConstDZH = getDZHhConstant_shiftWP(ZZMass,false,NEWWPZH);
      D2jet = 1/(newCConstD2jet/oldCConstD2jet*(1/D2jet-1)+1);
      D1jet = 1/(newCConstD1jet/oldCConstD1jet*(1/D1jet-1)+1);
      DWH = 1/(newCConstDWH/oldCConstDWH*(1/DWH-1)+1);
      DZH = 1/(newCConstDZH/oldCConstDZH*(1/DZH-1)+1);
      float DVH = max(DWH,DZH);
           
      
      // Fill M4l histograms
      if ( (_current_process == Settings::Data && blind(ZZMass)) || _current_process != Settings::Data )
      {
         blinded_histos->FillM4l( ZZMass, _event_weight, _current_final_state, _current_category, _current_process );
      }
       
       unblinded_histos->FillM4l( ZZMass, _event_weight, _current_final_state, _current_category, _current_process );
      
      // Fill MZ1 histograms
      if ( blind(ZZMass) )
      {
         blinded_histos->FillMZ1( ZZMass, Z1Mass, _event_weight, _current_final_state, _current_category, _current_process );
      }
      
      unblinded_histos->FillMZ1( ZZMass, Z1Mass, _event_weight, _current_final_state, _current_category, _current_process );
      
      // Fill MZ2 histograms
      if ( blind(ZZMass) )
      {
         blinded_histos->FillMZ2( ZZMass, Z2Mass, _event_weight, _current_final_state, _current_category, _current_process );
      }
      
      unblinded_histos->FillMZ2( ZZMass, Z2Mass, _event_weight, _current_final_state, _current_category, _current_process );
      
      // Fill KD histograms
      if ( blind(ZZMass) )
      {
         blinded_histos->FillKD( ZZMass, KD, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 >=2 ) blinded_histos->FillDVBFDEC( ZZMass, DVBFDEC, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 >=2 ) blinded_histos->FillDVHDEC( ZZMass, DVHDEC, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 ==1 ) blinded_histos->FillD1jet( ZZMass, D1jet, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 >=2 ) blinded_histos->FillD2jet( ZZMass, D2jet, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 >=2 ) blinded_histos->FillDWH( ZZMass, DWH, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 >=2 ) blinded_histos->FillDZH( ZZMass, DZH, _event_weight, _current_final_state, _current_category, _current_process );
         if ( nCleanedJetsPt30 >=2 ) blinded_histos->FillDVH( ZZMass, DVH, _event_weight, _current_final_state, _current_category, _current_process );
      }
      
      unblinded_histos->FillKD( ZZMass, KD, _event_weight, _current_final_state, _current_category, _current_process );
		if ( nCleanedJetsPt30 >=2 ) unblinded_histos->FillDVBFDEC( ZZMass, DVBFDEC, _event_weight, _current_final_state, _current_category, _current_process );
		if ( nCleanedJetsPt30 >=2 ) unblinded_histos->FillDVHDEC( ZZMass, DVHDEC, _event_weight, _current_final_state, _current_category, _current_process );
      if ( nCleanedJetsPt30 ==1 ) unblinded_histos->FillD1jet( ZZMass, D1jet, _event_weight, _current_final_state, _current_category, _current_process );
      if ( nCleanedJetsPt30 >=2 ) unblinded_histos->FillD2jet( ZZMass, D2jet, _event_weight, _current_final_state, _current_category, _current_process );
      if ( nCleanedJetsPt30 >=2 ) unblinded_histos->FillDWH( ZZMass, DWH, _event_weight, _current_final_state, _current_category, _current_process );
      if ( nCleanedJetsPt30 >=2 ) unblinded_histos->FillDZH( ZZMass, DZH, _event_weight, _current_final_state, _current_category, _current_process );
      if ( nCleanedJetsPt30 >=2 ) unblinded_histos->FillDVH( ZZMass, DVH, _event_weight, _current_final_state, _current_category, _current_process );
      
      // Fill MZ1 vs MZ2 histograms
      if ( blind(ZZMass) )
      {
         blinded_histos->FillMZ1vsMZ2( ZZMass, Z1Mass, Z2Mass, _event_weight, _current_final_state, _current_category, _current_process );
      }
      
      unblinded_histos->FillMZ1vsMZ2( ZZMass, Z1Mass, Z2Mass, _event_weight, _current_final_state, _current_category, _current_process );
      
      // Fill 2D histograms vs M4l with error
      if ( blind(ZZMass) || _current_process != Settings::Data )
      {
         if (_current_process == Settings::Data)
         {
            blinded_histos->FillVectors( ZZMass, ZZMassErrCorr, KD, DVBFDEC, DVHDEC,nCleanedJetsPt30, D1jet, D2jet, DWH, DZH, DVH, _current_final_state, _current_category);
         }
      
         blinded_histos->FillDvsM4l( ZZMass, KD, DVBFDEC, DVHDEC, nCleanedJetsPt30, D1jet, D2jet, DWH, DZH, DVH, _event_weight, _current_final_state, _current_category, _current_process );
      }
      
      if (_current_process == Settings::Data)
      {
         unblinded_histos->FillVectors( ZZMass, ZZMassErrCorr, KD, DVBFDEC, DVHDEC, nCleanedJetsPt30, D1jet, D2jet, DWH, DZH, DVH, _current_final_state, _current_category );
      }
		
      unblinded_histos->FillDvsM4l( ZZMass, KD, DVBFDEC, DVHDEC, nCleanedJetsPt30, D1jet, D2jet, DWH, DZH, DVH, _event_weight, _current_final_state, _current_category, _current_process );

		int leading_id = -9;
		int trailing_id = -9;

		if(LepPt->at(0) > LepPt->at(1) && LepPt->at(0) > LepPt->at(2) && LepPt->at(0) > LepPt->at(3)) leading_id=0;
		if(LepPt->at(1) > LepPt->at(0) && LepPt->at(1) > LepPt->at(2) && LepPt->at(1) > LepPt->at(3)) leading_id=1;
		if(LepPt->at(2) > LepPt->at(0) && LepPt->at(2) > LepPt->at(1) && LepPt->at(2) > LepPt->at(3)) leading_id=2;
		if(LepPt->at(3) > LepPt->at(0) && LepPt->at(3) > LepPt->at(1) && LepPt->at(3) > LepPt->at(2)) leading_id=3;

		if(LepPt->at(0) < LepPt->at(1) && LepPt->at(0) < LepPt->at(2) && LepPt->at(0) < LepPt->at(3)) trailing_id=0;
		if(LepPt->at(1) < LepPt->at(0) && LepPt->at(1) < LepPt->at(2) && LepPt->at(1) < LepPt->at(3)) trailing_id=1;
		if(LepPt->at(2) < LepPt->at(0) && LepPt->at(2) < LepPt->at(1) && LepPt->at(2) < LepPt->at(3)) trailing_id=2;
		if(LepPt->at(3) < LepPt->at(0) && LepPt->at(3) < LepPt->at(1) && LepPt->at(3) < LepPt->at(2)) trailing_id=3;

		if(LepPt->at(2) > LepPt->at(3)) {leading_id=2;trailing_id=3;}
		if(LepPt->at(3) > LepPt->at(2)) {leading_id=3;trailing_id=2;}

		Pt_leading  = LepPt->at(leading_id);
		Pt_trailing = LepPt->at(trailing_id);

		Eta_leading  = LepEta->at(leading_id);
		Eta_trailing = LepEta->at(trailing_id);

		SIP_leading  = LepSIP->at(leading_id);
		SIP_trailing = LepSIP->at(trailing_id);

		ISO_leading  = LepCombRelIsoPF->at(leading_id);
		ISO_trailing = LepCombRelIsoPF->at(trailing_id);
		
		// Fill other histograms
      if ( blind(ZZMass) )
      {
         blinded_histos->FillOthers( ZZMass, ZZPt, ZZEta, PFMET, Pt_leading, Pt_trailing, Eta_leading, Eta_trailing, SIP_leading, SIP_trailing, ISO_leading, ISO_trailing, nExtraLep, nCleanedJetsPt30, nCleanedJetsPt30BTagged_bTagSF, KD, DVBFDEC, DVHDEC, _event_weight, _current_final_state, _current_category, _current_process );
      }
		
          unblinded_histos->FillOthers( ZZMass, ZZPt, ZZEta, PFMET, Pt_leading, Pt_trailing, Eta_leading, Eta_trailing, SIP_leading, SIP_trailing, ISO_leading, ISO_trailing, nExtraLep, nCleanedJetsPt30, nCleanedJetsPt30BTagged_bTagSF, KD, DVBFDEC, DVHDEC, _event_weight, _current_final_state, _current_category, _current_process );
	
		
      
   } // end for loop
   
   cout << "[INFO] Histograms for " << input_file_name << " filled." << endl;
}
//=====================================================


//=====================================================
void Plotter::FillHistograms( TString input_file_name , int year)
{

   input_file = new TFile(input_file_name);
	
   _current_process = find_current_process( input_file_name, 0 , 0);
	
   if(year == 2017) _lumi = 41.37;
   if(year == 2016) _lumi = 35.86706;
	
   hCounters = (TH1F*)input_file->Get("ZZTree/Counters");
   n_gen_events = (Long64_t)hCounters->GetBinContent(1);
   gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
	
   input_tree = (TTree*)input_file->Get("ZZTree/candTree");
   Init( input_tree, input_file_name );
	
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
	
   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
		
      // Check number of leptons in event
      if ( LepEta->size() != 4 )
      {
         cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
         continue;
      }

      if ( !(ZZsel >= 90) ) continue;
		
		
      // K factors
      _k_factor = calculate_K_factor(input_file_name);
		
      // Final event weight
      _event_weight = (_lumi * 1000 * xsec * _k_factor * overallEventWeight) / gen_sum_weights;
      if ( input_file_name.Contains("ggH") && year == 2017) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

		combination_histos->FillM4lCombination(ZZMass, _event_weight, _current_process);
		
		
   } // end for loop
	
   cout << "[INFO] Combination histograms for " << input_file_name << " filled." << endl;
}
//=====================================================


//=======================
void Plotter::MakeM4lZX()
{

     for (int i_cat = 0; i_cat < num_of_categories; i_cat++)
     {
         blinded_histos->MakeZXShape( _expected_yield_SR, i_cat );
         unblinded_histos->MakeZXShape( _expected_yield_SR, i_cat);
     }
   
   cout << "[INFO] Z+X shape for M4l done." << endl;
}
//=======================

//===================================================================
void Plotter::Split_2e2mu()
{
	_merge_2e2mu = false;
	blinded_histos->Split_2e2mu();
	unblinded_histos->Split_2e2mu();
}
//===================================================================

//===================================================================
void Plotter::SetBlinding(float blinding_lower, float blinding_upper)
{
   _blinding_lower[0] = blinding_lower;
   _blinding_upper[0] = blinding_upper;
   _blinding_lower[1] = 0.;
   _blinding_upper[1] = 0.;
}
//===================================================================



//===================================================================
void Plotter::SetBlinding(float blinding_lower_0, float blinding_upper_0, float blinding_lower_1, float blinding_upper_1)
{
   _blinding_lower[0] = blinding_lower_0;
   _blinding_upper[0] = blinding_upper_0;
   _blinding_lower[1] = blinding_lower_1;
   _blinding_upper[1] = blinding_upper_1;
}
//===================================================================



//===============================================================================
void Plotter::MakeHistogramsZX( TString input_file_data_name, TString  input_file_FR_name )
{
   
   FakeRates *FR = new FakeRates( input_file_FR_name );

   input_file_data = new TFile(input_file_data_name);
   input_tree_data = (TTree*)input_file_data->Get("CRZLLTree/candTree");
   Init( input_tree_data, input_file_data_name );

   
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
   
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
   
      if ( !CRflag ) continue;
      if ( !test_bit(CRflag, CRZLLss) ) continue;
   
      _current_final_state = FindFinalStateZX();
		
		_current_category = categoryMor18(nExtraLep,
													 nExtraZ,
													 nCleanedJetsPt30,
													 nCleanedJetsPt30BTagged_bTagSF,
													 jetQGL,
													 p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
													 p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
													 p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
													 p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
													 pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
													 p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
													 p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
													 p_HadWH_mavjj_JECNominal,
													 p_HadWH_mavjj_true_JECNominal,
													 p_HadZH_mavjj_JECNominal,
													 p_HadZH_mavjj_true_JECNominal,
													 jetPhi,
													 ZZMass,
													 PFMET,
													 false,// Use VHMET category
													 false);// Use QG tagging
      
   
      // Calculate yield
      _yield_SR = _fs_ROS_SS.at(_current_final_state)*FR->GetFakeRate(LepPt->at(2),LepEta->at(2),LepLepId->at(2))*FR->GetFakeRate(LepPt->at(3),LepEta->at(3),LepLepId->at(3));
      
      _expected_yield_SR[_current_final_state][_current_category] += _yield_SR; // this number needs to be used when renormalizing histograms that have some cut/blinding
      _number_of_events_CR[_current_final_state][_current_category]++;
      
      if ( _merge_2e2mu && _current_final_state == Settings::fs2mu2e ) _current_final_state = Settings::fs2e2mu; //We can only do this after _yield_SR is calculated
      
      // Calculate kinematic discriminants
      KD = p_GG_SIG_ghg2_1_ghz1_1_JHUGen / ( p_GG_SIG_ghg2_1_ghz1_1_JHUGen + p_QQB_BKG_MCFM*getDbkgkinConstant(Z1Flav*Z2Flav,ZZMass) );
		
      DVBFDEC = (nCleanedJetsPt30>=2) ? D_bkg_VBFdec( p_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	p_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	p_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	p_JJVBF_BKG_MCFM_JECNominal,
     																	p_HadZH_BKG_MCFM_JECNominal,
																		p_HadWH_BKG_MCFM_JECNominal,
     																	p_JJQCD_BKG_MCFM_JECNominal,
																		p_HadZH_mavjj_JECNominal,
     																	p_HadZH_mavjj_true_JECNominal,
     																	p_HadWH_mavjj_JECNominal,
     																	p_HadWH_mavjj_true_JECNominal,
     																	pConst_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	pConst_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	pConst_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	pConst_JJVBF_BKG_MCFM_JECNominal,
     																	pConst_HadZH_BKG_MCFM_JECNominal,
     																	pConst_HadWH_BKG_MCFM_JECNominal,
     																	pConst_JJQCD_BKG_MCFM_JECNominal,
	    																Z1Flav*Z2Flav,
     																	ZZMass) : -2;
		
      DVHDEC = (nCleanedJetsPt30>=2) ?   D_bkg_VHdec( p_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	p_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	p_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	p_JJVBF_BKG_MCFM_JECNominal,
     																	p_HadZH_BKG_MCFM_JECNominal,
																		p_HadWH_BKG_MCFM_JECNominal,
     																	p_JJQCD_BKG_MCFM_JECNominal,
																		p_HadZH_mavjj_JECNominal,
     																	p_HadZH_mavjj_true_JECNominal,
     																	p_HadWH_mavjj_JECNominal,
     																	p_HadWH_mavjj_true_JECNominal,
     																	pConst_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
     																	pConst_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
     																	pConst_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
     																	pConst_JJVBF_BKG_MCFM_JECNominal,
     																	pConst_HadZH_BKG_MCFM_JECNominal,
     																	pConst_HadWH_BKG_MCFM_JECNominal,
     																	pConst_JJQCD_BKG_MCFM_JECNominal,
	    																Z1Flav*Z2Flav,
     																	ZZMass) : -2;
		
      D2jet = (nCleanedJetsPt30>=2) ? DVBF2j_ME(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass) : -2 ;
      D1jet = (nCleanedJetsPt30==1) ? DVBF1j_ME(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass) : -2 ;
      DWH = (nCleanedJetsPt30>=2) ? DWHh_ME(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass) : -2 ;
      DZH = (nCleanedJetsPt30>=2) ? DZHh_ME(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadZH_mavjj_true_JECNominal, ZZMass) : -2 ;
      
      
      float oldCConstD2jet = getDVBF2jetsConstant(ZZMass);
      float oldCConstD1jet = getDVBF1jetConstant(ZZMass);
      float oldCConstDWH = getDWHhConstant(ZZMass);
      float oldCConstDZH = getDZHhConstant(ZZMass);
      float newCConstD2jet = getDVBF2jetsConstant_shiftWP(ZZMass,false,NEWWP2J);
      float newCConstD1jet = getDVBF1jetConstant_shiftWP(ZZMass,false,NEWWP1J);
      float newCConstDWH = getDWHhConstant_shiftWP(ZZMass,false,NEWWPWH);
      float newCConstDZH = getDZHhConstant_shiftWP(ZZMass,false,NEWWPZH);
      D2jet = 1/(newCConstD2jet/oldCConstD2jet*(1/D2jet-1)+1);
      D1jet = 1/(newCConstD1jet/oldCConstD1jet*(1/D1jet-1)+1);
      DWH = 1/(newCConstDWH/oldCConstDWH*(1/DWH-1)+1);
      DZH = 1/(newCConstDZH/oldCConstDZH*(1/DZH-1)+1);
      float DVH = max(DWH,DZH);
      
   
      // Fill m4l Z+X histograms
      unblinded_histos->FillM4lZX( ZZMass, _yield_SR, _current_final_state, _current_category );
      blinded_histos->FillM4lZX( ZZMass, _yield_SR, _current_final_state, _current_category);
      
      // Fill mZ1 Z+X histograms
      unblinded_histos->FillMZ1ZX( ZZMass, Z1Mass, _yield_SR, _current_final_state, _current_category );
      
      if (blind(ZZMass))
      {
         blinded_histos->FillMZ1ZX( ZZMass, Z1Mass, _yield_SR, _current_final_state, _current_category);
      }
      
      // Fill mZ2 Z+X histograms
      unblinded_histos->FillMZ2ZX( ZZMass, Z2Mass, _yield_SR, _current_final_state, _current_category );
      
      if (blind(ZZMass))
      {
         blinded_histos->FillMZ2ZX( ZZMass, Z2Mass, _yield_SR, _current_final_state, _current_category);
      }
      
      // Fill KD Z+X histograms
      unblinded_histos->FillKDZX( ZZMass, KD, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 >= 2 ) unblinded_histos->FillDVBFDECZX( ZZMass, DVBFDEC, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 >= 2 ) unblinded_histos->FillDVHDECZX( ZZMass, DVHDEC, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 == 1 ) unblinded_histos->FillD1jetZX( ZZMass, D1jet, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 >= 2 ) unblinded_histos->FillD2jetZX( ZZMass, D2jet, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 >= 2 ) unblinded_histos->FillDWHZX( ZZMass, DWH, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 >= 2 ) unblinded_histos->FillDZHZX( ZZMass, DZH, _yield_SR, _current_final_state, _current_category );
      if ( nCleanedJetsPt30 >= 2 ) unblinded_histos->FillDVHZX( ZZMass, DVH, _yield_SR, _current_final_state, _current_category );
      
      if (blind(ZZMass))
      {
         blinded_histos->FillKDZX( ZZMass, KD, _yield_SR, _current_final_state, _current_category);
			if ( nCleanedJetsPt30 >= 2 ) blinded_histos->FillDVBFDECZX( ZZMass, DVBFDEC, _yield_SR, _current_final_state, _current_category );
         if ( nCleanedJetsPt30 >= 2 ) blinded_histos->FillDVHDECZX( ZZMass, DVHDEC, _yield_SR, _current_final_state, _current_category );
         if ( nCleanedJetsPt30 == 1 ) blinded_histos->FillD1jetZX( ZZMass, D1jet, _yield_SR, _current_final_state, _current_category);
         if ( nCleanedJetsPt30 >= 2 ) blinded_histos->FillD2jetZX( ZZMass, D2jet, _yield_SR, _current_final_state, _current_category);
         if ( nCleanedJetsPt30 >= 2 ) blinded_histos->FillDWHZX( ZZMass, DWH, _yield_SR, _current_final_state, _current_category);
         if ( nCleanedJetsPt30 >= 2 ) blinded_histos->FillDZHZX( ZZMass, DZH, _yield_SR, _current_final_state, _current_category);
         if ( nCleanedJetsPt30 >= 2 ) blinded_histos->FillDVHZX( ZZMass, DVH, _yield_SR, _current_final_state, _current_category );
      }
		
		int leading_id = -9;
		int trailing_id = -9;

		if(LepPt->at(0) > LepPt->at(1) && LepPt->at(0) > LepPt->at(2) && LepPt->at(0) > LepPt->at(3)) leading_id=0;
		if(LepPt->at(1) > LepPt->at(0) && LepPt->at(1) > LepPt->at(2) && LepPt->at(1) > LepPt->at(3)) leading_id=1;
		if(LepPt->at(2) > LepPt->at(0) && LepPt->at(2) > LepPt->at(1) && LepPt->at(2) > LepPt->at(3)) leading_id=2;
		if(LepPt->at(3) > LepPt->at(0) && LepPt->at(3) > LepPt->at(1) && LepPt->at(3) > LepPt->at(2)) leading_id=3;

		if(LepPt->at(0) < LepPt->at(1) && LepPt->at(0) < LepPt->at(2) && LepPt->at(0) < LepPt->at(3)) trailing_id=0;
		if(LepPt->at(1) < LepPt->at(0) && LepPt->at(1) < LepPt->at(2) && LepPt->at(1) < LepPt->at(3)) trailing_id=1;
		if(LepPt->at(2) < LepPt->at(0) && LepPt->at(2) < LepPt->at(1) && LepPt->at(2) < LepPt->at(3)) trailing_id=2;
		if(LepPt->at(3) < LepPt->at(0) && LepPt->at(3) < LepPt->at(1) && LepPt->at(3) < LepPt->at(2)) trailing_id=3;
		
		if(LepPt->at(2) > LepPt->at(3)) {leading_id=2;trailing_id=3;}
		if(LepPt->at(3) > LepPt->at(2)) {leading_id=3;trailing_id=2;}

		Pt_leading  = LepPt->at(leading_id);
		Pt_trailing = LepPt->at(trailing_id);

		Eta_leading  = LepEta->at(leading_id);
		Eta_trailing = LepEta->at(trailing_id);

		SIP_leading  = LepSIP->at(leading_id);
		SIP_trailing = LepSIP->at(trailing_id);

		ISO_leading  = LepCombRelIsoPF->at(leading_id);
		ISO_trailing = LepCombRelIsoPF->at(trailing_id);
		
		// Fill other histograms
      if ( blind(ZZMass) )
      {
         blinded_histos->FillOthersZX( ZZMass, ZZPt, ZZEta, PFMET, Pt_leading, Pt_trailing, Eta_leading, Eta_trailing, SIP_leading, SIP_trailing, ISO_leading, ISO_trailing, nExtraLep, nCleanedJetsPt30, nCleanedJetsPt30BTagged_bTagSF, KD, DVBFDEC, DVHDEC, _yield_SR, _current_final_state, _current_category );
			
         blinded_histos->FillDvsM4l_ZX( ZZMass, KD, DVBFDEC, DVHDEC, nCleanedJetsPt30, _yield_SR, _current_final_state, _current_category );
      }
		
          unblinded_histos->FillOthersZX( ZZMass, ZZPt, ZZEta, PFMET, Pt_leading, Pt_trailing, Eta_leading, Eta_trailing, SIP_leading, SIP_trailing, ISO_leading, ISO_trailing, nExtraLep, nCleanedJetsPt30, nCleanedJetsPt30BTagged_bTagSF, KD, DVBFDEC, DVHDEC, _yield_SR, _current_final_state, _current_category );
		
          unblinded_histos->FillDvsM4l_ZX( ZZMass, KD, DVBFDEC, DVHDEC, nCleanedJetsPt30, _yield_SR, _current_final_state, _current_category );
		
   } // End events loop
   
   for (  int i_cat = 0; i_cat < num_of_categories - 1; i_cat++  )
   {
      for ( int i_fs = 0; i_fs < num_of_final_states - 1; i_fs++  )
      {
         _expected_yield_SR[Settings::fs4l][i_cat]       += _expected_yield_SR[i_fs][i_cat];   //calculate expected yield for inclusive 4l final state
         _number_of_events_CR[Settings::fs4l][i_cat]     += _number_of_events_CR[i_fs][i_cat];
         _expected_yield_SR[i_fs][Settings::inclusive]   += _expected_yield_SR[i_fs][i_cat];   //calculate expected yield for inclusive category
         _number_of_events_CR[i_fs][Settings::inclusive] += _number_of_events_CR[i_fs][i_cat];
         
         if ( _merge_2e2mu )
         {
            _expected_yield_SR[Settings::fs2e2mu][i_cat]       += _expected_yield_SR[Settings::fs2mu2e][i_cat];   //merge 2e2mu and 2mu2e final state
            _number_of_events_CR[Settings::fs2e2mu][i_cat]     += _number_of_events_CR[Settings::fs2mu2e][i_cat];
            _expected_yield_SR[Settings::fs2mu2e][i_cat]        = 0.;
            _number_of_events_CR[Settings::fs2mu2e][i_cat]      = 0.;
         }
      }
   }
   for ( int i_fs = 0; i_fs < num_of_final_states - 1; i_fs++  )
   {
      _expected_yield_SR[Settings::fs4l][Settings::inclusive] += _expected_yield_SR[i_fs][Settings::inclusive];
   }
  
   // Print Z + X expected yields for inclusive category
   cout << endl;
   cout << "========================================================================================" << endl;
   cout << "[INFO] Control printout." << endl << "!!! Numbers shoud be identical to yields from SS method !!!" << endl;
   for ( int i_fs = 0; i_fs < num_of_final_states - 1; i_fs++ )
   {
      if ( _merge_2e2mu && i_fs == Settings::fs2mu2e) continue;
      cout << "Category: " << Settings::inclusive << "   Final state: " << i_fs << endl;
      cout << _expected_yield_SR[i_fs][Settings::inclusive] << " +/- " <<
      _expected_yield_SR[i_fs][Settings::inclusive]/sqrt(_number_of_events_CR[i_fs][Settings::inclusive]) << " (stat., evt: " <<
      _number_of_events_CR[i_fs][Settings::inclusive] << ")" << " +/- " << _expected_yield_SR[i_fs][Settings::inclusive]*0.50 << " (syst.)" << endl;
   }
  
   cout << "[INFO] Total = " << _expected_yield_SR[Settings::fs4l][Settings::inclusive] << endl;
   cout << "========================================================================================" << endl;
   cout << endl;
  
   // Smooth histograms
   if ( SMOOTH_ZX_FULL_RUN2_SS )
   {
      cout << "[INFO] Smoothing Z+X histograms..." << endl;
      blinded_histos->SmoothHistograms();
      unblinded_histos->SmoothHistograms();
   }
      
   unblinded_histos->RenormalizeZX(_expected_yield_SR);   
   blinded_histos->RenormalizeZX(_expected_yield_SR);
   
   cout << "[INFO] Z+X histograms filled." << endl;
}
//===============================================================================



//=========================================
void Plotter::GetHistos( TString file_name )
{
   histo_map[file_name]->GetHistos("ROOT_files/" + file_name + ".root");
   
   cout << "[INFO] Got all histograms." << endl;
}
//=========================================



//===========================
void Plotter::FillInclusive()
{
   unblinded_histos->FillInclusive();
   blinded_histos->FillInclusive();
   
   cout << "[INFO] Summing of histograms finished." << endl;
}
//===========================

//===========================
void Plotter::FillInclusiveCombination()
{
   combination_histos->FillInclusiveCombination();
   cout << "[INFO] Summing of histograms finished." << endl;
}
//===========================


//==================
void Plotter::Save()
{
   system("mkdir -p ROOT_files");
   unblinded_histos->SaveHistos("ROOT_files/Unblinded.root");
   blinded_histos->SaveHistos("ROOT_files/Blinded.root");
   
   cout << "[INFO] All histograms are saved in a root file." << endl;
}
//==================



//==================
void Plotter::Delete()
{
   unblinded_histos->DeleteHistos();
   blinded_histos->DeleteHistos();
   
   cout << "[INFO] Memory clean-up done." << endl;
}
//==================



//==================
void Plotter::plot_1D_single( TString file_name, TString variable_name, TString folder, int fs, int cat )
{
   histo_map[file_name]->plot_1D_single( file_name, variable_name, folder, fs, cat );
   
}
//==================

//==================
void Plotter::Plot()
{
   combination_histos->plot_Combination( "Combination" );
	
}
//==================



//=======================================================================================
void Plotter::plot_1D_all_cat( TString file_name, TString variable_name, TString folder )
{
   histo_map[file_name]->plot_1D_all_cat( file_name, variable_name, folder);
}
//=======================================================================================



//==================
void Plotter::plot_1D_all_fs( TString file_name, TString variable_name, TString folder)
{
   histo_map[file_name]->plot_1D_all_fs( file_name, variable_name, folder);
   
}
//==================



//===============================================================================================
void Plotter::plot_2D_single( TString file_name, TString variable_name, TString folder, int cat )
{
   histo_map[file_name]->plot_2D_single( file_name, variable_name, folder, cat );
}
//===============================================================================================



//=====================================================================================================
void Plotter::plot_2D_error_single( TString file_name, TString variable_name, TString folder, int cat )
{
   histo_map[file_name]->plot_2D_error_single( file_name, variable_name, folder, cat ); 
}
//=====================================================================================================



//=============================================================================================
void Plotter::plot_2D_error_all_cat( TString file_name, TString variable_name, TString folder )
{
   histo_map[file_name]->plot_2D_error_all_cat( file_name, variable_name, folder );
}
//=============================================================================================



//==========================================================
int Plotter::find_current_process( TString input_file_name , int genExtInfo, int n_gen_assoc_lep)
{
   
   int current_process = -999;
   
   // Assign dataset to correct process
   if ( input_file_name.Contains("Data") )           current_process = Settings::Data;
   if ( input_file_name.Contains("ggH125") )         current_process = Settings::H125ggH;
   if ( input_file_name.Contains("VBFH125") )        current_process = Settings::H125VBF;
   if ( input_file_name.Contains("WplusH125")  && genExtInfo > 10)   current_process = Settings::H125VH; //prepare for splitting VH and ttH into lep and had
   if ( input_file_name.Contains("WminusH125") && genExtInfo > 10)   current_process = Settings::H125VH;
   if ( input_file_name.Contains("ZH125")      && genExtInfo > 10)   current_process = Settings::H125VH;
   if ( input_file_name.Contains("ttH125")     && n_gen_assoc_lep > 0)   current_process = Settings::H125ttH;
	if ( input_file_name.Contains("WplusH125")  && genExtInfo <= 10)  current_process = Settings::H125VH; //prepare for splitting VH and ttH into lep and had
	if ( input_file_name.Contains("WminusH125") && genExtInfo <= 10)  current_process = Settings::H125VH;
	if ( input_file_name.Contains("ZH125")      && genExtInfo <= 10)  current_process = Settings::H125VH;
   if ( input_file_name.Contains("ttH125")     && n_gen_assoc_lep == 0)  current_process = Settings::H125ttH;
   if ( input_file_name.Contains("bbH125") )         current_process = Settings::H125bbH;
   if ( input_file_name.Contains("tqH125") )         current_process = Settings::H125tqH;
   if ( input_file_name.Contains("ZZTo4l") )         current_process = Settings::qqZZ;
   if ( input_file_name.Contains("ggTo4e") )         current_process = Settings::ggZZ;
   if ( input_file_name.Contains("ggTo4mu") )        current_process = Settings::ggZZ;
   if ( input_file_name.Contains("ggTo4tau") )       current_process = Settings::ggZZ;
   if ( input_file_name.Contains("ggTo2e2mu") )      current_process = Settings::ggZZ;
   if ( input_file_name.Contains("ggTo2e2tau") )     current_process = Settings::ggZZ;
   if ( input_file_name.Contains("ggTo2mu2tau") )    current_process = Settings::ggZZ;
   if ( input_file_name.Contains("DYJetsToLL_M50") ) current_process = Settings::DY;
   if ( input_file_name.Contains("TTJets") )         current_process = Settings::ttbar;
   if ( input_file_name.Contains("TTTo2L2Nu") )      current_process = Settings::ttbar;
   // End assign dataset to correct process
   
   return current_process;
}
//==========================================================



//=================================
float Plotter::calculate_K_factor(TString input_file_name)
{

   float k_factor = 1;
   
   if ( input_file_name.Contains("ZZTo4l"))
   {  
      k_factor = KFactor_EW_qqZZ * KFactor_QCD_qqZZ_M; // As of Moriond2016
   }
   else if ( input_file_name.Contains("ggTo"))
   {
      k_factor = KFactor_QCD_ggZZ_Nominal; // as of Moriond2016
   }
   return k_factor;
}
//=================================


//=============================
int Plotter::CountAssociatedLeptons()
{
	int n_gen_assoc_leptons = 0;
	for ( int i_gen_assoc_lep = 0; i_gen_assoc_lep < 2; i_gen_assoc_lep++ )
		{
			if ( abs(gen_assoc_lep_id_.at(i_gen_assoc_lep)) == 11 || abs(gen_assoc_lep_id_.at(i_gen_assoc_lep)) == 13 )
			{
				n_gen_assoc_leptons++;
			}
		}
	return n_gen_assoc_leptons;
}
//=============================


//===========================
int Plotter::FindFinalState()
{
   int final_state = -999;

   if ( Z1Flav == -121 )
   {
      if ( Z2Flav == -121 )
         final_state = Settings::fs4e;
      else if ( Z2Flav == -169 )
         final_state = Settings::fs2e2mu;
      else
         cerr << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", Z2Flav = " << Z2Flav << endl;
      }
   else if ( Z1Flav == -169 )
   {
      if ( Z2Flav == -121 )
         final_state = Settings::fs2mu2e;
      else if ( Z2Flav == -169 )
         final_state = Settings::fs4mu;
      else
         cerr << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", Z2Flav = " << Z2Flav << endl;
   }
   else
   {
      cerr << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", Z1Flav = " << Z1Flav << endl;
   }
   
   if ( _merge_2e2mu && final_state == Settings::fs2mu2e ) final_state = Settings::fs2e2mu;

   return final_state;
}
//===========================



//=============================
int Plotter::FindFinalStateZX()
{
   int final_state = -999;

   if ( Z1Flav == -121 )
   {
      if ( Z2Flav == +121 )
         final_state = Settings::fs4e;
      else if ( Z2Flav == +169 )
         final_state = Settings::fs2e2mu;
      else
         cerr << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", Z2Flav = " << Z2Flav << endl;
      }
   else if ( Z1Flav == -169 )
   {
      if ( Z2Flav == +121 )
         final_state = Settings::fs2mu2e;
      else if ( Z2Flav == +169 )
         final_state = Settings::fs4mu;
      else
         cerr << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", Z2Flav = " << Z2Flav << endl;
   }
   else
   {
      cerr << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", Z1Flav = " << Z1Flav << endl;
   }
   
   return final_state;
}
//=============================



//=================================
bool Plotter::blind( float ZZMass)
{
   if( ((ZZMass < _blinding_lower[0]) || (ZZMass > _blinding_upper[0])) && ((ZZMass < _blinding_lower[1]) || (ZZMass > _blinding_upper[1])))
   {
      return true;
   }
   else
   {
      return false;
   }
}
//=================================
