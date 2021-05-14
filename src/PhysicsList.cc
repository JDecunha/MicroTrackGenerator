// SIM //
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

// GEANT4 //
#include "G4SystemOfUnits.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4RunManager.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"


PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    messenger = new PhysicsListMessenger(this);
}


PhysicsList::~PhysicsList()
{
}


void PhysicsList::ConstructParticle()
{
    G4BosonConstructor().ConstructParticle();
    G4LeptonConstructor().ConstructParticle();
    G4MesonConstructor().ConstructParticle();
    G4BaryonConstructor().ConstructParticle();
    G4IonConstructor().ConstructParticle();
    G4ShortLivedConstructor().ConstructParticle();
}


void PhysicsList::AddPhysicsList(G4VPhysicsConstructor* physics_list)
{
    RegisterPhysics(physics_list);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

