#pragma once

class G4VPhysicsConstructor;
class PhysicsListMessenger;
#include "G4VModularPhysicsList.hh"

class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

    void ConstructProcess() override;
    void ConstructParticle() override;

    void AddEMPhysicsListConstructor(G4VPhysicsConstructor* emPhysicsConstructor);
    const PhysicsListMessenger* GetMessenger() const { return _messenger; };

  private:
    PhysicsListMessenger* _messenger{};
    G4VPhysicsConstructor* _emPhysicsConstructor{};
};

