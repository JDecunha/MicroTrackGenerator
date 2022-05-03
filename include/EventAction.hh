#pragma once

#include "G4UserEventAction.hh"
#include "G4Types.hh"

class RunAction;
class TTree;

class EventAction : public G4UserEventAction
{
  public:
	EventAction();
	~EventAction();

	void EndOfEventAction(const G4Event*);

  private:
	friend class RunAction;

	void InitializeEventIndexTree();

	long long indexEntry{};
	//provides a series of indices for which entry in Tracks TTree is the end of an event
	TTree *pEventIndexTree{}; 
	//Flags
	G4bool fInitialized{};
};

		

