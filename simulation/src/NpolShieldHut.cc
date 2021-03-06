//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	         *
//******************************************************************

// %% NpolShieldHut.cc %%

// Polarimeter Shield hut construction file
// Created: William Tireman - January 2015

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolShieldHut.hh"
#include "NpolPolarimeter.hh"

G4double NpolShieldHut::PosFront = 6.35*m; //-2.0*m; // position of front wall (center)
G4double NpolShieldHut::vertAngle = 0.1000*rad; // 0.100 rad nominal 0.120 rad for 10.25/14.75in gap// vertical acceptance angle
G4double NpolShieldHut::horAngle = 0.160*rad;  // 0.160 rad nomial 0.200 rad for 10in gap horizontal acceptance angle // the max for 56cm wide gap is +/- 100 mrad
G4double NpolShieldHut::NpolAng =  NpolPolarimeter::NpolAng; // Polarimeter central angle
G4double NpolShieldHut::wallThickness = 0.900*m;  // Thickness of the concrete walls (nominal)
G4double NpolShieldHut::wallHeight = 7.0*m; // Nominal height of the shield hut walls
G4double NpolShieldHut::frontWallWidth = 4.8768*m;  // Width of the front and back walls
G4double NpolShieldHut::vertOffSet = 0.3424*m; // Vertical offset for target pivot
G4double NpolShieldHut::OffSetRoof = (wallHeight + wallThickness)/2-vertOffSet; // Roof offset
G4double NpolShieldHut::PosTagger = PosFront + wallThickness/2 + 1.50*cm;  // position of NPOL tagger
G4double NpolShieldHut::sideWallLength = 5.50*m - wallThickness; // length of side walls

G4double NpolShieldHut::PosSide = PosFront + wallThickness/2 + sideWallLength/2; // - 5.5*cm;  // -5*cm for 7*m and +2.5*cm for 5*m
G4double NpolShieldHut::PosBack = PosFront + sideWallLength + wallThickness; // PosFront + sideWallLength;
G4double NpolShieldHut::PosRoof = PosFront  + 0.5*(sideWallLength + wallThickness); 

NpolShieldHut::NpolShieldHut() {
  ConstructHutFrontWall();
  ConstructHutBackWall();
  ConstructHutSideWall();
  ConstructHutRoof();
  ConstructNPOLTagger();
}

NpolShieldHut::~NpolShieldHut() {}

G4String NpolShieldHut::GetName() {
  return G4String("Shield Hut");
}

// Construct a thin vacuum box so we can tag particles passing through the collimator.  
// Place it just a few millimeters off the front steel wall
void NpolShieldHut::ConstructNPOLTagger(){
  
  // Make the NPOL tagger 5*cm wider/taller than the collimator to assure it covers all particles
  // Limit particle flux in post run analysis with acceptance cuts
  G4double xlen = 2*(PosTagger)*tan(horAngle/2) + 5.0*cm;
  G4double ylen = 2*(PosTagger)*tan(vertAngle/2) + 5.0*cm; 
  G4double zlen = 0.200*cm;

  G4Box *NPOLTagger = new G4Box("NPOLTagger",xlen/2,ylen/2,zlen/2);
  NPOLTaggerLV = 
	new G4LogicalVolume(NPOLTagger,NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"NPOLTaggerLV",0,0,0);
  //G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  //NPOLTaggerLV->SetVisAttributes(TaggerVisAtt);
  NPOLTaggerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the front wall of the shield hut from 4 ft by 4 ft by 3 ft blocks
// will simplfy here to a 3 foot deep wall that is 16 feet wide and 15 feet high
void NpolShieldHut::ConstructHutFrontWall() {
  // constants for sizing and positioning
  // front wall sizes
  G4double xlen = frontWallWidth, ylen = wallHeight, zlen = wallThickness;
  // collimator sizing
  G4double xlen1 = 2*(PosFront-zlen/2)*tan(horAngle/2), xlen2 = 2*(PosFront+zlen/2)*tan(horAngle/2);
  G4double ylen1 = 2*(PosFront-zlen/2)*tan(vertAngle/2), ylen2 = 2*(PosFront+zlen/2)*tan(vertAngle/2);

  // Create the necessary solids
  G4Box *Sheet = new G4Box("Sheet",xlen/2, ylen/2, zlen/2);
  G4Trd *Collimator = new G4Trd("Collimator",xlen1/2, xlen2/2, ylen1/2, ylen2/2, (zlen+0.5*m)/2);
  
  // Rotation and translation information for the hole for the beam line
  G4RotationMatrix *yRot = new G4RotationMatrix;
  G4ThreeVector xTrans(+0.0*m, vertOffSet, 0.0*m);
  
  // Create the solid using SubtractionSolid
  G4SubtractionSolid *HutFrontWall = new G4SubtractionSolid("HutFrontWall", Sheet, Collimator, yRot, xTrans);
  
  // Generate the logical volume
  HutFrontWallLV = 
	new G4LogicalVolume(HutFrontWall,NpolMaterials::GetInstance()->GetMaterial("SSteel"),"HutFrontWallLV",0,0,0);
  
  G4VisAttributes *FrontWallVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutFrontWallLV->SetVisAttributes(FrontWallVisAtt);
}

// Construct the back wall of the shield hut from concrete blocks here we
// will simplfy to a 3 foot deep wall that is 16 feet wide and 24 feet high
void NpolShieldHut::ConstructHutBackWall() {
  // Back wall sizes
  G4double xlen = frontWallWidth, ylen = wallHeight, zlen = wallThickness;
  
  G4Box *HutBackWall = new G4Box("HutBackWall",xlen/2, ylen/2, zlen/2);
  HutBackWallLV = new G4LogicalVolume(HutBackWall,
				      NpolMaterials::GetInstance()->GetMaterial("SSteel"),"HutBackWallLV",0,0,0);
  G4VisAttributes *BackWallVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutBackWallLV->SetVisAttributes(BackWallVisAtt);
}

// Construct the side walls (one construct, two placements) for the shield 
// hut out of concrete.
void NpolShieldHut::ConstructHutSideWall() {
  G4double xlen = wallThickness, ylen = wallHeight, zlen = sideWallLength;
  
  G4Box *HutSideWall = new G4Box("HutSideWall",xlen/2, ylen/2, zlen/2);
  HutSideWallLV = new G4LogicalVolume(HutSideWall,
      NpolMaterials::GetInstance()->GetMaterial("Concrete"),"HutSideWallLV",0,0,0);
  G4VisAttributes *SideWallVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutSideWallLV->SetVisAttributes(SideWallVisAtt);
}

// Construct the roof.  The concrete blocks are, if memory serves right, 
// 18 inches in thickness.  In E93-038 we used two layers.  So, we will
// start with 36 inches

void NpolShieldHut::ConstructHutRoof() {
  
  G4double xlen = 4.8768*m, ylen = wallThickness, zlen = 6.25*m;
  G4Box *HutRoof = new G4Box("HutRoof", xlen/2, ylen/2, zlen/2);
  HutRoofLV = new G4LogicalVolume(HutRoof,NpolMaterials::GetInstance()->GetMaterial("Concrete"), "HutRoofLV",0,0,0);
  G4VisAttributes *RoofVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutRoofLV->SetVisAttributes(RoofVisAtt);
}

void NpolShieldHut::Place(G4LogicalVolume *motherLV) {
 
  PlaceCylindrical(NPOLTaggerLV, motherLV, "NPOLTagger", PosTagger, -NpolAng, 0);  
  PlaceCylindrical(HutFrontWallLV, motherLV, "HutFrontWall", PosFront,-NpolAng,-vertOffSet);
  PlaceCylindrical(HutBackWallLV, motherLV, "HutBackWall", PosBack,-NpolAng,-vertOffSet);
  PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(NpolAng) - frontWallWidth/2*cos(NpolAng),-vertOffSet, PosSide*cos(NpolAng) - frontWallWidth/2*sin(NpolAng) , 0*deg, -NpolAng, 0*deg); // right side wall
  PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(NpolAng) + frontWallWidth/2*cos(NpolAng),-vertOffSet, PosSide*cos(NpolAng)+ frontWallWidth/2*sin(NpolAng), 0*deg, -NpolAng, 0); // left side wall
  PlaceCylindrical(HutRoofLV, motherLV, "HutRoof", PosRoof, -NpolAng, OffSetRoof);
}
