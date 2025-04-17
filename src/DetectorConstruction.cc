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
#include "G4SystemOfUnits.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* Vacuum_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* Si_mat = nist->FindOrBuildMaterial("G4_Si"); 
  G4Material* Al_mat = nist->FindOrBuildMaterial("G4_Al"); 

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

  
//------------------------------------------------------------------------------//
	//--- Silicon detector
		
  		//ANNULAR DETECTORS 
		//-------------------------FIRST ANNULAR DL ON XY plane-----------------------//
		
			// Silicon Annular
			G4double tube_dPhi = 2.* M_PI * rad;
			G4double SiliconInnerRadius = 2.4*cm;
			G4double SiliconOuterRadius = 4.8*cm;
			G4double SiliconThickness = 305.*um;
			G4double DeadLayerThickness = 0.5*um;
			G4double ChordToChord = 89.64*mm;		
			G4double SADSample =5.0 *cm;
	//------------------------------------------------------------------------------//

	
		G4VSolid* solidTube = new G4Tubs("solidTube", SiliconInnerRadius, SiliconOuterRadius, SiliconThickness/2., 0., tube_dPhi);
		G4VSolid* solidBox = new G4Box("solidBox", ChordToChord/2., SiliconOuterRadius, SiliconThickness/2.);
	
		G4VSolid* solidDetector = new G4IntersectionSolid("solidDetector", solidTube, solidBox);
		G4double SilDetPos =  SiliconThickness/2 + SADSample;

		G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector,Si_mat,"logicDetector");

		 G4PVPlacement* physDetector = new G4PVPlacement(0,      					//no rotation
		         		G4ThreeVector(0.,0., SilDetPos),                                                          //   G4ThreeVector(0.,0.,DetectorSampleDistance),  	//position
		       			 logicDetector,					//its logical volume
		        		"Detector",						//its name
		        		logicWorld, 					//its mother  volume
		        		false,						//no boolean operation
		        		0,							//copy number
		        		checkOverlaps);					//overlaps checking

	//------------------------------------------------------------------------------//



	//------------------------------------------------------------------------------//
	//--- Silicon dead layer

		double DLPos = SilDetPos  -SiliconThickness/2 -	 DeadLayerThickness/2;
		
		G4VSolid* solidTubeDL = new G4Tubs("solidTubeDL", SiliconInnerRadius, SiliconOuterRadius, DeadLayerThickness/2., 0., tube_dPhi);

		G4VSolid* solidBoxDL = new G4Box("solidBoxDL", ChordToChord/2., SiliconOuterRadius, DeadLayerThickness/2.);

		G4VSolid* solidDetectorDL = new G4IntersectionSolid("solidDetectorDL", solidTubeDL, solidBoxDL);


		G4LogicalVolume* logicDetectorDL = new G4LogicalVolume(solidDetectorDL,Al_mat,"logicDetectorDL");



		new G4PVPlacement(0,      					//no rotation
			        G4ThreeVector(0.,0.,DLPos ),  	//position
			        logicDetectorDL,					//its logical volume
			        "DetectorDL",						//its name
			        logicWorld, 					//its mother  volume
			        false,						//no boolean operation
			        0,							//copy number
			        checkOverlaps);					//overlaps checking
 
  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicDetector;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
