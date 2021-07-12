// SIM //
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
// GEANT4 //
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* primary_action) : pPrimaryGenerator(primary_action)
{
    // Directories in which commands are organised.
    primary_directory = new G4UIdirectory("/primary/");
    primary_directory->SetGuidance("Commands relating to primary particle.");

    // 
    primary_type = new G4UIcmdWithAString("/primary/type", this);
    primary_type->SetGuidance("Set the primary particle type.");
    primary_type->SetParameterName("particle type", false);
    primary_type->AvailableForStates(G4State_Idle);

    primary_energy = new G4UIcmdWithADoubleAndUnit("/primary/energy", this);
    primary_energy->SetGuidance("Set the primary particle energy.");
    primary_energy->SetParameterName("particle energy", false);
    primary_energy->SetUnitCategory("Energy");
    primary_energy->AvailableForStates(G4State_Idle);
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
    delete primary_directory;
    delete primary_type;
    delete primary_energy;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == primary_type)
    {
      pPrimaryGenerator->SetPrimaryParticleType(value);
    }
    if (command == primary_energy)
    {
      pPrimaryGenerator->SetPrimaryEnergy(primary_energy->GetNewDoubleValue(value.c_str()));
    }
}

