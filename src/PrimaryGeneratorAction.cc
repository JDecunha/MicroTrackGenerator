#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    gun = new G4GeneralParticleSource();

    //Set the primary particle origin position according to the size of the voxel, so it starts on the edge
    G4double sideLength = ((DetectorConstruction*)(G4RunManager::GetRunManager()->GetUserDetectorConstruction()))->GetSideLength();
    gun->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0,0,sideLength/2));
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
	return gun->GetParticleEnergy()/MeV;
}

G4ThreeVector PrimaryGeneratorAction::GetParticleOrigin()
{
    return gun->GetCurrentSource()->GetPosDist()->GetCentreCoords();
}