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

		TTree *pTrackOutputTree;

		//Variables needed to populate the TTrees
		G4StepPoint* preStep;
		G4double x,y,z,edep; 
		long long edepsThisEvent;

		//Flags
		G4bool fTTreeInitialized;	
};

