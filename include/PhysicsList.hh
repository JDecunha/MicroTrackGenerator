#pragma once

// SIM //
class PhysicsListMessenger;

// GEANT4 //
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;


class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

  public:
    //void ConstructParticle();
    //void ConstructProcess();

    //void AddPhysicsList(G4VPhysicsConstructor* physics_list);

  private:
    //PhysicsListMessenger* messenger;
};

