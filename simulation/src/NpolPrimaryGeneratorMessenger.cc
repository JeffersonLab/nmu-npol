#include "NpolPrimaryGeneratorMessenger.hh"
#include "NpolPrimaryGeneratorAction.hh"

#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"


NpolPrimaryGeneratorMessenger::NpolPrimaryGeneratorMessenger(NpolPrimaryGeneratorAction* gun)
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
  hlp->SetParameterName("Choice", true);
  hlp->SetDefaultValue("off");
  hlp->SetCandidates("h off");
  hlp->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  //choice of DCS filter: filter for DCS computation
  filter = new G4UIcmdWithAString("/npol/gun/filter", this);
  filter->SetGuidance(" Choice of DCS filter");
  filter->SetGuidance(" Choice: none (default), unpolarized, polarized ");
  filter->SetParameterName("filter", true);
  filter->SetDefaultValue("none");
  filter->SetCandidates("none unpolarized polarized");
  filter->AvailableForStates(G4State_PreInit, G4State_Idle);

  //choice of generator: GPS or (e,e'n)
  genMethod = new G4UIcmdWithAString("/npol/gun/generator", this);
  genMethod->SetGuidance(" Choice of Generator");
  genMethod->SetGuidance(" 2 generators: G4 GPS or (e,e'n) cross sections");
  genMethod->SetGuidance(" Choices: gps (default) dcs");
  genMethod->SetGuidance(" gps --G4 built in general particle source");
  genMethod->SetGuidance(" dcs --differential cross section method for (e,e'n)");
  genMethod->SetParameterName("generator", true);
  genMethod->SetDefaultValue("gps");
  genMethod->SetCandidates("gps dcs");
  genMethod->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  //Differential Cross Section Channel
  channel = new G4UIcmdWithAnInteger("/npol/gun/channel", this);
  channel->SetGuidance(" Differential Channel to be used");
  channel->SetGuidance(" Choice: 1, 2, 3 (default), 4 ");
  channel->SetParameterName("channel", true);
  channel->SetDefaultValue((G4int)3);
  channel->AvailableForStates(G4State_PreInit, G4State_Idle);

  //Maximum Differential Cross Section value
  maxDCS = new G4UIcmdWithADouble("/npol/gun/maxDCS", this);
  maxDCS->SetGuidance(" Maximum DCS value");
  maxDCS->SetGuidance(" Choice: 0.5 (default) ");
  maxDCS->SetParameterName("maxDCS", true);
  maxDCS->SetDefaultValue((G4double)0.5);
  maxDCS->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Beam energy of electron beam
  energy = new G4UIcmdWithADoubleAndUnit("/npol/gun/energy", this);
  energy->SetGuidance(" Electron Beam Energy");
  energy->SetGuidance(" Choice: 4.4 GeV (default) ");
  energy->SetParameterName("beamEnergy", true);
  energy->SetDefaultUnit("GeV");
  energy->SetDefaultValue((G4double)4.4);
  energy->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  // Polarization of electron beam
  beamPolarization = new G4UIcmdWithADouble("/npol/gun/beamPolarization", this);
  beamPolarization->SetGuidance(" Electron Beam Polarization");
  beamPolarization->SetGuidance(" Choice: 0.8 (default) ");
  beamPolarization->SetParameterName("polBeam", true);
  beamPolarization->SetDefaultValue((G4double)0.8);
  beamPolarization->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set detector opening angle 
  openAngle = new G4UIcmdWithADoubleAndUnit("/npol/gun/openAngle", this);
  openAngle->SetGuidance(" Set Electron detetor openning angle");
  openAngle->SetGuidance(" Choice: 5 (default) ");
  openAngle->SetParameterName("openAngle", true);
  openAngle->SetDefaultUnit("deg");
  openAngle->SetDefaultValue((G4double)5);
  openAngle->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set helicity ratio of beam (+/-)
  helicityRatio = new G4UIcmdWithADouble("/npol/gun/helicityRatio", this);
  helicityRatio->SetGuidance(" Electron Beam Helicity ratio ");
  helicityRatio->SetGuidance(" Choice: 1 (default) [0 1] ");
  helicityRatio->SetParameterName("helicityRatio", true);
  helicityRatio->SetDefaultValue((G4double)1);
  helicityRatio->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set electric form factor of neutron value (GEn)
  GEn = new G4UIcmdWithADouble("/npol/gun/GEn", this);
  GEn->SetGuidance(" Neutron Electric Form Factor (GEn) ");
  GEn->SetGuidance(" Choice: 0 (default) ");
  GEn->SetParameterName("gen", true);
  GEn->SetDefaultValue((G4double)0);
  GEn->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set magnetic form factor of neutron value (GMn)
  GMn = new G4UIcmdWithADouble("/npol/gun/GMn", this);
  GMn->SetGuidance(" Neutron Magnetic Form Factor (GMn) ");
  GMn->SetGuidance(" Choice: 0 (default) ");
  GMn->SetParameterName("gmn", true);
  GMn->SetDefaultValue((G4double)0);
  GMn->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  // Set Inital values (defaults above)  Turns out it has to be done? Eh?
  fnpolAction->SetFilterValue("none");
  fnpolAction->SetGenMethodValue("gps");
  fnpolAction->SetMaxDCSValue(0.5);
  fnpolAction->SetChannelValue(3);
  fnpolAction->SetBeamEnergyValue(4.4);
  fnpolAction->SetBeamPolarizationValue(0.80);
  fnpolAction->SetOpeningAngleValue(5*deg); // unit needed to get it right later!
  fnpolAction->SetGenValue(0.0);
  fnpolAction->SetGmnValue(0.0);
  fnpolAction->SetHelicityRatioValue(1);
  
}

NpolPrimaryGeneratorMessenger::~NpolPrimaryGeneratorMessenger(){
  
  delete hlp;
  delete filter;
  delete genMethod;
  delete maxDCS;
  delete channel;
  delete energy;
  delete beamPolarization;
  delete openAngle;
  delete helicityRatio;
  delete GEn;
  delete GMn;
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
  if (command == genMethod){ fnpolAction->SetGenMethodValue(newVal); }
  if (command == maxDCS) { fnpolAction->SetMaxDCSValue(maxDCS->GetNewDoubleValue(newVal)); }
  if (command == channel) { fnpolAction->SetChannelValue(channel->GetNewIntValue(newVal)); }
  if (command == energy) { fnpolAction->SetBeamEnergyValue(energy->GetNewDoubleValue(newVal)); }
  if (command == beamPolarization) { fnpolAction->SetBeamPolarizationValue(beamPolarization->GetNewDoubleValue(newVal)); }
  if (command == openAngle) { fnpolAction->SetOpeningAngleValue(openAngle->GetNewDoubleValue(newVal)); }
  if (command == helicityRatio) { fnpolAction->SetHelicityRatioValue(helicityRatio->GetNewDoubleValue(newVal)); }
  if (command == GEn) { fnpolAction->SetGenValue(GEn->GetNewDoubleValue(newVal)); }
  if (command == GMn) { fnpolAction->SetGmnValue(GMn->GetNewDoubleValue(newVal)); }
  
}
