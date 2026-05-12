//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "SiliconSD.hh"
#include "SiliconHit.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
      //fLogicDetector0(nullptr)
{
        fLogicDetectors.clear();
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* Vacuum_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* Si_mat = nist->FindOrBuildMaterial("G4_Si"); 
  G4Material* Al_mat = nist->FindOrBuildMaterial("G4_Al"); 
  G4Material* Steel_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); 

  G4Element* natLi = nist->FindOrBuildElement("Li");
  G4Element* natF = nist->FindOrBuildElement("F");

  G4Material* LiF_mat = new G4Material("LiF", 2.635*g/cm3, 2);
  LiF_mat->AddElement(natLi, 1);
  LiF_mat->AddElement(natF, 1);

  G4Material* Mylar_mat = nist->FindOrBuildMaterial("G4_MYLAR");


  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_size = 0.7*m;
	  
  G4Box* solidWorld =    
    new G4Box("World",                      //its name
           world_size, world_size, world_size);  	//its size
        
  G4LogicalVolume* logicWorld =                         
        new G4LogicalVolume(solidWorld,		//its solid
           Vacuum_mat,		//its material
              "World");		//its name
                         
  G4VPhysicalVolume* physWorld = 
      new G4PVPlacement(0,                     //no rotation
            G4ThreeVector(),       //at (0,0,0)
            logicWorld,            //its logical volume
            "World",               //its name
            0,                     //its mother  volume
            false,                 //no boolean operation
            0,                     //copy number
            checkOverlaps);        //overlaps checking
 


      //Annular detector

    			// Silicon Annular
		G4double tube_dPhi = 2.* M_PI * rad;
		G4double SiliconInnerRadius = 2.4*cm;
		G4double SiliconOuterRadius = 4.8*cm;
		G4double SiliconThickness = 500.*um;
		G4double DeadLayerThickness = 0.5*um;
		G4double ChordToChord = 89.64*mm;		
		G4double SADDis = 11.0 *cm;
	
		G4VSolid* solidTube = new G4Tubs("solidTube", SiliconInnerRadius, SiliconOuterRadius, SiliconThickness/2., 0., tube_dPhi);
		G4VSolid* solidBox = new G4Box("solidBox", ChordToChord/2., SiliconOuterRadius, SiliconThickness/2.);	
		G4VSolid* solidDetector0 = new G4IntersectionSolid("solidDetector0", solidTube, solidBox);
		G4double SilDetPos0 =  SADDis + SiliconThickness/2;

		G4LogicalVolume* flogicDetector0 = new G4LogicalVolume(solidDetector0,Si_mat,"logicDetector0");
    
    	G4PVPlacement* physDetector0 = new G4PVPlacement(0,      					//no rotation
		         		G4ThreeVector(0.,0., SilDetPos0),                                                          //   G4ThreeVector(0.,0.,DetectorSampleDistance),  	//position
		       			 flogicDetector0,					//its logical volume
		        		"Detector0",						//its name
		        		logicWorld, 					//its mother  volume
		        		false,						//no boolean operation
		        		0,							//copy number
		        		checkOverlaps);					//overlaps checking


                	//--- Silicon dead layer


		double DLPos0 = SADDis - DeadLayerThickness/2. ;
		G4VSolid* solidTubeDL0 = new G4Tubs("solidTubeDL0", SiliconInnerRadius, SiliconOuterRadius, DeadLayerThickness/2., 0., tube_dPhi);
		G4VSolid* solidBoxDL0 = new G4Box("solidBoxDL0", ChordToChord/2., SiliconOuterRadius, DeadLayerThickness/2.);
		G4VSolid* solidDetectorDL = new G4IntersectionSolid("solidDetectorDL", solidTubeDL0, solidBoxDL0);
		G4LogicalVolume* logicDetectorDL0 = new G4LogicalVolume(solidDetectorDL,Al_mat,"logicDetectorDL0");
		new G4PVPlacement(0,      					//no rotation
			        G4ThreeVector(0.,0.,DLPos0 ),  	//position
			        logicDetectorDL0,					//its logical volume
			        "DetectorDL",						//its name
			        logicWorld, 					//its mother  volume
			        false,						//no boolean operation
			        0,							//copy number
			        checkOverlaps);					//overlaps checking

  
//------------------------------------------------------------------------------//

//Square detectors

// ------------------SILICON BOXES ----------------------------------/// A, B, C, D 

G4double SilBoxX = 49.5 *mm;
G4double SilBoxY = 49.5 *mm;
G4double SilBoxZ = 400 *um; 

G4double SilBoxXDL = 49.5*mm;
G4double SilBoxYDL = 49.5*mm;
G4double SilBoxThicknessDL = 500*nm;


G4double AirBoxSideZ = 49.5*mm ;
G4double AirBoxSideY = 10*cm ;
G4double AirBoxSiDistanceX = 4.5 *cm + SilBoxZ/2 ;
G4double AirBoxSiDistanceY = 4.5 *cm + SilBoxZ/2 ;
G4double AirBoxPos = 5*cm + DeadLayerThickness + SiliconThickness + AirBoxSideZ/2; 
G4double AirBoxSiDLY = AirBoxSiDistanceY - SilBoxZ/2 - SilBoxThicknessDL/2;
G4double AirBoxSiDLX = AirBoxSiDistanceX - SilBoxZ/2 - SilBoxThicknessDL/2;

//Air box on which sides the Si boxes take place 
G4VSolid* SolidAirBox = new G4Box("AirBox",AirBoxSideY/2  , AirBoxSideY/2 , AirBoxSideZ/2);
G4LogicalVolume* logicAirBox = new G4LogicalVolume(SolidAirBox,Vacuum_mat,"logicAirBox");

new G4PVPlacement(0,      					//no rotation
G4ThreeVector(0.,0.,AirBoxPos ),  	//position
logicAirBox,					//its logical volume
"AirBox",						//its name
logicWorld, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking

//First Silicon Box  A Up 

G4VSolid* SolidSiBoxA = new G4Box("SiBoxA",SilBoxX/2  , SilBoxY/2 , SilBoxZ/2 );
G4LogicalVolume* logicSiBoxA = new G4LogicalVolume(SolidSiBoxA,Si_mat,"logicSiBoxA");
G4RotationMatrix* rotationA = new G4RotationMatrix();
rotationA->rotateX(90.0 * deg); // Rotate by 90 degrees

new G4PVPlacement(rotationA,      					//no rotation
G4ThreeVector(0.,AirBoxSiDistanceY,0 ),  	//position
logicSiBoxA,					//its logical volume
"SiBoxA",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking

//Silicon Box A DL
G4VSolid* SolidSiBoxADL = new G4Box("SiBoxADL",SilBoxXDL/2  , SilBoxYDL/2 , SilBoxThicknessDL/2 );
G4LogicalVolume* logicSiBoxADL = new G4LogicalVolume(SolidSiBoxADL,Al_mat,"logicSiBoxADL");


new G4PVPlacement(rotationA,      					//no rotation
G4ThreeVector(0.,AirBoxSiDLY,0 ),  	//position
logicSiBoxADL,					//its logical volume
"SiBoxADL",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking

//Second Silicon Box  B Down 

G4VSolid* SolidSiBoxB = new G4Box("SiBoxB",SilBoxX/2  , SilBoxY/2 , SilBoxZ/2 );
G4LogicalVolume* logicSiBoxB = new G4LogicalVolume(SolidSiBoxB,Si_mat,"logicSiBoxB");
G4RotationMatrix* rotationB = new G4RotationMatrix();
rotationB->rotateX(90.0 * deg); // 

new G4PVPlacement(rotationB,      				
G4ThreeVector(0.,-AirBoxSiDistanceY,0 ),  	//position
logicSiBoxB,					//its logical volume
"SiBoxB",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking



//Silicon Box B DL
G4VSolid* SolidSiBoxBDL = new G4Box("SiBoxBDL",SilBoxXDL/2  , SilBoxYDL/2 , SilBoxThicknessDL/2 );
G4LogicalVolume* logicSiBoxBDL = new G4LogicalVolume(SolidSiBoxBDL,Al_mat,"logicSiBoxBDL");


new G4PVPlacement(rotationB,      					
G4ThreeVector(0.,-AirBoxSiDLY,0 ),  	//position
logicSiBoxBDL,					//its logical volume
"SiBoxBDL",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking



//Third Silicon Box  C Left (negative X axis)

G4VSolid* SolidSiBoxC= new G4Box("SiBoxC",SilBoxX/2  , SilBoxY/2 , SilBoxZ/2 );
G4LogicalVolume* logicSiBoxC = new G4LogicalVolume(SolidSiBoxC,Si_mat,"logicSiBoxC");
G4RotationMatrix* rotationC = new G4RotationMatrix();
rotationC->rotateY(90.0 * deg); // 

new G4PVPlacement(rotationC,      				
G4ThreeVector(-AirBoxSiDistanceX,0.,0. ),  	//position
logicSiBoxC,					//its logical volume
"SiBoxC",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking



//Silicon Box C DL
G4VSolid* SolidSiBoxCDL = new G4Box("SiBoxCDL",SilBoxXDL/2  , SilBoxYDL/2 , SilBoxThicknessDL/2 );
G4LogicalVolume* logicSiBoxCDL = new G4LogicalVolume(SolidSiBoxCDL,Al_mat,"logicSiBoxCDL");


new G4PVPlacement(rotationC,      					
G4ThreeVector(-AirBoxSiDLX,0.,0. ),  	//position
logicSiBoxCDL,					//its logical volume
"SiBoxCDL",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking

//Fourth Silicon Box  D Left (positive X axis)

G4VSolid* SolidSiBoxD= new G4Box("SiBoxD",SilBoxX/2  , SilBoxY/2 , SilBoxZ/2 );
G4LogicalVolume* logicSiBoxD = new G4LogicalVolume(SolidSiBoxD,Si_mat,"logicSiBoxD");
G4RotationMatrix* rotationD = new G4RotationMatrix();
rotationD->rotateY(90.0 * deg); // 

new G4PVPlacement(rotationD,      				
G4ThreeVector(AirBoxSiDistanceX,0.,0. ),  	//position
logicSiBoxD,					//its logical volume
"SiBoxD",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking

//Silicon Box D DL
G4VSolid* SolidSiBoxDDL = new G4Box("SiBoxDDL",SilBoxXDL/2  , SilBoxYDL/2 , SilBoxThicknessDL/2 );
G4LogicalVolume* logicSiBoxDDL = new G4LogicalVolume(SolidSiBoxDDL,Al_mat,"logicSiBoxDDL");


new G4PVPlacement(rotationD,      					
G4ThreeVector(AirBoxSiDLX,0.,0. ),  	//position
logicSiBoxDDL,					//its logical volume
"SiBoxDDL",						//its name
logicAirBox, 					//its mother  volume
false,						//no boolean operation
0,							//copy number
checkOverlaps);					//overlaps checking					

fLogicDetectors.push_back(flogicDetector0);   // annular
fLogicDetectors.push_back(logicSiBoxA);       // box A
fLogicDetectors.push_back(logicSiBoxB);       // box B
fLogicDetectors.push_back(logicSiBoxC);       // box C
fLogicDetectors.push_back(logicSiBoxD);       // box D


//sample 
double SampleDX = 0.5*cm;
double SampleDY = 0.5*cm;
double SampleDZ = 400*nm;
G4VSolid* SolidSample = new G4Box("SolidSample",SampleDX/2  , SampleDY/2 , SampleDZ/2 );

//backing material for sample
double BackingDX = 0.5*cm;
double BackingDY = 0.5*cm;
double BackingDZ = 5*um;
G4VSolid* SolidBacking = new G4Box("SolidBacking",BackingDX/2  , BackingDY/2 , BackingDZ/2 );




return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    // Create a new instance of your sensitive detector for this thread
    auto siliconSD = new SiliconSD("SiliconSD", "SiliconHitsCollection");

    // Register it with the SD manager
    G4SDManager::GetSDMpointer()->AddNewDetector(siliconSD);

        for (auto lv : fLogicDetectors)
    {
        if (lv) lv->SetSensitiveDetector(siliconSD);
        G4cout << "Assigning SD to: " << lv->GetName() << G4endl;
    }


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


