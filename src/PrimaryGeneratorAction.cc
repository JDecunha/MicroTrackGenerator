//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    particleTypeInitialized = false;
    energyInitialized = false;

    //Create the gun and messenger
    gun = new G4ParticleGun();
    pPrimaryMessenger = new PrimaryGeneratorMessenger(this);

    //Set the primary particle origin position to 0,0,0. 
    //Note that the box around the particle gets shifted in the DetectorConstruction
    gun->SetParticlePosition(G4ThreeVector(0,0,0));

    //Set appropriate direction (down in Z axis)
    gun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));

    //Other particle properties are set by the messenger in the Set functions below
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete gun;
    delete pPrimaryMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

    //Randomly shift the energy so that it spans the entire energy bin rather than being monoenergetic
    double energyRandomTransform = G4UniformRand() - 0.5;
    energyRandomTransform = energyRandomTransform*_energyBinWidth; //Scale so it spans the energy bin we want it to

    //Set the energy
    gun->SetParticleEnergy(_energy+energyRandomTransform);
    // G4cout << _energy+energyRandomTransform << G4endl;

    //Shoot!
    gun->GeneratePrimaryVertex(event);
}

void PrimaryGeneratorAction::CheckInitialized() //This gets called from the RunACtion
{
    if (particleTypeInitialized != true)
    {
        G4ExceptionDescription description;
        description << "Particle type was not defined. Set in macro after initialization with /primary/type" << G4endl;
        G4Exception("PrimaryGeneratorAction::PrimaryGeneratorAction", "Particle type NDEF", FatalException, description, "");
    }
    if (energyInitialized != true)
    {
        G4ExceptionDescription description;
        description << "Particle energy was not defined. Set in macro after initialization with /primary/energy" << G4endl;
        G4Exception("PrimaryGeneratorAction::PrimaryGeneratorAction", "Primary Energy NDEF", FatalException, description, "");
    }

}

// Methods for setting data fields of the generator using the messenger

void PrimaryGeneratorAction::SetPrimaryParticleType(const G4String& name)
{
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(name);
    gun->SetParticleDefinition(particle);
    particleTypeInitialized = true;
}

void PrimaryGeneratorAction::SetPrimaryEnergy(const G4double& energy)
{
    _energy = energy; 
    energyInitialized = true;
}

void PrimaryGeneratorAction::SetPrimaryEnergyBinWidth(const G4double& binWidth)
{
    _energyBinWidth = binWidth;
}

// Methods for accessing data fields of the generator

G4String PrimaryGeneratorAction::GetPrimaryName() const
{
	return gun->GetParticleDefinition()->GetParticleName();
}

G4double PrimaryGeneratorAction::GetPrimaryEnergy() const
{
	return _energy;
}

G4double PrimaryGeneratorAction::GetPrimaryEnergyBinWidth() const
{
    return _energyBinWidth;
}

G4ThreeVector PrimaryGeneratorAction::GetParticleOrigin() const
{
    return gun->GetParticlePosition();
}

G4ThreeVector PrimaryGeneratorAction::GetParticleInitialDirection() const
{
    return gun->GetParticleMomentumDirection();
}