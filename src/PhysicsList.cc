//MicroTrackGenerator
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4RunManager.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4EmDNAPhysics.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    //Create the messenger
    _messenger = new PhysicsListMessenger(this);

    //Set default values
    SetDefaultCutValue(1.0*nanometer);
  	SetVerboseLevel(1);
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(10*eV, 1*GeV);

    //Default physics list
    _emPhysicsConstructor = new G4EmDNAPhysics();
}

PhysicsList::~PhysicsList() 
{
    delete _messenger;
    delete _emPhysicsConstructor;
}

//
// Construct particles and processes 
//

void PhysicsList::ConstructParticle() 
{
    //This is the particle list for Geant4 DNA.
    //Taken from a Geant4 DNA example application.
    G4BosonConstructor  pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();

    G4MesonConstructor pMesonConstructor;
    pMesonConstructor.ConstructParticle();

    G4BaryonConstructor pBaryonConstructor;
    pBaryonConstructor.ConstructParticle();

    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();

    G4ShortLivedConstructor pShortLivedConstructor;
    pShortLivedConstructor.ConstructParticle();

    G4DNAGenericIonsManager* genericIonsManager;
    genericIonsManager=G4DNAGenericIonsManager::Instance();
    genericIonsManager->GetIon("alpha++");
    genericIonsManager->GetIon("alpha+");
    genericIonsManager->GetIon("helium");
    genericIonsManager->GetIon("hydrogen");  
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();
    _emPhysicsConstructor->ConstructProcess();
}


//The messenger calls this function to set the physics list
void PhysicsList::AddEMPhysicsListConstructor(G4VPhysicsConstructor* emPhysicsConstructor)
{
    delete _emPhysicsConstructor;
    _emPhysicsConstructor = emPhysicsConstructor;
}


