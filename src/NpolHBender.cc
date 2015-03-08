//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolHBender.cc %%

// Super HMS horizontal bender magnet Constructor file.
// Created: Daniel Wilbern - March 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4GDMLParser.hh"

#include "NpolMaterials.hh"
#include "NpolHBender.hh"

NpolHBender::NpolHBender() {
	G4cout << "Initializing HBender" << G4endl;
}

NpolHBender::~NpolHBender() {
	G4cout << "Deleting HBender" << G4endl;
}

G4VPhysicalVolume *NpolHBender::Construct(G4LogicalVolume *motherLV) {
  
  G4double ShmsAng = 16.0*deg, PosHB = 1.76*m;
  G4GDMLParser parser;
  
  G4String gdmlFilename = "gdml/HBender.gdml";
  parser.Read(gdmlFilename);
  
  G4LogicalVolume *HBBoreLV = parser.GetVolume("HBBoreLogic");
  G4LogicalVolume *HBCoilLV = parser.GetVolume("HBCoilLogic");
  G4LogicalVolume *HBYorkLV = parser.GetVolume("HBYorkLogic");
  G4LogicalVolume *HBCryoBoxLV = parser.GetVolume("HBCryoBoxLogic");
  
  PlaceCylindrical(HBBoreLV,motherLV,"HBBore", PosHB, ShmsAng,0);
  PlaceCylindrical(HBCoilLV,motherLV,"HBCoil", PosHB, ShmsAng,0);
  PlaceCylindrical(HBYorkLV,motherLV,"HBYork", PosHB, ShmsAng,0);
  PlaceCylindrical(HBCryoBoxLV,motherLV,"HBCryoBox", PosHB, ShmsAng,0);
} 

