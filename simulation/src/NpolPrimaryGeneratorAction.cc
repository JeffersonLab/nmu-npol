//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                      *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolPrimaryGeneratorAction.cc %%

// Generates the primary particle for each event
// Created: William Tireman
// Modified: 26-June-2016  Changed to General Particle Source -- W.T.
// Modified: 23-March-2019 Just updated the code to include a (e,e'n)
//         generator created by Tongtong Cao (Hampton U.). This method
//         needs more work so it can be more versitile but its on the
//         right track.
// Modified: 3-April-2019  Added NPOL messenger class and debugged -- W.T.

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "TMath.h" 
#include "TRandom3.h"

#include "JGenPhaseSpace.h"
#include "JGenFermiMomentum.h"

#include "NpolPrimaryGeneratorAction.hh"
#include "NpolPolarimeter.hh"

/* ----------- constants ----------- */ 
#define massElectron 0.000510998956 // GeV
#define massNeutron 0.939565378 // GeV
#define massDeuteron 1.875612928 // GeV
#define massProton 0.93827231 // GeV
#define alpha 0.0072973525664 // fine structure constant
#define dcsConversion 2.56819e-6 // const for conversion from 1ub to 1GeV^-2
/* ----------- constants ----------- */

G4double NpolPrimaryGeneratorAction::NpolAng = (NpolPolarimeter::NpolAng)*180./TMath::Pi();

NpolPrimaryGeneratorAction::NpolPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0),fParticleGun2(0)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  gunMessenger = new NpolPrimaryGeneratorMessenger(this);
 
  // Must create both particle gun options since MACRO doesn't run until after
  // they are created.  However, at event generation we can call the one needed.

  // Gun 1 is the standard ParticleGun class and uses Tongtong's generator
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleDefinition* pType = particleTable->FindParticle("geantino");
  fParticleGun->SetParticleDefinition(pType);
  
  // Gun 2 is the GeneralParticleSource class from G4; can use ion sources
  // beams, biasing files, etc. 
  fParticleGun2 = new G4GeneralParticleSource();
 
}

NpolPrimaryGeneratorAction::~NpolPrimaryGeneratorAction()
{
  std::cout << "Deleting Particle Gun" << std::endl;
  delete fParticleGun;
  delete fParticleGun2;
  delete gunMessenger;
} 

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  // in the Macro file, /npol/gun/generator sets if the run uses the
  // diff. cross section method from Tongtong or the G4 GPS source.
  if(genMethod == "dcs"){
	G4double NpolAng = -NpolPolarimeter::NpolAng;
	GenerateNeutronEvent();
	G4double nMom = primeEvent.neutronVector->P();
	G4double nTheta = primeEvent.neutronVector->Theta();
	G4double nPhi = primeEvent.neutronVector->Phi();

	// Need to optimize the Polarization setting to make sense in real experiement setting.
	// For now, we just rotate the Transverse and longitudinal components by 90 degrees.
	G4double polX = primeEvent.polLong;//primeEvent.polTran; // normally this
	G4double polY = 0.;
	G4double polZ = -primeEvent.polTran;//primeEvent.polLong; // normally this

	G4double xDir = sin(nTheta)*cos(nPhi);
	G4double yDir = sin(nTheta)*sin(nPhi);
	G4double zDir = cos(nTheta);
	
	G4double xPrimeDir = xDir*cos(NpolAng) + zDir*sin(NpolAng);
	G4double yPrimeDir = 1*yDir;
	G4double zPrimeDir = -xDir*sin(NpolAng) + zDir*cos(NpolAng);
	G4ThreeVector momPrime;
	momPrime.setX(xPrimeDir); momPrime.setY(yPrimeDir); momPrime.setZ(zPrimeDir);
	
	fParticleGun->SetParticleMomentum(nMom);
	fParticleGun->SetParticleMomentumDirection(momPrime);
	fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
	fParticleGun->SetParticlePolarization(G4ThreeVector(polX,polY,polZ));
	fParticleGun->GeneratePrimaryVertex(anEvent); 
  } else if (genMethod == "gps"){
	fParticleGun2->GeneratePrimaryVertex(anEvent);
  }
  
}

//  Generator from Tongtong Cao (post-doc, Hampton U.) for computation of a Neutron Lorentz 4-vector
//  using the differential cross sections for polarized and unpolarized (e,e'n) reaction.  See
//  document for more details in nmu-npol/simulation/npol-doc folder. 
void NpolPrimaryGeneratorAction::GenerateNeutronEvent(){

  bool evtFlag = false;
  double thetaNeutronFree=NpolAng;// polar angle of neutron polarimeter (deg)
  double thetaNeutronFreeRad=thetaNeutronFree/180.*TMath::Pi();
  double pSNeutronFree=2*massNeutron*beamEnergy*(massNeutron+beamEnergy)*cos(thetaNeutronFreeRad)/((2*beamEnergy*massNeutron+massNeutron*massNeutron+beamEnergy*beamEnergy*sin(thetaNeutronFreeRad)*sin(thetaNeutronFreeRad)));
  double pSElectronFree=sqrt(pow(pSNeutronFree*sin(thetaNeutronFreeRad),2)+pow(beamEnergy-pSNeutronFree*cos(thetaNeutronFreeRad),2));
  double q2Free=2*(beamEnergy-pSElectronFree)*massNeutron;
  double thetaSElectronFree=asin(sqrt((beamEnergy-pSElectronFree)*massNeutron/2/beamEnergy/pSElectronFree))*2/TMath::Pi()*180;

  //// Variables defination ////
  int event = 0; //Serial number of events
  double masses[2]={massElectron,massNeutron}; // masses of product paritcles for en
  double massesENP[3]={massElectron,massNeutron, massProton}; // masses of product paritcles for ed
   
  // Commented out Lorentz Vectors not being used at the moment to suppress warnings.
  pBeam = &beam;
  pTarget = &target;
  pSpectator = &spectator;
  pW = &w;  
  pP1 = &p1;
  pP2 = &p2;
  pP3 = &p3;			

   ////Some preparation////
  TRandom3 randomNum(0);  // randomNum is used in the main function
  TRandom3 *myRNG=new TRandom3(0);
  gRandom = myRNG;  //gRandom is used in the JGenPhaseSpace class

  double ran1=0, ran2=0, ran3=0; //random number: ran1 and ran2 are for extraction of Fermi momentum, ran3 is for fitlering events by DCS

  JGenPhaseSpace eventEN, eventED; 
  TLorentzVector deuteron;
  deuteron.SetXYZT(0,0,0,massDeuteron);
  double thetaSElectronRad=0; // polar angle of scattered electron (rad)
  double mottDCS=0; // Mott differential cross section
  double percentPosHeli=helicityRatio/(1+helicityRatio); // perecent of events with positive helicity
  ////Some preparation////


  if(gen==0) gen = genCalc(q2Free);  // If gen is not assigned, gen is calculated by the formula.
  if(gmn==0) gmn = gmnCalc(q2Free);  // If gem is not assigned, gen is calculated by the formula.

  /* channel 1 */
  if(channel == 1){
	do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
      }
	  
      // Set Lorentz vectors for beam and target
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target.SetXYZT(0, 0, 0, massNeutron);
      w = beam + target;
	  
      if (eventEN.SetDecay (w, 2, masses)){
		eventEN.Generate ();
		pP1 = eventEN.GetDecay (0);
		pP2 = eventEN.GetDecay (1);
	    
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=="none"){
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			evtFlag = true;
			event++;
		  }	event++;
		} 
		
		// Filter events by polarized DCS
		if(filter=="unpolarized"){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			if(unpolDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
		
		if(filter=="polarized"){ 
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			if(gRandom->Rndm()<percentPosHeli) helicity=1;
			else helicity=-1;
			polTrans=polBeam*(-2*gen*gmn*sqrt(tau*(1+tau))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			polLongi=polBeam*(2*pow(gmn,2)*tau*sqrt((1+tau)*(1+tau*pow(sin(thetaSElectronRad/2),2)))*(1/cos(thetaSElectronRad/2))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			phaseShift=atan(polTrans/polLongi)/TMath::Pi()*180;
			polDCS=unpolDCS*(polTrans+polLongi);
			if(polDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
      }
    } while(!(evtFlag)); 
  } 
  /* End channel 1 */

  /* channel 2 */
  else if(channel == 2){
	do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
      }
	  
      // Use reject-accept method to select a value of Fermi Momentum
      ran1 = 0.5*randomNum.Rndm();
      ran2 = 11.0*randomNum.Rndm();
      if (JGenFermiMomentum::Instance().Spectral(ran1)<ran2) continue;
      fcos = (2*randomNum.Rndm())-1;
      fphi = 2*3.141592653*randomNum.Rndm();
      fpx = ran1*sqrt(1-fcos*fcos)*cos(fphi);
      fpy = ran1*sqrt(1-fcos*fcos)*sin(fphi);
      fpz = ran1*fcos;
      fEn = sqrt(fpx*fpx+fpy*fpy+fpz*fpz+massNeutron*massNeutron);
	  
      // Set Lorentz vectors for beam and target
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target.SetXYZT(fpx, fpy, fpz, fEn);
      spectator=deuteron-target;
      w = beam + target;
	  
      if (eventEN.SetDecay (w, 2, masses)){
		eventEN.Generate ();
		pP1 = eventEN.GetDecay (0);
		pP2 = eventEN.GetDecay (1);
	    
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=="none"){
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			evtFlag = true;
			event++;
		  }	event++;
		} 
		
		// Filter events by polarized DCS
		if(filter=="unpolarized"){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			
			if(unpolDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
		
		if(filter=="polarized"){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			mottDCS=pow(alpha,2)/(4*pow(beamEnergy,2)*pow(sin(thetaSElectronRad/2),4))*pow(cos(thetaSElectronRad/2),2);
			unpolDCS=mottDCS*energySElectron/beamEnergy/((1+tau)*epsilon)*(tau*pow(gmn,2)+epsilon*pow(gen,2))/dcsConversion;
			if(gRandom->Rndm()<percentPosHeli) helicity=1;
			else helicity=-1;
			polTrans=polBeam*(-2*gen*gmn*sqrt(tau*(1+tau))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			polLongi=polBeam*(2*pow(gmn,2)*tau*sqrt((1+tau)*(1+tau*pow(sin(thetaSElectronRad/2),2)))*(1/cos(thetaSElectronRad/2))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			phaseShift=atan(polTrans/polLongi)/TMath::Pi()*180;
			polDCS=unpolDCS*(polTrans+polLongi);
			if(polDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
      }
    } while(!(evtFlag)); 
  }
  /* End channel 2 */
  
  /* channel 3 */
  else if(channel == 3){
    do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
	  }
	  
      // Use reject-accept method to select a value of Fermi Momentum
      ran1 = 0.5*randomNum.Rndm();
      ran2 = 11.0*randomNum.Rndm();
      if (JGenFermiMomentum::Instance().Spectral(ran1)<ran2) continue;
      fcos = (2*randomNum.Rndm())-1;
      fphi = 2*3.141592653*randomNum.Rndm();
      fpx = ran1*sqrt(1-fcos*fcos)*cos(fphi);
      fpy = ran1*sqrt(1-fcos*fcos)*sin(fphi);
      fpz = ran1*fcos;
      fEn = sqrt(fpx*fpx+fpy*fpy+fpz*fpz+massNeutron*massNeutron);
	  
	  
      // Set Lorentz vectors for beam and target
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target.SetXYZT(fpx, fpy, fpz, fEn);
	  
      spectator=deuteron-target;
      w = beam + target;
	  
      if (eventEN.SetDecay (w, 2, masses)){
		eventEN.Generate ();
		pP1 = eventEN.GetDecay (0);
		pP2 = eventEN.GetDecay (1);
	    
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=="none"){
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){	
			evtFlag = true;
			event++;
		  }	event++;
		} 
		
		// Filter events by polarized DCS
		if(filter=="unpolarized"){
		  ran3 = maxDCS*randomNum.Rndm(); 
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			double k1=q2*pow(gmn,2);
			double k2=pow(2*massNeutron,2)*(pow(gen,2)+tau*pow(gmn,2))/(1+tau);
			double term1=beam.E()*pP1->E()-beam.Px()*pP1->Px()-beam.Py()*pP1->Py()-beam.Pz()*pP1->Pz()-2*pow(massElectron,2);
			double term2=(beam.E()*target.E()-beam.Px()*target.Px()-beam.Py()*target.Py()-beam.Pz()*target.Pz())*(pP1->E()*target.E()-pP1->Px()*target.Px()-pP1->Py()*target.Py()-pP1->Pz()*target.Pz())/pow(massNeutron,2)-q2/4;
			double M2=pow(2*alpha,2)/pow(q2,2)*(k1*term1+k2*term2);
			unpolDCS=1./4*pow(pP1->P(),3)*M2/(beam.P()*massNeutron*fabs(beamEnergy*pP1->E()*massNeutron-pP2->E()*pow(massElectron,2)))/dcsConversion;
			if(unpolDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
		
		if(filter=="polarized"){
		  ran3 = maxDCS*randomNum.Rndm();
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){
			double k1=q2*pow(gmn,2);
			double k2=pow(2*massNeutron,2)*(pow(gen,2)+tau*pow(gmn,2))/(1+tau);
			double term1=beam.E()*pP1->E()-beam.Px()*pP1->Px()-beam.Py()*pP1->Py()-beam.Pz()*pP1->Pz()-2*pow(massElectron,2);
			double term2=(beam.E()*target.E()-beam.Px()*target.Px()-beam.Py()*target.Py()-beam.Pz()*target.Pz())*(pP1->E()*target.E()-pP1->Px()*target.Px()-pP1->Py()*target.Py()-pP1->Pz()*target.Pz())/pow(massNeutron,2)-q2/4;
			double M2=pow(2*alpha,2)/pow(q2,2)*(k1*term1+k2*term2);
			unpolDCS=1./4*pow(pP1->P(),3)*M2/(beam.P()*massNeutron*fabs(beamEnergy*pP1->E()*massNeutron-pP2->E()*pow(massElectron,2)))/dcsConversion;
			if(gRandom->Rndm()<percentPosHeli) helicity=1;
			else helicity=-1;
			polTrans=polBeam*(-2*gen*gmn*sqrt(tau*(1+tau))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			polLongi=polBeam*(2*pow(gmn,2)*tau*sqrt((1+tau)*(1+tau*pow(sin(thetaSElectronRad/2),2)))*(1/cos(thetaSElectronRad/2))*tan(thetaSElectronRad/2))/(pow(gen,2)+(tau+2*tau*(1+tau)*pow(tan(thetaSElectronRad/2),2))*pow(gmn,2));
			phaseShift=atan(polTrans/polLongi)/TMath::Pi()*180;
			polDCS=unpolDCS*(polTrans+polLongi);
			if(polDCS>ran3){
			  evtFlag = true;
			  event++;
			} event++;
		  }
		}
	  }
	} while(!(evtFlag));  
  }
  /* End channel 3 */
  
 /* channel 4 */
  else if(channel == 4){
    do {
      // Print out a message every 100 events 
      if (event % 100 == 0){
		fprintf (stderr, "%d\r", event);
		fflush (stderr);
      }
	  
      beam.SetXYZT (0.0, 0.0, sqrt(beamEnergy*beamEnergy - massElectron*massElectron), beamEnergy);
      target=deuteron;
      w = beam + target;
	  
      if (eventED.SetDecay (w, 3, massesENP)){
		eventED.Generate ();
		pP1 = eventED.GetDecay (0);
		pP2 = eventED.GetDecay (1);
		pP3 = eventED.GetDecay (2);
		
		energySElectron=pP1->E();
		thetaSElectronRad=pP1->Theta();
		thetaSElectron=thetaSElectronRad/TMath::Pi()*180;
		pRNeutron=pP2->P();
		thetaRNeutron=pP2->Theta()/TMath::Pi()*180;
		
		q2=pow((beam-*pP1).P(),2)-pow((beam-*pP1).E(),2);
		tau=q2/(4*massNeutron*massNeutron);
		epsilon=pow(1+2*(1+tau)*pow(tan(thetaSElectronRad/2),2),-1);
		
		// Do not filter events by DCS
		if(filter=="none"){
		  if(thetaSElectron>thetaSElectronFree-openAngle/deg/2 && thetaSElectron <thetaSElectronFree+openAngle/deg/2){	
			evtFlag = true;
			event++;
		  }	event++;
		}
      }
    } while(!(evtFlag)); 
  }
  /* End channel 4 */
  else {
	// do nothing
  }

  // Fill the struct with the necessary Lorentz vectors and polarization values
  TLorentzVector *copyVector1 = new TLorentzVector(*pP1);
  TLorentzVector *copyVector2 = new TLorentzVector(*pP2);
  TLorentzVector *copyVector3 = new TLorentzVector(*pP3);
  primeEvent.electronVector = copyVector1; 
  primeEvent.neutronVector = copyVector2; 
  primeEvent.thirdParticleVector = copyVector3; 
  primeEvent.polLong = polLongi;
  primeEvent.polTran = polTrans; 
  
  return;
}


G4double NpolPrimaryGeneratorAction::genCalc(G4double val){
  q2=val;
  tau=q2/4/massNeutron/massNeutron;
  double gD=pow(1+q2/0.71,-2);
  gen=-0.886*(-1.913)*tau*gD/(1+3.29*tau);
  return gen;
}

G4double NpolPrimaryGeneratorAction::gmnCalc(G4double val){
  q2=val;
  double b[5]={3.26,-0.272, 0.0123, -2.52, 2.55};
  gmn=-1.913/(1+b[0]*q2/(1+b[1]*q2/(1+b[2]*q2/(1+b[3]*q2/(1+b[4]*q2)))));
  return gmn;
}

// Set actions for Messenger Class messenging.
void NpolPrimaryGeneratorAction::SetMaxDCSValue(G4double val){ maxDCS = val; }

void NpolPrimaryGeneratorAction::SetFilterValue(G4String val) { filter = val; }

void NpolPrimaryGeneratorAction::SetGenMethodValue(G4String val) { genMethod = val; }

void NpolPrimaryGeneratorAction::SetChannelValue(G4int val) { channel = val; }

void NpolPrimaryGeneratorAction::SetBeamEnergyValue(G4double val) { beamEnergy = val; }

void NpolPrimaryGeneratorAction::SetBeamPolarizationValue(G4double val) { polBeam = val; }

void NpolPrimaryGeneratorAction::SetOpeningAngleValue(G4double val) { openAngle = val; }

void NpolPrimaryGeneratorAction::SetGenValue(G4double val) { gen = val; }

void NpolPrimaryGeneratorAction::SetGmnValue(G4double val) { gmn = val; }

void NpolPrimaryGeneratorAction::SetHelicityRatioValue(G4double val) { helicityRatio = val; }
