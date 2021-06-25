#include "PhysicsList.hh"
#include "G4EmDNAPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4RunManager.hh"


PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    SetDefaultCutValue(1.0*nanometer);
  	SetVerboseLevel(1);

  	RegisterPhysics(new G4EmDNAPhysics());

  	G4ProductionCutsTable::GetProductionCutsTable()->
      SetEnergyRange(10*eV, 1*GeV);
    
    //messenger = new PhysicsListMessenger(this);
}


PhysicsList::~PhysicsList()
{
}
