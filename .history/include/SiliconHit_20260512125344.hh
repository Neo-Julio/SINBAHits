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
/// \file B4/B4c/include/CalorHit.hh
/// \brief Definition of the B4c::CalorHit class

#ifndef B4cCalorHit_h
#define B4cCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"


class SiliconHit : public G4VHit
{
  public:
   SiliconHit() = default;
   SiliconHit(const SiliconHit&) = default;
  ~SiliconHit() override = default;

    // operators
    SiliconHit& operator=(const SiliconHit&) = default;
    G4bool operator==(const SiliconHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    void Draw()  override{}
    void Print() override;

    // methods to handle data
    void Add(G4double de, G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;
    G4int GetPID() const { return fPID; }
    void SetPID(G4int pid) { fPID = pid; }
    //TrackID
    // Getter
    G4int GetTrackID() const { return fTrackID; }
    // Setter
    void SetTrackID(G4int id) { fTrackID = id; }
    // Getter
    G4int GetDetNum() const { return fDetNum; }
    // Setter
    void SetDetNum(G4int id) { fDetNum = id; }

    

  private:
    G4double fEdep = 0.;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength = 0.; ///< Track length in the  sensitive volume
    G4int fTrackID;
    G4int fDetNum = -1; ///< Detector number (0 for annular, 1-4 for boxes)
    G4int fPID = 0;     ///< Particle ID
  };

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using SiliconHitsCollection = G4THitsCollection<SiliconHit>;

extern G4ThreadLocal G4Allocator<SiliconHit>* SiliconHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SiliconHit::operator new(size_t)
{
  if (!SiliconHitAllocator) {
   SiliconHitAllocator = new G4Allocator<SiliconHit>;
  }
  void *hit;
  hit = (void *) SiliconHitAllocator->MallocSingle();
  return hit;
}

inline void SiliconHit::operator delete(void *hit)
{
  if (!SiliconHitAllocator) {
    SiliconHitAllocator = new G4Allocator<SiliconHit>;
  }
  SiliconHitAllocator->FreeSingle((SiliconHit*) hit);
}

inline void SiliconHit::Add(G4double de, G4double dl) {
  fEdep += de;
  fTrackLength += dl;
}

inline G4double SiliconHit::GetEdep() const {
  return fEdep;
}

inline G4double SiliconHit::GetTrackLength() const {
  return fTrackLength;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
