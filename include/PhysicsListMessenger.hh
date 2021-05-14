#pragma once

// SIM //
class PhysicsList;

// GEANT4 //
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;


class PhysicsListMessenger : public G4UImessenger
{
  public:
    PhysicsListMessenger(PhysicsList* physics_list);
    virtual ~PhysicsListMessenger();

  public:
    void SetNewValue(G4UIcommand* command, G4String value);

  private:
    PhysicsList* physics_list_;

    G4UIdirectory* phys_dir;

    // Commands under /sim/phys/
    G4UIcmdWithAString* add_physics_command; 
};

