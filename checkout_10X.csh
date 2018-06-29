#!/bin/tcsh -fe
#
# Instructions:
# wget -O ${TMPDIR}/checkout_10X.csh https://raw.githubusercontent.com/mkiani/HHZZbb/master/checkout_10X.csh
# cd $CMSSW_BASE/src
# cmsenv
# chmod u+x ${TMPDIR}/checkout_10X.csh
# ${TMPDIR}/checkout_10X.csh

############## For CMSSW_10_1_6
git cms-init

#Preliminary electron scale and smearing corrections according to https://twiki.cern.ch/twiki/bin/view/CMS/EGMSmearer
# git cms-merge-topic cms-egamma:EGM_94X_v1
# (cd EgammaAnalysis/ElectronTools/data ; git clone https://github.com/ECALELFS/ScalesSmearings.git ; cd ScalesSmearings ; git checkout Run2017_17Nov2017_v1)

#Electron MVA ID in 94X according to https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentificationRun2#Recipes_for_regular_users
# MVA ID V2 now, not yet available as part of official recepie
# git cms-merge-topic guitargeek:ElectronID_MVA2017_V2_HZZ_940pre3
# rm -rf RecoEgamma/ElectronIdentification/data #Delete old BDT weights so we can clone new ones
# git clone -b ElectronID_MVA2017_V2 https://github.com/guitargeek/RecoEgamma-ElectronIdentification RecoEgamma/ElectronIdentification/data/
#Hack to make our run_cfg.py job script work with new implementation of ID
# sed -i "s@-float('Inf')@-999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_iso_V1_cff.py
# sed -i "s@float('Inf')@999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_iso_V1_cff.py
# sed -i "s@-float('Inf')@-999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_iso_V2_cff.py
# sed -i "s@float('Inf')@999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_iso_V2_cff.py
# sed -i "s@-float('Inf')@-999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_noIso_V1_cff.py
# sed -i "s@float('Inf')@999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_noIso_V1_cff.py
# sed -i "s@-float('Inf')@-999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_noIso_V2_cff.py
# sed -i "s@float('Inf')@999999.@g" RecoEgamma/ElectronIdentification/python/Identification/mvaElectronID_Fall17_noIso_V2_cff.py


#MET corrections according to https://twiki.cern.ch/twiki/bin/view/CMS/MissingETUncertaintyPrescription#Instructions_for_9_4_X_X_0_for_M
# git cms-merge-topic cms-met:METRecipe94x


#MELA Analytics
git clone https://github.com/usarica/MelaAnalytics.git

#MELA
git clone https://github.com/cms-analysis/HiggsAnalysis-ZZMatrixElement.git ZZMatrixElement
(cd ZZMatrixElement; git checkout -b from-v216 v2.1.6)
# replace ZZMatrixElement/MELA/setup.sh -j 8
(                                                                 \
  cd ${CMSSW_BASE}/src/ZZMatrixElement/MELA/COLLIER/             ;\
  set pkgname="collier-1.2"                                      ;\
  set pkgdir="COLLIER-1.2"                                       ;\
  set tarname=$pkgname".tar.gz"                                  ;\
  set tarweb="https://www.hepforge.org/archive/collier/"$tarname ;\
  set libname="libcollier.so"                                    ;\
  set tmpdir="colliertmp"                                        ;\
  wget $tarweb                                                   ;\
  mkdir $tmpdir                                                  ;\
  tar -xvzf $tarname -C $tmpdir                                  ;\
  rm $tarname                                                    ;\
  mv $tmpdir"/"$pkgdir"/src/"* ./                                ;\
  rm -rf $tmpdir                                                 ;\
  make                                                           ;\
  mv $libname "../data/"$SCRAM_ARCH"/"$libname                   ;\
)
(                                                                 \
  cd ${CMSSW_BASE}/src/ZZMatrixElement/MELA/fortran/             ;\
  make all                                                       ;\
  mv libjhugenmela.so ../data/${SCRAM_ARCH}/                     ;\
)

#kinematic refitting
git clone https://github.com/mhl0116/KinZfitter-1.git KinZfitter
(cd KinZfitter ; git checkout -b from-27daebb 27daebb)

#### Please do not add any custom (non-CMSSW) package before this line ####

#ZZAnalysis
git clone git@github.com:mkiani/HHZZbb.git HHZZbb
(cd HHZZbb; git checkout master)

# Higgs Combination Package, Needed for the Double Crystall Ball function. 
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit

#MuScleFit: probably tbf
#git clone https://github.com/scasasso/usercode MuScleFit


#muon momentum scale corrections (76X)
git clone https://github.com/bachtis/Analysis.git -b KaMuCa_V4 KaMuCa


