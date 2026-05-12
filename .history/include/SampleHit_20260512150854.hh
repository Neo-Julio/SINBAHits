#ifndef SampleHit_h
#define SampleHit_h 1


#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


class SampleHit : public G4VHit
{
  public:
    SampleHit() = default;
    virtual ~SampleHit() = default;

    // Setters
    void SetPID(G4int pid)           { fPid = pid; }
    void SetKinE(G4double e)         { fKinE = e; }
    void SetPos(G4ThreeVector xyz)   { fPos = xyz; }
    void SetPartName(G4String name)  { fPartName = name; }

    // Getters
    G4int         GetPID()      const { return fPid; }
    G4double      GetKinE()     const { return fKinE; }
    G4ThreeVector GetPos()      const { return fPos; }
    G4String      GetPartName() const { return fPartName; }

  private:
    G4int         fPid = 0;
    G4double      fKinE = 0.;
    G4ThreeVector fPos;
    G4String      fPartName = "";
};

typedef G4THitsCollection<SampleHit> SampleHitsCollection;
#endif