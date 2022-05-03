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

    G4String GetPrimaryName() const;
    G4double GetPrimaryEnergy() const;
    G4ThreeVector GetParticleOrigin() const;
    G4ThreeVector GetParticleInitialDirection() const;

    void SetPrimaryParticleType(const G4String& name);
    void SetPrimaryEnergy(const G4double& energy);

  private:
    friend class RunAction;

    void CheckInitialized();

    G4ParticleGun* gun{};
    PrimaryGeneratorMessenger* pPrimaryMessenger{};

    G4bool energyInitialized{};
    G4bool particleTypeInitialized{};
};

