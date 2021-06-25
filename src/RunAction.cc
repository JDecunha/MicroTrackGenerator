#include "RunAction.hh"
#include "G4Run.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "Analysis.hh"
#include "G4Threading.hh"
#include "CommandLineParser.hh"

//Root headers
#include"TROOT.h"
#include "TTree.h"

using namespace G4DNAPARSER;

void PrintNParticles(std::map<const G4ParticleDefinition*, int>& container);

RunAction::RunAction() : G4UserRunAction(),
      fInitialized(0), fDebug(false)
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* run)
{
  // In this example, we considered that the same class was
  // used for both master and worker threads.
  // However, in case the run action is long,
  // for better code review, this practice is not recommanded.
  //
  // Please note, in the example provided with the Geant4 X beta version,
  // this RunAction class were not used by the master thread.

  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == 
                     G4RunManager::sequentialRM);

  if(isMaster && sequential == false )
  // WARNING : in sequential mode, isMaster == true
  {
    BeginMaster(run);
  }
  else BeginWorker(run);
}

void RunAction::EndOfRunAction(const G4Run* run)
{

  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == 
                     G4RunManager::sequentialRM);

  if(isMaster && sequential == false)
  {
    EndMaster(run);
  }
  else
  {
    EndWorker(run);
  }
}

void RunAction::BeginMaster(const G4Run* run)
{
  if(fDebug)
  {
    bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == 
                       G4RunManager::sequentialRM);
    G4cout << "===================================" << G4endl;
    if(!sequential)
      G4cout << "================ RunAction::BeginMaster" << G4endl;
    PrintRunInfo(run);
    G4cout << "===================================" << G4endl;
  }
  ROOT::EnableThreadSafety(); //make root thread safe for each of the created subprocesses
  //I may not need this since my implementation is already thread safe
}


void RunAction::BeginWorker(const G4Run* run)
{
  if (fDebug)
  {
    G4cout << "===================================" << G4endl;
    G4cout << "================ RunAction::BeginWorker" << G4endl;
    PrintRunInfo(run);
    G4cout << "===================================" << G4endl;
  }
  if(fInitialized == false) InitializeWorker(run);

  CreateHistogram();
}

void RunAction::EndMaster(const G4Run*)
{
}


void RunAction::EndWorker(const G4Run* run)
{
  if(fDebug)
  {
    G4cout << "===================================" << G4endl;
    G4cout << "================ RunAction::EndWorker" << G4endl;
    PrintRunInfo(run);
    G4cout << "===================================" << G4endl;
  }

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0 )
  {
    if(fDebug)
    {
      G4cout << "================ NO EVENTS TREATED IN THIS RUN ==> Exit"
             << G4endl;
    }
    return;
  }

  ///////////////
  // Write Histo
  //
  WriteHistogram();

  ///////////////
  // Complete cleanup
  //
  delete G4AnalysisManager::Instance();

}


void RunAction::InitializeWorker(const G4Run*)
{
  /*RunInitManager::Instance()->Initialize();

  if (fpTrackingAction == 0)
  {
    fpTrackingAction = (TrackingAction*) G4RunManager::GetRunManager()->
        GetUserTrackingAction();

    if(fpTrackingAction == 0 && isMaster == false)
    {
      G4ExceptionDescription exDescrption ;
      exDescrption << "fpTrackingAction is a null pointer. "
          "Has it been correctly initialized ?";
      G4Exception("RunAction::BeginOfRunAction",
          "RunAction001",FatalException, exDescrption);
    }
  }

  fInitialized = true;*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::CreateTTree()
{
  
}
void RunAction::CreateHistogram()
{
  // Book histograms, ntuple

  // Create analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in Analysis.hh

  CommandLineParser* parser = CommandLineParser::GetParser();
  Command* command(0);
  if((command = parser->GetCommandIfActive("-out"))==0) return;

  G4cout << "##### Create analysis manager " << "  " << this << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4cout << "Using " << analysisManager->GetType() <<
      " analysis manager" << G4endl;

  // Create directories

  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);

  // Open an output file
  G4String fileName;
  if(command->GetOption().empty() == false)
  {
    fileName = command->GetOption();
  }
  else
  {
   fileName = "microdosimetry";
  }
  analysisManager->OpenFile(fileName);

  // Creating ntuple

  analysisManager->CreateNtuple("microdosimetry", "physics");
  analysisManager->CreateNtupleDColumn("flagParticle");
  //analysisManager->CreateNtupleDColumn("flagProcess");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("totalEnergyDeposit");
  //analysisManager->CreateNtupleDColumn("stepLength");
  //analysisManager->CreateNtupleDColumn("kineticEnergyDifference");
  //analysisManager->CreateNtupleDColumn("kineticEnergy");
  //analysisManager->CreateNtupleDColumn("cosTheta");
  analysisManager->CreateNtupleIColumn("eventID");
  //analysisManager->CreateNtupleIColumn("trackID");
  //analysisManager->CreateNtupleIColumn("parentID");
  //analysisManager->CreateNtupleIColumn("stepID");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::WriteHistogram()
{
  CommandLineParser* parser = CommandLineParser::GetParser();
  Command* commandLine(0);
  if((commandLine = parser->GetCommandIfActive("-out"))==0) return;

  // print histogram statistics
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // if(!analysisManager->IsActive()) {return; }

  // save histograms
  //
  analysisManager->Write();
  analysisManager->CloseFile();

  if(fDebug)
  {
    G4cout << "================ ROOT FILES HAVE BEEN WRITTEN"
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::PrintRunInfo(const G4Run* run)
{
  G4cout << "================ Run is = "
         << run->GetRunID() << G4endl;
  G4cout << "================ Run type is = "
         << G4RunManager::GetRunManager()->GetRunManagerType() << G4endl;
  G4cout << "================ Event processed = "
         << run->GetNumberOfEventToBeProcessed() << G4endl;
  G4cout << "================ Nevent = "
         << run->GetNumberOfEvent() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrintNParticles(std::map<const G4ParticleDefinition*, int>& container)
{
  std::map<const G4ParticleDefinition*, int>::iterator it;
  for(it = container.begin() ;
      it != container.end(); it ++)
  {
    G4cout << "N " << it->first->GetParticleName() << " : "
        << it->second << G4endl;
  }
}
