#ifndef NpolPolRotateMessenger_h 
#define NpolPolRotateMessenger_h 1

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

class PolNucleonRotate;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class NpolPolRotateMessenger: public G4UImessenger {
public:
  NpolPolRotateMessenger(PolNucleonRotate *npol);
  virtual ~NpolPolRotateMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  PolNucleonRotate*            fnpolAction;
  G4UIcmdWithAString*          hlp;
  G4UIdirectory*               fnpolDir;
  G4UIcmdWithoutParameter*     listCmd;
  G4UIcmdWithABool*            polScatCmd;

};
#endif
