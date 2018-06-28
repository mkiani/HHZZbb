#----------------------------------------------------------------------
#
# Configuration for data
#
#----------------------------------------------------------------------


# Load deafult job config
import os
PyFilePath = os.environ['CMSSW_BASE'] + "/src/HHZZbb/AnalysisStep/test/"
execfile(PyFilePath + "analyzer.py")        

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#process.appendPhotons.photonSel="skip"

#process.appendPhotons.photonSel = "passThrough"
#process.FSR  = cms.EDAnalyzer("FSRAnalyzer",)
#process.FSRPath = cms.EndPath(process.FSR)
