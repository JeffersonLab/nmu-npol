//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_PrimaryGeneratorAction_h
#define Npol_PrimaryGeneratorAction_h

#include "NpolPrimaryGeneratorMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleGunMessenger.hh"
#include "TLorentzVector.h"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;

class NpolPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  NpolPrimaryGeneratorAction();
  virtual ~NpolPrimaryGeneratorAction();
  G4ParticleGun* GetParticleGun() {return fParticleGun;};
  
  virtual void GeneratePrimaries(G4Event*);
  void GenerateNeutronEvent();
  double genCalc(double q2);
  double gmnCalc(double q2);
  
  void SetFilterValue(G4String val);
  void SetMaxDCSValue(G4double val);
  void SetChannelValue(G4int val);
  void SetBeamEnergyValue(G4double val);
  void SetBeamPolarizationValue(G4double val);
  void SetOpeningAngleValue(G4double val);
  void SetGenValue(G4double val);
  void SetGmnValue(G4double val);
  void SetHelicityRatioValue(G4double val);
  
public:
  
  static G4double NpolAng;
  G4double maxDCS, beamEnergy, polBeam, openAngle, helicityRatio;
  G4double gen, gmn;
  G4String filter;
  G4int channel;
private:
  NpolPrimaryGeneratorMessenger* gunMessenger;
  G4ParticleGun* fParticleGun;
  G4GeneralParticleSource* fParticleGun2;
  
};

#endif

