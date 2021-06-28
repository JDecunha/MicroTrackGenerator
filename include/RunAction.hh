#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "SteppingAction.hh"
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

  void CreateTFile();
  void WriteTFileHeader();

  void BeginMaster(const G4Run*);
  void EndMaster(const G4Run*);

  void BeginWorker(const G4Run*);
  void EndWorker(const G4Run*);

  SteppingAction *pSteppingAction;
  //Pointer to eventaction goes here later
  TFile *pTrackOutputFile;
};
#endif
