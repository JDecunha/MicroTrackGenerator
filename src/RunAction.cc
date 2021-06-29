//MicroTrackGenerator
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "CommandLineParser.hh"
#include "Analysis.hh"
//Geant4
#include "G4Run.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"
//ROOT
#include"TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace G4DNAPARSER;

RunAction::RunAction() : G4UserRunAction() { }

RunAction::~RunAction() { }

void RunAction::BeginOfRunAction(const G4Run* run)
{
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);

  if(isMaster && sequential == false ) //note that in sequential mode, BeginMaster will never be called. So put MT only things there
  {
    BeginMaster(run);
  }
  else BeginWorker(run);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);

  if(isMaster && sequential == false)
  {
    EndMaster(run);
  }
  else
  {
    EndWorker(run);
  }
}

void RunAction::BeginMaster(const G4Run*)
{
  ROOT::EnableThreadSafety(); //make ROOT thread safe from the main thread. ROOT will crash without this.
}
void RunAction::EndMaster(const G4Run*) { }

void RunAction::BeginWorker(const G4Run*)
{
  CreateTFile(); //Create the output TFile. 
  InitializeTTrees(); //Instruct stepping and event action to initialize
  WriteTFileInformationFields(); //Write information fields to the file
}
void RunAction::EndWorker(const G4Run*)
{
  pTrackOutputFile->Write();
  pTrackOutputFile->Close();
  delete pTrackOutputFile;
}

void RunAction::CreateTFile()
{
  //Get the output filename
  CommandLineParser* parser = CommandLineParser::GetParser();
  Command* command(0);

  if((command = parser->GetCommandIfActive("-out"))==0) 
  {
    return;
  }

  G4String fileName;
  if(command->GetOption().empty() == false)
  {
    fileName = command->GetOption();
  }

  //Append the threadID to the filename, if we are running multithreaded
  G4int threadID = G4Threading::G4GetThreadId();
  if (threadID != -2) //Geant4 is in sequential mode if threadID is -2
  {
    fileName.append(("_thread"+std::to_string(threadID)));
  }
  fileName.append(".root");

  //Create a thread local File
  //List of compression settings contained here: https://root.cern.ch/doc/master/structROOT_1_1RCompressionSetting_1_1EDefaults.html#a47faae5d3e4bb7b1941775f764730596aa27e7f29058cc84d676f20aea9b86c30
  pTrackOutputFile = new TFile(fileName,"RECREATE");
}

void RunAction::InitializeTTrees()
{
  //The TFile has been created so we can instruct the SteppingAction and EvenctAction to initialize their TTrees
  pSteppingAction = (SteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
  pEventAction = (EventAction*)G4RunManager::GetRunManager()->GetUserEventAction();
  pSteppingAction->InitializeTTree();
  pEventAction->InitializeEventIndexTree();
}

void RunAction::WriteTFileInformationFields()
{
  //Get primary generator action pointer
  pPrimaryGeneratorAction = (PrimaryGeneratorAction*)G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();

  //Pull properties about primary particles
  TNamed primaryParticle = TNamed("Primary particle",pPrimaryGeneratorAction->GetPrimaryName());
  TNamed primaryEnergy = TNamed("Primary energy",std::to_string(pPrimaryGeneratorAction->GetPrimaryEnergy()));

  //Write properties to the currently open TFile
  primaryParticle.Write();
  primaryEnergy.Write();
}

