#include "SampleSD.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

SampleSD::SampleSD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
    collectionName.insert(hitsCollectionName);
}

void SampleSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new SampleHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool SampleSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Check for secondaries produced in this step
    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    
    if (secondaries->size() > 0) {
        for (auto sec : *secondaries) {
            SampleHit* newHit = new SampleHit();
            newHit->SetPID(sec->GetDefinition()->GetPDGEncoding());
            newHit->SetPartName(sec->GetDefinition()->GetParticleName());
            newHit->SetKinE(sec->GetKineticEnergy());
            newHit->SetPos(sec->GetPosition()); // Interaction vertex
            
            fHitsCollection->insert(newHit);
        }
    }
    return true;
}

void SampleSD::EndOfEvent(G4HCofThisEvent*)
{
    if (!fHitsCollection) return;

    auto analysisManager = G4AnalysisManager::Instance();
    
    for (G4int i=0; i<fHitsCollection->entries(); ++i) {
        auto hit = (*fHitsCollection)[i];
        
        // Fill Ntuple index 1 (The Sample Ntuple)
        analysisManager->FillNtupleDColumn(1, 0, hit->GetKinE());
        analysisManager->FillNtupleDColumn(1, 1, hit->GetPos().x());
        analysisManager->FillNtupleDColumn(1, 2, hit->GetPos().y());
        analysisManager->FillNtupleDColumn(1, 3, hit->GetPos().z());
        analysisManager->FillNtupleIColumn(1, 4, hit->GetPID());
        
        analysisManager->AddNtupleRow(1);
    }
}