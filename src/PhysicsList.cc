//MicroTrackGenerator
#include "PhysicsList.hh"
//Geant4
#include "G4EmDNAPhysics_option2.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    SetDefaultCutValue(1.0*nanometer);
  	SetVerboseLevel(1);

  	RegisterPhysics(new G4EmDNAPhysics_option2());

  	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(10*eV, 1*GeV);
}

PhysicsList::~PhysicsList() { }
