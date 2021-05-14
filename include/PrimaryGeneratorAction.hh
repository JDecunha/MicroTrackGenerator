#pragma once

// GEANT4 //
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4GeneralParticleSource;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* event);

  private:
    G4GeneralParticleSource* gun;
};

