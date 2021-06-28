#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4Types.hh"
#include "G4SteppingManager.hh"
#include "G4UserSteppingAction.hh"
#include "TFile.h"
#include "TTree.h"

class RunAction;

class SteppingAction : public G4UserSteppingAction
{
	public:
	  SteppingAction();
	  ~SteppingAction();
	  
	  void UserSteppingAction(const G4Step*);

	private:
		void InitializeTTree();

		TTree *pTrackOutputTree;
		G4double *x,*y,*z,*edep; //We need these as pointers, so we can add them to the heap and have the Tree point there
		G4StepPoint* preStep;
		G4bool fTTreeInitialized;
		friend class RunAction;
};

#endif
