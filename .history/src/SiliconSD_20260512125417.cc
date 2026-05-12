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
/// \file B4/B4c/src/CalorimeterSD.cc
/// \brief Implementation of the B4c::CalorimeterSD class

#include "SiliconSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


SiliconSD::SiliconSD(const G4String& name, const G4String& hitsCollectionName)
                           
 : G4VSensitiveDetector(name),
      fHCID(-1)  
  
{
  collectionName.insert(hitsCollectionName);
}


SiliconHit* SiliconSD::FindHit(G4int trackID, G4int detNum)
{
    G4int nHits = fHitsCollection->entries();
    for (G4int i = 0; i < nHits; i++) {
        auto hit = (*fHitsCollection)[i];
        if (hit->GetTrackID() == trackID &&
            hit->GetDetNum()  == detNum)  // 👈 importante!
        {
            return hit;
        }
    }
    return nullptr;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiliconSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection
  = new SiliconHitsCollection(SensitiveDetectorName, collectionName[0]);
  G4cout << "SiliconSD::Initialize called. CollectionName: " << collectionName[0] << G4endl;
  G4cout << "[DEBUG] SiliconSD Initialized. Collection name: " << GetName() << G4endl;
  // Add this collection in hce
 // auto hcID
   // = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
 // hce->AddHitsCollection( hcID, fHitsCollection );
 if (fHCID < 0) {
  fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
}
hce->AddHitsCollection(fHCID, fHitsCollection);
  // Create hits
  // fNofCells for cells + one more for total sums
  //for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new SiliconHit());
//  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SiliconSD::ProcessHits(G4Step* step,
                                     G4TouchableHistory*)
  { 
    auto PID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    auto eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Energy deposit
    auto edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    G4int trackID = step->GetTrack()->GetTrackID();

    // Identify detector
    G4String name = step->GetPreStepPoint()
                        ->GetTouchable()
                        ->GetVolume()
                        ->GetName();

    G4int detNum = -1;

    if (name == "Detector0") detNum = 0;
    else if (name == "SiBoxA") detNum = 1;
    else if (name == "SiBoxB") detNum = 2;
    else if (name == "SiBoxC") detNum = 3;
    else if (name == "SiBoxD") detNum = 4;

    //  IMPORTANT: use trackID + detNum
    SiliconHit* hit = FindHit(trackID, detNum);

    if (!hit) {
        hit = new SiliconHit();
        hit->SetTrackID(trackID);
        hit->SetDetNum(detNum);   // 
        fHitsCollection->insert(hit);
    }

    // accumulate energy
    hit->Add(edep, step->GetStepLength());
    hit->SetPID(PID);
    G4cout << "TrackID=" << trackID 
       << " PID=" << PID 
       << " Detector=" << detNum 
       << " Edep=" << edep/keV << " keV"
       << G4endl;



    return true;
}
//     // Only care if particle enters the volume
//     auto preStep = step->GetPreStepPoint();
//   //  if (preStep->GetStepStatus() != fGeomBoundary) return false;

//   // energy deposit+++++
//   auto edep = step->GetTotalEnergyDeposit();
//   if (edep == 0) return false;
   
//   G4int trackID = step->GetTrack()->GetTrackID();
 
//  SiliconHit* hit = FindHitByTrackID(trackI,detNum);
//     if (!hit) {
//         hit = new SiliconHit();
//         hit->SetTrackID(trackID);
//         fHitsCollection->insert(hit);
//     } 
 
//        hit->Add(edep, step->GetStepLength());

//        G4String name = step->GetPreStepPoint()
//                       ->GetTouchable()
//                       ->GetVolume()
//                       ->GetName();

// G4int detNum = -1;

// if (name == "Detector0") detNum = 0;
// else if (name == "SiBoxA") detNum = 1;
// else if (name == "SiBoxB") detNum = 2;
// else if (name == "SiBoxC") detNum = 3;
// else if (name == "SiBoxD") detNum = 4;
 
//    return true;
// }
 
 
 
 
 
  // step length Calo Implementation (Total QUantities)
 /* G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }
 G4cout << "[DEBUG] ProcessHits called. edep = " << edep << " stepLength = " << stepLength << G4endl;
  //if ( edep==0. && stepLength == 0. ) return false;
 if ( edep == 0. ) return false;  // safer
  auto touchable = (step->GetPreStepPoint()->GetTouchable());

  // Get calorimeter cell id


  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[0];

  // Get hit for total accounting
 

  // Add values
  hit->Add(edep, stepLength);*/



/*G4bool SiliconSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Get energy deposited
    G4double edep = aStep->GetTotalEnergyDeposit();

    // Ignore steps with no energy deposition
    if (edep <= 0.) return false;

    // Get step length (only for charged particles)
    G4double stepLength = 0.;
    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
        stepLength = aStep->GetStepLength();

    // Debug print
    G4cout << "[SiliconSD::ProcessHits] Edep = " << G4BestUnit(edep, "Energy")
           << ", StepLength = " << G4BestUnit(stepLength, "Length") << G4endl;

    // Reuse or create first hit
    SiliconHit* hit = nullptr;

    // Use a single hit per event for total accumulation
    if (fHitsCollection->entries() == 0) {
        hit = new SiliconHit();
        fHitsCollection->insert(hit);
    } else {
        hit = (*fHitsCollection)[0];
    }

    // Accumulate edep and track length
    hit->Add(edep, stepLength);

    return true;
}*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiliconSD::EndOfEvent(G4HCofThisEvent*)
{
  if (!fHitsCollection) return;

  auto analysisManager = G4AnalysisManager::Instance();
  auto eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  for (std::size_t i = 0; i < fHitsCollection->entries(); ++i) {
      auto hit = (*fHitsCollection)[i];
      
      if (hit->GetEdep() > 0.) {
          // Match the columns you created in RunAction:
          analysisManager->FillNtupleDColumn(0, hit->GetEdep());        // Eabs
          analysisManager->FillNtupleDColumn(1, hit->GetTrackLength()); // Labs
          analysisManager->FillNtupleDColumn(2, hit->GetTrackID());    // TrackID
          analysisManager->FillNtupleDColumn(3, hit->GetDetNum());     // DetNum
          analysisManager->FillNtupleDColumn(4, eventID);              // EventID
          analysisManager->FillNtupleDColumn(5, hit->GetPID());     // PID (if you have this in your hit class)

          analysisManager->AddNtupleRow(); 
      }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


