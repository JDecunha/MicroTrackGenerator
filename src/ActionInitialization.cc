#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"


ActionInitialization::ActionInitialization()
: G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
 // In MT mode, to be clearer, the RunAction class for the master thread might
 // be different than the one used for the workers.
 // This RunAction will be called before and after starting the
 // workers.
 // For more details, please refer to :
 //https://twiki.cern.ch/twiki/bin/view/Geant4/Geant4MTForApplicationDevelopers
 //

  SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
  // G4cout << "Build for = "
  // << G4RunManager::GetRunManager()->GetRunManagerType()
  // << G4endl;

  SetUserAction(new PrimaryGeneratorAction);

  RunAction* runAction= new RunAction();
  SetUserAction(runAction);

  SetUserAction(new SteppingAction());
}  

