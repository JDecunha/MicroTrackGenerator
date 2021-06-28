#include "PrimaryGeneratorAction.hh"

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

G4String PrimaryGeneratorAction::GetPrimaryName()
{
	return gun->GetParticleDefinition()->GetParticleName();
}

G4double PrimaryGeneratorAction::GetPrimaryEnergy()
{
	return gun->GetParticleEnergy();
}