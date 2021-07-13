//MicroTrackGenerator
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
//Extern
#include "CommandLineParser.hh"
//Geant4
#include "G4Types.hh"
#include "G4UImanager.hh"
#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif

//Global command line parser
CommandLineParser* parser(0);

//Definitions for functions in main
void Parse(int& argc, char** argv);
void CheckCommandLineInputs(Command* commandLine);

//Implementation
int main(int argc,char** argv)
{
  //Disable Geant4Backtrace signal handling to improve performance
  G4Backtrace::DefaultSignals() = std::set<int>{};

  // Parse options given in commandLine
  Parse(argc, argv);
  Command* commandLine(0);
  //Check that the required command line inputs have been given, if not throw fatal exception
  CheckCommandLineInputs(commandLine);

  //I've verified that setting the seed this way works for multithreading,
  //by looking at number of edeps at end of event action in each thread. 
  //Sergio confirms this works too: https://geant4-forum.web.cern.ch/t/different-random-seeds-but-same-results/324
  G4Random::setTheSeed(stol(parser->GetCommandIfActive("-seed")->GetOption())); 

  //Set up RunManager depending on whether multithreading is enabled
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager= new G4MTRunManager;
  if ((commandLine = parser->GetCommandIfActive("-mt")))
  {
    int nThreads = 2; //Can't use MTRunManager with less than 2 threads
    if(commandLine->GetOption() == "NMAX")
    {
     nThreads = G4Threading::G4GetNumberOfCores();
    }
    else
    {
     nThreads = G4UIcommand::ConvertToInt(commandLine->GetOption());
    }
    runManager->SetNumberOfThreads(nThreads);
  }
#else
  G4RunManager* runManager = new G4RunManager();
#endif

  // Set mandatory user initialization classes
  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new PhysicsList);
  runManager->SetUserInitialization(new ActionInitialization());

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  //Run the macro file
  if ((commandLine = parser->GetCommandIfActive("-mac")))
  {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + commandLine->GetOption());
  }
  
  delete runManager;

  return 0;
}

void CheckCommandLineInputs(Command* commandLine)
{
  //If the input hasn't been given or the input is empty throw a fatal exception
  
  if ((commandLine = parser->GetCommandIfActive("-mac")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Macro file not given. Set with -mac option at runtime" << G4endl;
        G4Exception("MicroTrackGenerator::MicroTrackGenerator", "Macro file NDEF", FatalException, description, "");
    }
  }
  else
  {
        G4ExceptionDescription description;
        description << "Macro file not given. Set with -mac option at runtime" << G4endl;
        G4Exception("MicroTrackGenerator::MicroTrackGenerator", "Macro file NDEF", FatalException, description, "");
  }

  if ((commandLine = parser->GetCommandIfActive("-out")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Output filename not given. Set with -out option at runtime" << G4endl;
        G4Exception("MicroTrackGenerator::MicroTrackGenerator", "Output filename NDEF", FatalException, description, "");
    }
  }
  else
  {
      G4ExceptionDescription description;
      description << "Output filename not given. Set with -out option at runtime" << G4endl;
      G4Exception("MicroTrackGenerator::MicroTrackGenerator", "Output filename NDEF", FatalException, description, "");  
  }

  if ((commandLine = parser->GetCommandIfActive("-seed")))
  {
    if (commandLine->GetOption() == "")
    {
        G4ExceptionDescription description;
        description << "Random seed not given. Set with -seed option at runtime" << G4endl;
        G4Exception("MicroTrackGenerator::MicroTrackGenerator", "Random seed NDEF", FatalException, description, "");
    }
  }
  else
  {
      G4ExceptionDescription description;
      description << "Random seed not given. Set with -seed option at runtime" << G4endl;
      G4Exception("MicroTrackGenerator::MicroTrackGenerator", "Random seed NDEF", FatalException, description, ""); 
  }
}

void Parse(int& argc, char** argv)
{

  // Parse options given in commandLine
  parser = CommandLineParser::GetParser();

  parser->AddCommand("-mac", Command::WithOption,"Give a mac file to execute","");

  parser->AddCommand("-seed",Command::WithOption,"Provide a seed for the random engine","");

  parser->AddCommand("-out",Command::WithOption,"Output filename","");

#ifdef G4MULTITHREADED
  parser->AddCommand("-mt",Command::WithOption,"Launch in MT mode (events computed in parallel)","");
#endif



  // If -h or --help is given in option : print help and exit
  if (parser->Parse(argc, argv) != 0) 
  {
    CommandLineParser::DeleteInstance();
    std::exit(0);
  }

  // Kill application if wrong argument in command line
  if (parser->CheckIfNotHandledOptionsExists(argc, argv))
  {
    abort();
  }
}
