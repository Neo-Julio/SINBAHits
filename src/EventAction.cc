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
/// \file B4/B4c/src/EventAction.cc
/// \brief Implementation of the B4c::EventAction class

#include "EventAction.hh"
#include "SiliconSD.hh"
#include "SiliconHit.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiliconHitsCollection*
EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection
    = static_cast<SiliconHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(
                              G4double absoEdep, G4double absoTrackLength
                            ) const
{
  // print event statistics
  G4cout
     << "   Absorber: total energy: "
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << "       total track length: "
     << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
     << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  if (fAbsHCID == -1) {
    auto mgr = G4SDManager::GetSDMpointer();
    fAbsHCID = mgr->GetCollectionID("SiliconHitsCollection");
    if (fAbsHCID == -1) {
      G4Exception("EventAction::EndOfEventAction",
                  "MyCode001", JustWarning,
                  "SiliconHitsCollection not found in SDManager.");
      return;
    }
  }

  auto absoHC = GetHitsCollection(fAbsHCID, event);
  if (!absoHC || absoHC->entries() == 0) {
    G4cout << "No hits in this event." << G4endl;
    return;
  }

  // Sum over all hits
  G4double totalEdep = 0.;
  G4double totalTrackLength = 0.;

  for (G4int i = 0; i < absoHC->entries(); ++i) {
    auto* hit = (*absoHC)[i];
    totalEdep += hit->GetEdep();
    totalTrackLength += hit->GetTrackLength();
  }

  // Skip events with zero total energy deposition
  if (totalEdep <= 0.) {
    G4cout << "No energy deposited in this event." << G4endl;
    return;
  }

  // Print event stats (if needed)
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ((printModulo > 0) && (eventID % printModulo == 0)) {
    PrintEventStatistics(totalEdep, totalTrackLength);
    G4cout << "--> End of event: " << eventID << "\n" << G4endl;
  }

  // Fill histograms & ntuple
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0, totalEdep);
  analysisManager->FillH1(1, totalTrackLength);
  analysisManager->FillNtupleDColumn(0, totalEdep);
  analysisManager->FillNtupleDColumn(1, totalTrackLength);
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


