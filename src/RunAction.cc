//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
//Extern
#include "CommandLineParser.hh"
//ROOT
#include"TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
//Geant4
#include "G4Run.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"

RunAction::RunAction() : G4UserRunAction() { }
RunAction::~RunAction() { }

//
// Beginning of run action
//

void RunAction::BeginOfRunAction(const G4Run* run)
{
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);

  if(isMaster && sequential == false ) //note that in sequential mode, BeginMaster will never be called. So put MT only things there
  {
    BeginMaster(run);
  }
  else BeginWorker(run);
}

void RunAction::BeginMaster(const G4Run*)
{
  ROOT::EnableThreadSafety(); //make ROOT thread safe from the main thread. ROOT will crash without this.
}

void RunAction::BeginWorker(const G4Run*)
{
  CheckPrimaryGeneratorInitialized();
  CreateTFile(); //Create the output TFile. 
  InitializeTTrees(); //Instruct stepping and event action to initialize
}

//
// End of run action
//

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

void RunAction::EndMaster(const G4Run*) { }

void RunAction::EndWorker(const G4Run*)
{
  pTrackOutputFile->Write(0,TObject::kWriteDelete);
  pTrackOutputFile->Close();

  delete pTrackOutputFile;
}

//
//Thread local worker functions
//

inline void RunAction::CheckPrimaryGeneratorInitialized()
{
  //Get primary generator action pointer and check if initialized (i.e. user messenger parameters have been set)
  pPrimaryGeneratorAction = (PrimaryGeneratorAction*)G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
  pPrimaryGeneratorAction->CheckInitialized();
}

void RunAction::CreateTFile()
{
  //Create an output TFile with appropriate name, random seed and thread ID are appended to name
  G4String fileName;

  //Create a thread local command line parser
  CommandLineParser* parser = CommandLineParser::GetParser();

  //Append the macro name
  fileName.append(parser->GetCommandIfActive("-out")->GetOption());

  //Append the seed
  fileName.append(parser->GetCommandIfActive("-seed")->GetOption());

  //Append the threadID to the filename, if we are running multithreaded
  G4int threadID = G4Threading::G4GetThreadId();
  if (threadID != -2) //Geant4 is in sequential mode if threadID is -2
  {
    fileName.append(("_thread_"+std::to_string(threadID)));
  }

  //Append the filename extension
  fileName.append(".root");

  //Create a thread local File
  //List of compression settings contained here: https://root.cern.ch/doc/master/structROOT_1_1RCompressionSetting_1_1EDefaults.html#a47faae5d3e4bb7b1941775f764730596aa27e7f29058cc84d676f20aea9b86c30
  pTrackOutputFile = new TFile(fileName,"RECREATE");

  //Now that file has been created attach the additional information fields
  WriteTFileInformationFields(); 
}

void RunAction::WriteTFileInformationFields() 
{
  CommandLineParser* parser = CommandLineParser::GetParser();
  G4int threadID = G4Threading::G4GetThreadId(); if (threadID == -2) {threadID = 0;}

  //Get primary generator action pointer
  pPrimaryGeneratorAction = (PrimaryGeneratorAction*)G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();

  //Get and parse the particle origin
  G4ThreeVector particleOrigin = pPrimaryGeneratorAction->GetParticleOrigin();
  G4String originString = std::to_string(particleOrigin.getX()) + ", " + std::to_string(particleOrigin.getY()) + ", " + std::to_string(particleOrigin.getZ());

  //Get and parse the particle initial direction
  G4ThreeVector initialDirection = pPrimaryGeneratorAction->GetParticleInitialDirection();
  G4String directionString = std::to_string(initialDirection.getX()) + ", " + std::to_string(initialDirection.getY()) + ", " + std::to_string(initialDirection.getZ());

  //Get the side length
  G4double sideLength = ((DetectorConstruction*)(G4RunManager::GetRunManager()->GetUserDetectorConstruction()))->GetSideLength();

  G4String physicsListString{};
  
  //G4VModularPhysicsList virtually inherits from G4VUserPhysicsList. So I have to use a dynamic cast (casting at runtime instead of compile 
  //time) to make this cast work. Virtual inheritance exists to eliminate ambiguities with diamond inheritance, but in my opinion 
  //G4VModularPhysicsList could have done without it and it would have worked just fine.
  const PhysicsList* pPhys = dynamic_cast<const PhysicsList*>(G4RunManager::GetRunManager()->GetUserPhysicsList());

  if (pPhys) //check the dynamic cast worked
  {
    physicsListString= pPhys->GetMessenger()->GetPhysicsListString(); //Get the physics list string
  }
  else
    throw std::runtime_error("Dynamic cast from UserPhysicsList to Physics List failed.");

  //Pull properties about primary particles
  TNamed tnPrimaryParticle = TNamed("Primary particle",pPrimaryGeneratorAction->GetPrimaryName());
  TNamed tnPrimaryEnergy = TNamed("Primary energy [MeV]",std::to_string(pPrimaryGeneratorAction->GetPrimaryEnergy()));
  TNamed tnParticleOrigin = TNamed("Primary particle origin [mm]",originString);
  TNamed tnParticleInitialDirection = TNamed("Primary particle direction [x,y,z]",directionString);
  TNamed tnVoxelSideLength = TNamed("Voxel side length [mm]",std::to_string(sideLength));
  TNamed tnRandomSeed = TNamed("Random seed",parser->GetCommandIfActive("-seed")->GetOption());
  TNamed tnRhreadID = TNamed("Thread ID",std::to_string(threadID));
  TNamed tnPhysicsList = TNamed("Physics List", physicsListString);

  //Write properties to the currently open TFile
  tnPrimaryParticle.Write();
  tnPrimaryEnergy.Write();
  tnParticleOrigin.Write();
  tnParticleInitialDirection.Write();
  tnVoxelSideLength.Write();
  tnRandomSeed.Write();
  tnRhreadID.Write();
  tnPhysicsList.Write();
}

void RunAction::InitializeTTrees()
{
  //The TFile has been created so we can instruct the SteppingAction and EvenctAction to initialize their TTrees
  pSteppingAction = (SteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction();
  pEventAction = (EventAction*)G4RunManager::GetRunManager()->GetUserEventAction();

  pSteppingAction->InitializeTTree();
  pEventAction->InitializeEventIndexTree();
}


