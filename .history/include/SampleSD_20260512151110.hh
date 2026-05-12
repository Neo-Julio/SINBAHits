#ifndef SampleSD_h
#define SampleSD_h 1

#include "G4VSensitiveDetector.hh"
#include "SampleHit.hh"

class SampleSD : public G4VSensitiveDetector
{
  public:
    SampleSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~SampleSD() = default;

    virtual void   Initialize(G4HCofThisEvent* hce) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void   EndOfEvent(G4HCofThisEvent* hce) override;

  private:
    SampleHitsCollection* fHitsCollection = nullptr;
};
#endif