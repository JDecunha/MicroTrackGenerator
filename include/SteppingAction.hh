#ifndef SteppingAction_h
#define SteppingAction_h 1

//Geant4
#include "G4Types.hh"
#include "G4SteppingManager.hh"
//ROOT
#include "TTree.h"
//MicroTrackGenerator
#include "G4UserSteppingAction.hh"

class RunAction;
class EventAction;

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
		G4long ResetEdepsThisEvent();

		TTree *pTrackOutputTree;

		//Variables needed to populate the TTrees
		G4StepPoint* preStep;
		G4double x,y,z,edep; 
		G4long edepsThisEvent;

		//Flags
		G4bool fTTreeInitialized;	
};

#endif
