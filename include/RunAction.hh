#pragma once

#include "G4UserRunAction.hh"

class G4Run;
class TFile;
class TTree;
class EventAction;
class SteppingAction;
class PrimaryGeneratorAction;

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
