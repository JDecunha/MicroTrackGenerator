//MicroTrackGenerator
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
//Geant4
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option2.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* physics_list) : _PhysicsList(physics_list)
{
    // Directories in which commands are organised.
    _physics_directory = new G4UIdirectory("/physics/");
    _physics_directory->SetGuidance("Commands relating to physis list.");

    // Set the voxel side length
    _physics_list_cmd = new G4UIcmdWithAString("/physics/list", this);
    _physics_list_cmd->SetGuidance("Choose the physics list. Current options are DNA0 and DNA2.");
    _physics_list_cmd->SetParameterName("list", false);
    _physics_list_cmd->AvailableForStates(G4State_PreInit);
}


PhysicsListMessenger::~PhysicsListMessenger()
{
    delete _physics_directory;
    delete _physics_list_cmd;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == _physics_list_cmd)
    {
        //We store this, so the runManager can access and save it to the output track file
        _physicsListString = value; 

        if(_physicsListString == "DNA0")
        {
            _PhysicsList->AddEMPhysicsListConstructor(new G4EmDNAPhysics());
        }
        else if(_physicsListString == "DNA2")
        {
            _PhysicsList->AddEMPhysicsListConstructor(new G4EmDNAPhysics_option2());
        }
        else
            throw std::runtime_error("The EM physics list " + value + " is not a valid option (Choose DNA0 or DNA2).");
    }
}

