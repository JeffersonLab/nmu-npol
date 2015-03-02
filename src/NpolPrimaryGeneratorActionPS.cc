// Primary Generator Action file with a point source and 
// limited in theta and phi angles 


#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "Randomize.hh"

#include "NpolPrimaryGeneratorActionPS.hh"

NpolPrimaryGeneratorActionPS::NpolPrimaryGeneratorActionPS()
  : worldBox(NULL)
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  // default particle kinematic
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName="neutron");
 
  particleGun->SetParticleDefinition(particle);


}

NpolPrimaryGeneratorActionPS::~NpolPrimaryGeneratorActionPS()
{
  delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorActionPS::GeneratePrimaries(G4Event* anEvent)
{
  
G4double x0 = 0.0*m;
G4double y0 = 0.0*m;
G4double z0 = 0.0*m;


  // insert code to calculate a momentum vector from a point source and
  // restricted in theta and phi
  // Theta is the azimulthal angle and phi is the rotation angle 

 G4double theta = -(28.0+3.975*(2*G4UniformRand()-1))*deg;
  G4double phi = (3.975*(2*G4UniformRand()-1))*deg;
  
  G4ThreeVector momDirection;
  momDirection.setRThetaPhi(1., theta, phi);
  particleGun->SetParticleMomentumDirection(momDirection);
  particleGun->SetParticleEnergy(2000.*MeV);
  
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
  
}

