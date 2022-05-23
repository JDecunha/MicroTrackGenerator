#pragma once

#include "G4UImessenger.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;

class PhysicsListMessenger : public G4UImessenger
{
  public:
	PhysicsListMessenger(PhysicsList* physics_list);
	~PhysicsListMessenger() override;

	void SetNewValue(G4UIcommand* command, G4String value) override;

	inline const G4String& GetPhysicsListString() const { return _physicsListString; };
                         
  private:
	G4String _physicsListString{};
	PhysicsList* _PhysicsList{};
	G4UIdirectory* _physics_directory{};
	G4UIcmdWithAString* _physics_list_cmd{};	
};
