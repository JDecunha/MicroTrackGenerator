#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"

#include "globals.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;

class TrackingAction;

class RunAction : public G4UserRunAction
{
public:
  
  RunAction();
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:

  /////////////////
  // Histogramming
  //
  void CreateHistogram();
  void CreateTTree();
  void WriteHistogram();

  /////////////////
  // Worker
  //
  void BeginMaster(const G4Run*);
  void EndMaster(const G4Run*);

  /////////////////
  // Worker
  //
  void InitializeWorker(const G4Run*);
  void BeginWorker(const G4Run*);
  void EndWorker(const G4Run*);

  /////////////////
  // Print Info
  //
  void PrintRunInfo(const G4Run* run);

  /////////////////
  // Attributes
  //
  bool fInitialized;
  bool fDebug;

};
#endif
