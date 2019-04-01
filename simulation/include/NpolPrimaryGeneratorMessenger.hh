#ifndef NpolPrimaryGeneratorMessenger_h 
#define NpolPrimaryGeneratorMessenger_h 1

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

class NpolPrimaryGeneratorAction;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class NpolPrimaryGeneratorMessenger: public G4UImessenger {
public:
  NpolPrimaryGeneratorMessenger(NpolPrimaryGeneratorAction* );
  virtual ~NpolPrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);
  
private:
  NpolPrimaryGeneratorAction* fnpolAction;
  G4UIdirectory*               fgunDir;
  G4UIcmdWithoutParameter*     listCmd;
  G4UIcmdWithAString*          hlp;
  G4UIcmdWithAString*          filter;
  G4UIcmdWithADouble*          maxDCS, *beamPolarization;
  G4UIcmdWithADoubleAndUnit*   energy, *openAngle;
  G4UIcmdWithAnInteger*        channel;

};
#endif
