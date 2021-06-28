#include "SteppingAction.hh"
#include "G4SystemOfUnits.hh"


SteppingAction::SteppingAction() : G4UserSteppingAction()
{
  //Allocate memory for the variables which will be written to the TTree
  x = new G4double;
  y = new G4double;
  z = new G4double;
  edep = new G4double;

  //Set initial flags
  fTTreeInitialized = false;
}

SteppingAction::~SteppingAction()
{
  //De-allocate memory 
  delete x;
  delete y;
  delete z;
  delete edep;
}

void SteppingAction::InitializeTTree()
{
  if (fTTreeInitialized == false)
  {
    //Create the Tree
    pTrackOutputTree = new TTree("tracks","Track information data");

    //Configure the branches
    pTrackOutputTree->Branch("x",x,"x/D");
    pTrackOutputTree->Branch("y",y,"y/D");
    pTrackOutputTree->Branch("z",z,"z/D");
    pTrackOutputTree->Branch("edep",edep,"edep/D");
    
    fTTreeInitialized = true;

    //Additional notes:
    //We don't need the output file because it already has been created in RunAction
    //ROOT will link the newly created TTree to the currently opened TFile
    //ROOT will do garbage collection on the TTree when we close the TFile later too.
    //
    //Tree splitting cannot be set because TTree is composed of fundamental data types
    //Tree compression is set when the file is created
  }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{ 
  preStep = step->GetPreStepPoint();

  if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()!="Transportation")
  {
    *edep = step->GetTotalEnergyDeposit()/eV;
    if(*edep != 0)
    {
      *x=preStep->GetPosition().x()/nanometer;
      *y=preStep->GetPosition().y()/nanometer;
      *z=preStep->GetPosition().z()/nanometer;
      pTrackOutputTree->Fill();
    }
  }
}    
