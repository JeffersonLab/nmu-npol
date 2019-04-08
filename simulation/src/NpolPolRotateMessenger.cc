// W. Tireman 8-April-2019
// Created this messenger class to send commands from G4 Macro
// to the polarizaton scattering code in PolNucleonRotate and
// hence the G4HadronElasticProcess and G4HadronicProcess classes.

#include "NpolPolRotateMessenger.hh"
#include "PolNucleonRotate.hh"

#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"


NpolPolRotateMessenger::NpolPolRotateMessenger(PolNucleonRotate* npol)
:fnpolAction(npol){
  fnpolDir = new G4UIdirectory("/npol/");
  fnpolDir->SetGuidance("Npol general control commands.");

  listCmd = new G4UIcmdWithoutParameter("/npol/list",this);
  listCmd->SetGuidance("List of commands.");
   
  //help option
  hlp = new G4UIcmdWithAString("/npol/help", this);
  hlp->SetGuidance(" Lists possible actions to be taken");
  hlp->SetGuidance(" Choice: h ");
  hlp->SetParameterName("Choice", true);
  hlp->SetDefaultValue("off");
  hlp->SetCandidates("h off");
  hlp->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  polScatCmd = new G4UIcmdWithABool("/npol/polarScattering", this);
  polScatCmd->SetGuidance(" Chose polarized (n,p) and (n,C) scattering with this bool" );
  polScatCmd->SetGuidance(" Choice: true or false [default] ");
  polScatCmd->SetParameterName("polarScatCmd", true);
  polScatCmd->SetDefaultValue(false);

  // Set defaults
  fnpolAction->SetPolarScatteringValue(false);
      
}

NpolPolRotateMessenger::~NpolPolRotateMessenger(){
  
  delete hlp;
  delete listCmd;
  delete polScatCmd; 
  delete fnpolDir;
}

void NpolPolRotateMessenger::SetNewValue(G4UIcommand* command, G4String newVal){

  
  //G4cout << " \n  Is this GENERATOR being called at all??????????? \n \n";
  if(command == hlp){
    G4cout << "\nUsage: NpolGenerator [options] inputFile\n\n";
    G4cout << "\toptions:\n";
    G4cout << "\t/npol/help\t\tThis information\n";
    G4cout << "\t/npol/rootFile [filename]\tdirect ROOT output to <filename>\n";
  }
  
  if (polScatCmd){ fnpolAction->SetPolarScatteringValue(polScatCmd->GetNewBoolValue(newVal)); }
  
}
