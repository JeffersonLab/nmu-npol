
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TVector.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

#include "../include/NpolVertex.hh"
#include "../include/NpolTagger.hh"
#include "../include/NpolStatistics.hh"
#include "../include/NpolStep.hh"

struct polarimeter_edep_info {
  bool analyzerSectionsHit[4];
  bool topESectionsHit[4];
  bool topdESectionsHit[4];
  bool bottomESectionsHit[4];
  bool bottomdESectionsHit[4];

  double analyzerSectionsEdep[4];
  double topESectionsEdep[4];
  double topdESectionsEdep[4];
  double bottomESectionsEdep[4];
  double bottomdESectionsEdep[4];
};

// Example assembly volume name: av_1_impr_1_TopDetLV_0
int GetAVNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",3);
    return atoi(volName.substr(3,underscorePos-3).c_str());
  } else{
    return 0;
  }
}

int GetImprNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",1+
					      volName.find_first_of("_",3));
    return atoi(volName.substr(underscorePos+1,1).c_str());
  } else
    return 0;
}

int GetPlacementNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",1+volName.find_first_of
					      ("_",1+volName.find_first_of
					       ("_",1+volName.find_first_of
						("_",1+volName.find_first_of("_",3)))));
    return atoi(volName.substr(underscorePos+1,std::string::npos).c_str());
  } else
    return 0;
}

struct polarimeter_edep_info *GetEdepInfo(const std::map<std::string,double> &eDep) {
  struct polarimeter_edep_info *info =
    (struct polarimeter_edep_info *)calloc(1,sizeof(struct polarimeter_edep_info));

  std::map<std::string,double>::const_iterator it;
  for(it = eDep.begin(); it != eDep.end(); it++) {
    int AVNumber = GetAVNumber(it->first);
    if(AVNumber == 0 || AVNumber > 10) continue;
    int PVNumber = GetPlacementNumber(it->first);
    
    int section = -1;
    switch(AVNumber) {
    case 1:{ // Top E array 1
      //int section = (PVNumber >= 5 ? 0 : 1);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 13){
	int section = 1;
      }
      (info->topESectionsEdep)[section] += it->second;
      if(it->second >= 1 && section != -1) (info->topESectionsHit)[section] = true;
      break;}
    case 2:{ // Top E array 2 
      //int section = (PVNumber >= 5 ? 2 : 3);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 14){
	int section = 1;
      }
      (info->topESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->topESectionsHit)[section] = true;
      break;}
    case 3:{  // Top dE array 1
      //int section = (PVNumber >= 5 ? 0 : 1);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 13){
	int section = 1;
      }
      (info->topdESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->topdESectionsHit)[section] = true;
      break;}
    case 4:{  // Top dE array 2
      //int section = (PVNumber >= 5 ? 2 : 3);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 14){
	int section = 1;
      }
      (info->topdESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->topdESectionsHit)[section] = true;
      break;}
    case 5:{ // BottomE array 1
      //int section = (PVNumber >= 5 ? 0 : 1);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 13){
	int section = 1;
      }
      (info->bottomESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->bottomESectionsHit)[section] = true;
      break;}
    case 6:{ // BottomE array 2
      //int section = (PVNumber >= 5 ? 2 : 3);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 14){
	int section = 1;
      }
      (info->bottomESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->bottomESectionsHit)[section] = true;
      break;}
    case 7:{ // Bottom dE array 1
      //int section = (PVNumber >= 5 ? 0 : 1);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 13){
	int section = 1;
      }
      (info->bottomdESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->bottomdESectionsHit)[section] = true;
      break;}
    case 8:{ // Bottom dE array 2
      //int section = (PVNumber >= 5 ? 2 : 3);
      if(PVNumber >= 0 || PVNumber <= 7){
	int section = 0;
      }else if(PVNumber >= 8 || PVNumber <= 14){
	int section = 1;
      }
      (info->bottomdESectionsEdep)[section] += it->second;
      if(it->second >= 1) (info->bottomdESectionsHit)[section] = true;
      break;}
    case 9:{  // Front arrays 1 and 2
      int ImprNumber = GetImprNumber(it->first);
      (info->analyzerSectionsEdep)[ImprNumber-1] += it->second;
      if(it->second >= 1) (info->analyzerSectionsHit)[ImprNumber-1] = true;
      break;}
    case 10:{ // Front arrays 3 and 4
      int ImprNumber = GetImprNumber(it->first);
      (info->analyzerSectionsEdep)[ImprNumber+1] += it->second;
      if(it->second >= 1) (info->analyzerSectionsHit)[ImprNumber+1] = true;
      break;}
    default:
      break;
    }
  }
  return info;
}

// Requirement 1: At least one hit (energy deposition >= 1 MeV) in some detector in the
// analyzer and an energy deposition of >= 1 MeV in some detectors in the dE and E arrays
// (both top or both bottom) in the same section.

// Requirement 2: No hits in the analyzers in the previous (upstream) sections to avoid
// counting of secondary scatterings in the polarimeter.

// Requirement 3: Total energy in the analyzer detectors in the section of interest should
// be more than 4 MeV; total energy deposition in the E-array of interest should be more
// than 5 MeV.

// Requirement 4: Total energy deposition in the analyzer+dE+E detectors in the section of
// interest should be >= 50 MeV (viz. a little bit more than the minimum ionizing particle
// (MIP) crossing two 10 cm thick detectors.

// Requirement 5: Top/Bottom asymmetry of energy depositions in the section of interest
// (for example, if we have "good" energy depositions in the bottom dE and E arrays, the
// total energy deposition in the top arrays should be at least 20 times less than in the
// bottom ones. And vice versa...) This criterion helps suppress inelastic interactions
// and select recoil protons from elastic and quasielastic scatterings of neutrons in the
// polarimeter analyzer.

// Requirement 6 (Not yet implemented): Because the analyzing power is concentrated in the
// range of neutron scattering angles of 4-25 degrees, we applied correspondent cuts on the
// "recoil proton" scattering angle (viz. 45.3 degrees at Q^2 = 3.95 (GeV/c)^2, 40.8-80.2
// degrees at 5.22 (GeV/c)^2, and 36.0-78.3 degrees at 6.88 (GeV/c)^2).

bool CheckRequirements(const struct polarimeter_edep_info *info) {
  // Find out which polarimeter section was hit, start from the back
  int sectionOfInterest = -1;
  for(int i = 3; i >= 0 ; i--) {
    if((info->analyzerSectionsHit)[i]) {
      // check requirement 2
      sectionOfInterest = i;
    } 

    // Check requirement 1a
    if(sectionOfInterest == -1){
      return false;
    } else {
      return true;
    }
  }

  // Check requirement 5
  bool topWasHit;
  if(((info->topESectionsEdep)[sectionOfInterest]+
      (info->topdESectionsEdep)[sectionOfInterest]) / 
     ((info->bottomESectionsEdep)[sectionOfInterest]+
      (info->bottomdESectionsEdep)[sectionOfInterest]) >= 20) 
    topWasHit = true;
  else if(((info->bottomESectionsEdep)[sectionOfInterest]+
	   (info->bottomdESectionsEdep)[sectionOfInterest]) / 
	  ((info->topESectionsEdep)[sectionOfInterest]+
	   (info->topdESectionsEdep)[sectionOfInterest]) >= 20) 
    topWasHit = false;
  else
    return false;
  
  // Check requirement 4a
  if((info->analyzerSectionsEdep)[sectionOfInterest] <= 4)
    return false;
  if(topWasHit) {
    
    // Check requirement 1b
    if(!((info->topESectionsHit)[sectionOfInterest] && 
	 (info->topdESectionsHit)[sectionOfInterest]))
      return false;
    
    // Check requirement 4b
    if((info->topESectionsEdep)[sectionOfInterest] <= 5)
      return false;
    
    // Check requirement 3
    double totalEdep = (info->analyzerSectionsEdep)[sectionOfInterest] 
      + (info->topESectionsEdep)[sectionOfInterest] +
      (info->topdESectionsEdep)[sectionOfInterest];
    if(totalEdep <= 50)
      return false;
  } else {
    if(!((info->bottomESectionsHit)[sectionOfInterest] && 
	 (info->bottomdESectionsHit)[sectionOfInterest]))
      return false;
    if((info->bottomESectionsEdep)[sectionOfInterest] <= 5)
      return false;
    double totalEdep = (info->analyzerSectionsEdep)[sectionOfInterest] + 
      (info->bottomESectionsEdep)[sectionOfInterest] +
      (info->bottomdESectionsEdep)[sectionOfInterest];
    if(totalEdep <= 50)
      return false;
  }
  return true;
}

bool EventRequirementsPassed(const std::map<std::string,double> &eDep) {
  struct polarimeter_edep_info *info = GetEdepInfo(eDep);
  bool ret = CheckRequirements(info);
  free(info);
  return ret;
}

void dEoverE2() {

  gSystem->Load("NpolClass.so");
  //  std::vector<NpolVertex *> *anEntry = NULL;
  std::map<std::string, double> eDep;
  std::vector<NpolStep *> *aStepEntry = NULL;
  std::vector<NpolStatistics *> *aStatsEntry = NULL;

  // The TChain is very nice.
  TChain *npolTree = new TChain("T");
  npolTree->Add("/data3/cgen/FirstRun/NeutronOnly/npolNeutron_2-1GeV_1_*.root");
  TChain *statsTree = new TChain("T2");
  statsTree->Add("/data3/cgen/FirstRun/NeutronOnly/npolNeutron_2-1GeV_1_*.root");
   //npolTree->Add("/data2/tireman/LongRun/npolRun_1_0001.root");

  //npolTree->SetBranchAddress("tracks",&anEntry);
  npolTree->SetBranchAddress("steps",&aStepEntry);
  statsTree->SetBranchAddress("stats",&aStatsEntry);
  
  // loop over all stats branches (one per file)
  Long_t totalNeutrons = 0;
  Long_t totalNeturonsRecorded = 0;
  for(int i = 0; i < statsTree->GetEntries(); i++) {
    statsTree->GetEntry(i);
    totalNeutrons += ((*aStatsEntry)[0])->totalEvents;
    totalNeturonsRecorded += ((*aStatsEntry)[0])->eventsSaved;
  }
  std::cout << "Total neutrons on polarimeter: " << totalNeutrons << std::endl
	    << "Total neutrons striking polarimeter: " << totalNeturonsRecorded << std::endl;
   
  TFile *outFile = new TFile("dEoverE_Daniel.root","RECREATE");
  TH2F *h_dEoverEtop = new TH2F("dEoverEtop","dE over E",200,0,150,400,0,20);
  TH2F *h_dEoverEbot = new TH2F("dEoverEbot","dE over E",200,0,150,400,0,20);
  TH2F *h_dEoverEplus = new TH2F("dEoverEplus","dE over E",200,0,150,400,0,20);
  int eventsPassed = 0;
  int eventsFailed = 0;

  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  //for(int i = 0; i < 100000; i++) {
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
    if(i % 1000 == 0)
      std::cout << "Processing event #" << i << std::endl;

    // loop over vector elements (one per vertex)
    if(aStepEntry->empty()) continue;
    Int_t nvertices = aStepEntry->size();
    for (Int_t j = 0; j < nvertices; j++){
      NpolStep *aStep = (*aStepEntry)[j];
      if(aStep == NULL) continue;
 
      if(eDep.find(aStep->volume) == eDep.end())
	eDep[aStep->volume] = 0;
      eDep[aStep->volume] += aStep->eDep;
    }

    if(!EventRequirementsPassed(eDep)) {
      eventsFailed++;
      continue;
    } else
      eventsPassed++;
    
    double Etop = 0.0;
    double Ebot = 0.0;
    double dEtop = 0.0;
    double dEbot = 0.0;
    std::map<std::string,double>::iterator it2;
    for(it2 = eDep.begin(); it2 != eDep.end(); it2++) {
      int avNum = GetAVNumber(it2->first);
      if(avNum == 0)
	continue;
      switch(avNum) {
      case 1: case 2:
	Etop += it2->second;
	break;
      case 3: case 4:
	dEtop += it2->second;
	break;
      case 5: case 6:
	Ebot += it2->second;
	break;
      case 7: case 8:
	dEbot += it2->second;
	break;
      default:
	break;
      }
    }

    if(Etop > 5 && dEtop > 1) h_dEoverEtop->Fill(Etop,dEtop);
    if(Ebot > 5 && dEbot > 1) h_dEoverEbot->Fill(Ebot,dEbot);
    eDep.clear();
  }
  
  std::cout << eventsPassed << " events passed requirements. "
	    << eventsFailed << " failed." << std::endl;
  
  h_dEoverEplus->Add(h_dEoverEtop,h_dEoverEbot);
  h_dEoverEtop->Write();
  h_dEoverEbot->Write();
  h_dEoverEplus->Write();
  outFile->Close();
}
