#pragma once

// GEANT4 //
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4GeneralParticleSource.hh"


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* event);
    G4String GetPrimaryName();
    G4double GetPrimaryEnergy();

  private:
    G4GeneralParticleSource* gun;
};

