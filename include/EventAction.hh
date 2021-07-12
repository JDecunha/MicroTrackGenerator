#ifndef EventAction_h
#define EventAction_h 1

#include "G4Types.hh"
#include "G4UserEventAction.hh"
#include "TTree.h"

class RunAction;

class EventAction : public G4UserEventAction
{
	public:

	  EventAction();
	  ~EventAction();
	  
	  void EndOfEventAction(const G4Event*);

	private:
		
		friend class RunAction;

		void InitializeEventIndexTree();

		TTree *pEventIndexTree; //provides a series of indices indicating which entry in the TTree is the end of an event
		long indexEntry;

		//Flags
		G4bool fInitialized;
};

#endif
