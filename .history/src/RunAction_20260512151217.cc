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
/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
   // set printing event number per each event
   G4RunManager::GetRunManager()->SetPrintProgress(1);

   // Create analysis manager
   // The choice of the output format is done via the specified
   // file extension.
   auto analysisManager = G4AnalysisManager::Instance();
 
   // Create directories
   //analysisManager->SetHistoDirectoryName("histograms");
   //analysisManager->SetNtupleDirectoryName("ntuple");
   analysisManager->SetVerboseLevel(1);
   analysisManager->SetNtupleMerging(true);
     // Note: merging ntuples is available only with Root output
 
   // Book histograms, ntuple
   //
 
   // Creating histograms
  // analysisManager->CreateH1("Eabs","Edep in absorber", 110, 0., 6*MeV);//0
  // analysisManager->CreateH1("Labs","trackL in absorber", 100, 0., 50*cm);//1

 
   // Creating ntuple0 fpr silicon hits
   //
  analysisManager->CreateNtuple("SINBA", "SiliconHits");
  analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->CreateNtupleDColumn("Labs");
  analysisManager->CreateNtupleDColumn("TrackID");
  analysisManager->CreateNtupleDColumn("DetNum");
  analysisManager->CreateNtupleDColumn("EventID");
  analysisManager->CreateNtupleDColumn("PID");  

    // Enter/Start X, Y, Z  
  analysisManager->CreateNtupleDColumn("PosEnterX");
  analysisManager->CreateNtupleDColumn("PosEnterY");
  analysisManager->CreateNtupleDColumn("PosEnterZ");

    // Exit/Stop X, Y, Z
  analysisManager->CreateNtupleDColumn("PosExitX");
  analysisManager->CreateNtupleDColumn("PosExitY");
  analysisManager->CreateNtupleDColumn("PosExitZ"); 


  // nTUPLA 1 for SampleSD hits
  // NTUPLE 1: Sample Interactions
analysisManager->CreateNtuple("Sample", "Secondary Production");
analysisManager->CreateNtupleDColumn("KinE"); // 0
analysisManager->CreateNtupleDColumn("X");    // 1
analysisManager->CreateNtupleDColumn("Y");    // 2
analysisManager->CreateNtupleDColumn("Z");    // 3
analysisManager->CreateNtupleIColumn("PID");  // 4
analysisManager->FinishNtuple();
 
}
 
 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
   //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "SAD.root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  // G4String fileName = "B4.hdf5";
  // G4String fileName = "B4.xml";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(0) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl;
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl;
    }

    G4cout << " EAbs : mean = "
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy")
       << " rms = "
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;



    G4cout << " LAbs : mean = "
      << G4BestUnit(analysisManager->GetH1(1)->mean(), "Length")
      << " rms = "
      << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Length") << G4endl;

  }

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


