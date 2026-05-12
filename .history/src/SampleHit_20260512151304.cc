#include "SampleHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

// Fondamentale per la gestione della memoria in Geant4
G4ThreadLocal G4Allocator<SampleHit>* SampleHitAllocator = nullptr;

SampleHit::SampleHit()
 : G4VHit(),
   fPid(0),
   fKinE(0.),
   fPos(G4ThreeVector()),
   fPartName("")
{}

SampleHit::~SampleHit() {}

// Costruttore di copia
SampleHit::SampleHit(const SampleHit& right)
  : G4VHit()
{
  fPid      = right.fPid;
  fKinE     = right.fKinE;
  fPos      = right.fPos;
  fPartName = right.fPartName;
}

// Operatore di assegnazione
const SampleHit& SampleHit::operator=(const SampleHit& right)
{
  fPid      = right.fPid;
  fKinE     = right.fKinE;
  fPos      = right.fPos;
  fPartName = right.fPartName;
  return *this;
}

// Operatore di confronto
G4bool SampleHit::operator==(const SampleHit& right) const
{
  return (this == &right) ? true : false;
}

// Metodo Draw (opzionale, utile per visualizzare dove avvengono le reazioni nel sample)
void SampleHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.); // Rosso per le interazioni nel sample
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}