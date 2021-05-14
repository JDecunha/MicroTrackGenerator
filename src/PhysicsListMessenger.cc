
// SIM //
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

// GEANT4 //
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "G4RunManager.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmDNAPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"


PhysicsListMessenger::PhysicsListMessenger(PhysicsList* physics_list)
                     : physics_list_(physics_list)
{
    // Directories in which commands are organised.
    //sim_dir = new G4UIdirectory("/sim/");
    //sim_dir->SetGuidance("General simulation UI commands.");

    phys_dir = new G4UIdirectory("/sim/phys/");
    phys_dir->SetGuidance("Commands for manipulating the simulation physics.");


    // Load the cells from a file.
    add_physics_command = new G4UIcmdWithAString("/sim/phys/add", this);
    add_physics_command->SetGuidance("Add a physics list..");
    add_physics_command->SetParameterName("physics", false);
    add_physics_command->AvailableForStates(G4State_PreInit);
}


PhysicsListMessenger::~PhysicsListMessenger()
{
    delete add_physics_command;
}


void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == add_physics_command)
    {
        if (value == "livermore")
        {
            physics_list_->AddPhysicsList(new G4EmLivermorePhysics());
        }
 
        else if (value == "penelope")
        {
            physics_list_->AddPhysicsList(new G4EmPenelopePhysics());
        }
 
        else if (value == "dna")
        {
            physics_list_->AddPhysicsList(new G4EmDNAPhysics());
        }
 
        else if (value == "decay")
        {
            physics_list_->AddPhysicsList(new G4DecayPhysics());
            physics_list_->AddPhysicsList(new G4RadioactiveDecayPhysics());
        }
       
        else
            throw std::runtime_error("The physics list " + value + " could not be loaded.");
    }
}

