#pragma once

#include "G4UserSteppingAction.hh"
#include "G4Types.hh"

class RunAction;
class EventAction;
class G4StepPoint;
class TTree;

class SteppingAction : public G4UserSteppingAction
{
	public:
	  SteppingAction();
	  ~SteppingAction();
	  
	  void UserSteppingAction(const G4Step*);

	private:
		friend class RunAction;
		friend class EventAction;

		void InitializeTTree();
		long long ResetEdepsThisEvent();

		//Variables needed to populate the TTrees
		long long edepsThisEvent{};
		G4double x,y,z,edep{}; 
		G4StepPoint* preStep{};
		TTree *pTrackOutputTree{};

		//Flags
		G4bool fTTreeInitialized{};	
};

