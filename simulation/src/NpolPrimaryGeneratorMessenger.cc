#include "NpolPrimaryGeneratorMessenger.hh"
#include "NpolPrimaryGeneratorAction.hh"

#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"

NpolPrimaryGeneratorMessenger::NpolPrimaryGeneratorMessenger(
                                  NpolPrimaryGeneratorAction* gun)
:fnpolAction(gun){
  fgunDir = new G4UIdirectory("/npol/gun/");
  fgunDir->SetGuidance("Particle Gun control commands.");

  listCmd = new G4UIcmdWithoutParameter("/npol/gun/list",this);
  listCmd->SetGuidance("List available particles.");
  listCmd->SetGuidance(" Invoke G4ParticleTable.");
  
  //help option
  hlp = new G4UIcmdWithAString("/npol/gun/help", this);
  hlp->SetGuidance(" Lists possible actions to be taken");
  hlp->SetGuidance(" Choice: h ");
  hlp->SetParameterName("choice", true);
  hlp->SetDefaultValue("off");
  hlp->SetCandidates("h off");
  hlp->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  //choice of DCS filter: filter for DCS computation
  filter = new G4UIcmdWithAString("/npol/gun/filter", this);
  filter->SetGuidance(" Choice of DCS filter");
  filter->SetGuidance(" Choice: none (default), unpolarized, polarized ");
  filter->SetParameterName("choice", true);
  filter->SetDefaultValue("none");
  filter->SetCandidates("none unpolarized polarized");
  filter->AvailableForStates(G4State_PreInit, G4State_Idle);

  //Differential Cross Section Channel
  channel = new G4UIcmdWithAnInteger("/npol/gun/channel", this);
  channel->SetGuidance(" Differential Channel to be used");
  channel->SetGuidance(" Choice: 1, 2, 3 (default), 4 ");
  channel->SetParameterName("choice", true);
  channel->SetDefaultValue((G4int)3);
  channel->AvailableForStates(G4State_PreInit, G4State_Idle);
  /*
  //Select if pi0 Channel will be used
  piDecay = new G4UIcmdWithABool("/npol/gun/selectPi0",this);
  piDecay->SetGuidance(" Decide if ROOT file will be created");
  piDecay->SetGuidance(" Choice: false (default), true ");
  piDecay->SetParameterName("choice", true);
  piDecay->SetDefaultValue((G4bool)true);
  piDecay->AvailableForStates(G4State_PreInit,G4State_Idle);*/

  //Maximum Differential Cross Section value
  maxDCS = new G4UIcmdWithADouble("/npol/gun/MaxDCS", this);
  maxDCS->SetGuidance(" Maximum DCS value");
  maxDCS->SetGuidance(" Choice: 0.5 (default) ");
  maxDCS->SetParameterName("choice", true);
  maxDCS->SetDefaultValue((G4double)0.5);
  maxDCS->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Beam energy of electron beam
  energy = new G4UIcmdWithADoubleAndUnit("/npol/gun/energy", this);
  energy->SetGuidance(" Electron Beam Energy");
  energy->SetGuidance(" Choice: 4.4 GeV (default) ");
  energy->SetParameterName("choice", true);
  energy->SetDefaultValue((G4double)4.4);
  energy->SetDefaultUnit("GeV");
  energy->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  // Polarization of electron beam
  beamPolarization = new G4UIcmdWithADouble("/npol/gun/beamPolarization", this);
  beamPolarization->SetGuidance(" Electron Beam Polarization");
  beamPolarization->SetGuidance(" Choice: 0.8 (default) ");
  beamPolarization->SetParameterName("choice", true);
  beamPolarization->SetDefaultValue((G4double)0.8);
  beamPolarization->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set detector opening angle 
  openAngle = new G4UIcmdWithADoubleAndUnit("/npol/gun/openAngle", this);
  openAngle->SetGuidance(" Set Electron detetor openning angle");
  openAngle->SetGuidance(" Choice: 5 (default) ");
  openAngle->SetParameterName("choice", true);
  openAngle->SetDefaultUnit("deg");
  openAngle->SetDefaultValue((G4double)5);
  openAngle->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  // Set Inital values (defaults above)  Turns out it has to be done? Eh?
  fnpolAction->SetFilterValue("none");
  fnpolAction->SetMaxDCSValue(0.5);
  fnpolAction->SetChannelValue(3);
  fnpolAction->SetBeamEnergyValue(4.4);
  fnpolAction->SetBeamPolarizationValue(0.80);
  fnpolAction->SetOpeningAngleValue(5);
}

NpolPrimaryGeneratorMessenger::~NpolPrimaryGeneratorMessenger(){
  
  delete hlp;
  delete filter;
  delete maxDCS;
  delete channel;
  delete energy;
  delete beamPolarization;
  delete openAngle;
  delete fgunDir;
}

void NpolPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newVal){

  
  //G4cout << " \n  Is this GENERATOR being called at all??????????? \n \n";
  if(command == hlp){
    //florAction->SetUseHelp(newVal);
    G4cout << "\nUsage: NpolGenerator [options] inputFile\n\n";
    G4cout << "\toptions:\n";
    G4cout << "\t/npol/gun/help\t\tThis information\n";
    G4cout << "\t/npol/gun/rootFile [filename]\tdirect ROOT output to <filename>\n";
    G4cout << "\t/npol/gun/filter [argument]\tOption for DCS filter, arguments:\n\t\t\tunpolarized --unpolarized DCS filter; \n\t\t\tpolarized --polarized DCS filter; default\n\t\t\tnone -- no filter\n";
    G4cout << "\t/npol/gun/massOfAprime [mean-of-resonance]\t\tMaxDCS --Maximum Value for Differental Cross Section; Default: 0.5\n";
  } 
  if (command == filter){ fnpolAction->SetFilterValue(newVal); }
  if (command == maxDCS) { fnpolAction->SetMaxDCSValue(maxDCS->GetNewDoubleValue(newVal)); }
  if (command == channel) { fnpolAction->SetChannelValue(channel->GetNewIntValue(newVal)); }
  if (command == energy) { fnpolAction->SetBeamEnergyValue(energy->GetNewDoubleValue(newVal)); }
  if (command == beamPolarization) { fnpolAction->SetBeamPolarizationValue(beamPolarization->GetNewDoubleValue(newVal)); }
  if (command == openAngle) { fnpolAction->SetOpeningAngleValue(openAngle->GetNewDoubleValue(newVal)); }
  
}
