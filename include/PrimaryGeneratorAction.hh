#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* event);

    G4String GetPrimaryName();
    G4double GetPrimaryEnergy();
    G4ThreeVector GetParticleOrigin();
    G4ThreeVector GetParticleInitialDirection();

    void SetPrimaryParticleType(G4String name);
    void SetPrimaryEnergy(G4double energy);

  private:

    friend class RunAction;

    void CheckInitialized();

    G4ParticleGun* gun;
    PrimaryGeneratorMessenger* pPrimaryMessenger;

    G4bool energyInitialized;
    G4bool particleTypeInitialized;
};

