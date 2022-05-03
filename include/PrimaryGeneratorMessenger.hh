#pragma once

#include "G4UImessenger.hh"
#include "G4Types.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class PrimaryGeneratorMessenger : public G4UImessenger
{
  public:
	PrimaryGeneratorMessenger(PrimaryGeneratorAction* primary_action);
	virtual ~PrimaryGeneratorMessenger();

	void SetNewValue(G4UIcommand* command, G4String value);
                         
  private:
	PrimaryGeneratorAction* pPrimaryGenerator{};

	G4UIdirectory* primary_directory{};
	G4UIcmdWithAString* primary_type{};
	G4UIcmdWithADoubleAndUnit* primary_energy{};
};
