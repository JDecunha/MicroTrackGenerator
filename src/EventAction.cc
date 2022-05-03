//MicroTrackGenerator
#include "EventAction.hh"
#include "SteppingAction.hh"
//ROOT
#include "TTree.h"
//Geant4
#include "G4Types.hh"
#include "G4RunManager.hh"

EventAction::EventAction() : G4UserEventAction() 
{ 
  fInitialized = false; 
  indexEntry = 0;
}

EventAction::~EventAction() { }

void EventAction::InitializeEventIndexTree()
{
  if (fInitialized == false)
  {
    //Create the Tree
    pEventIndexTree = new TTree("Track index","Entry number for the end of each track");

    //Configure the branch
    pEventIndexTree->Branch("index",&indexEntry,"unitless");

    fInitialized = true;
  }
}

void EventAction::EndOfEventAction(const G4Event*)
{
  //Add number of edeps this event to the running total
  indexEntry += ((SteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction())->ResetEdepsThisEvent();
  pEventIndexTree->Fill();
}
