// SIM //
#include "PrimaryGeneratorAction.hh"

// GEANT4 //
#include "G4GeneralParticleSource.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    gun = new G4GeneralParticleSource();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete gun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    gun->GeneratePrimaryVertex(event);
}

