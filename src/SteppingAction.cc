//MicroTrackGenerator
#include "SteppingAction.hh"
//ROOT
#include "TTree.h"
//Geant4
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction()
{
  //Set initial flags and values
  fTTreeInitialized = false;
  edepsThisEvent = 0;
}

SteppingAction::~SteppingAction(){ }

void SteppingAction::InitializeTTree()
{
  if (fTTreeInitialized == false)
  {
    //Create the Tree
    pTrackOutputTree = new TTree("Tracks","Track information data");

    //Configure the branches
    pTrackOutputTree->Branch("x [nm]",&x,"x/D");
    pTrackOutputTree->Branch("y [nm]",&y,"y/D");
    pTrackOutputTree->Branch("z [nm]",&z,"z/D");
    pTrackOutputTree->Branch("edep [eV]",&edep,"edep/D");
    
    fTTreeInitialized = true;

    //Additional notes:

    //We don't need the output file because it already has been created in RunAction
    //ROOT will link the newly created TTree to the currently opened TFile
    //ROOT will do garbage collection on the TTree when we close the TFile later too.
    
    //Tree splitting cannot be set because TTree is composed of fundamental data types
    //Tree compression is set when the file is created
  }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{ 
  //Save the positio and edep for every step
  preStep = step->GetPreStepPoint();

  if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()!="Transportation")
  {
    edep = step->GetTotalEnergyDeposit()/eV;
    if(edep != 0)
    {
      x=preStep->GetPosition().x()/nanometer;
      y=preStep->GetPosition().y()/nanometer;
      z=preStep->GetPosition().z()/nanometer;
      pTrackOutputTree->Fill();
      edepsThisEvent += 1;
    }
  }
}    

G4long SteppingAction::ResetEdepsThisEvent()
{
  //Grab the number of edeps this event, zero the internal counter, and return the value
  G4int returnVal = edepsThisEvent;
  edepsThisEvent = 0;
  return returnVal;
}
