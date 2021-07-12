#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "TFile.h"
#include "TTree.h"

#include "globals.hh"
#include <iostream>

class G4Run;

class RunAction : public G4UserRunAction
{
public:
  
  RunAction();
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:

  inline void CheckPrimaryGeneratorInitialized();
  void CreateTFile();
  void InitializeTTrees();
  void WriteTFileInformationFields();

  void BeginMaster(const G4Run*);
  void EndMaster(const G4Run*);

  void BeginWorker(const G4Run*);
  void EndWorker(const G4Run*);

  
  TFile *pTrackOutputFile;
  
  PrimaryGeneratorAction *pPrimaryGeneratorAction;
  SteppingAction *pSteppingAction;
  EventAction *pEventAction;
  
};
#endif
