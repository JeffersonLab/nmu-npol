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
#include <TLorentzVector.h>
#include "TMatrix.h"

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
  double genCalc(G4double q2);
  double gmnCalc(G4double q2);
  
  void SetFilterValue(G4String val);
  void SetGenMethodValue(G4String val);
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

private:
  G4double maxDCS, beamEnergy, polBeam, openAngle, helicityRatio;
  G4double gen, gmn;
  G4String filter, genMethod;
  G4int channel, helicity=1; // helicity
  G4double q2=0, energySElectron=0, thetaSElectron=0, pRNeutron=0;
  G4double x=0, y=0, z=0; // vertex
  G4double thetaRNeutron=0, tau=0, epsilon=0,polTrans=0, polLongi=0, phaseShift=0;
  G4double fcos=0, fphi=0,fpx=0, fpy=0, fpz=0, fEn=0; //variables for Fermi Momentum
  G4double unpolDCS=0, polDCS=0; // unploarized and polarized differential cross section

  TLorentzVector beam, target, spectator, w, p1, p2, p3; 
  TLorentzVector *pBeam = new TLorentzVector(), *pTarget = new TLorentzVector();
  TLorentzVector *pSpectator = new TLorentzVector(), *pW = new TLorentzVector();
  TLorentzVector *pP1 = new TLorentzVector(), *pP2 = new TLorentzVector();
  TLorentzVector *pP3 = new TLorentzVector(); // the third produced particle is proton for ed->enp
  
  struct EventInfo { TLorentzVector *electronVector; TLorentzVector *neutronVector;
	TLorentzVector *thirdParticleVector; G4double polLong; G4double polTran; } primeEvent;
  
private:
  NpolPrimaryGeneratorMessenger* gunMessenger;
  G4ParticleGun* fParticleGun;
  G4GeneralParticleSource* fParticleGun2;
  
};

#endif

