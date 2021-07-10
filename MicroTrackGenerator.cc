#include "G4Types.hh"

#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"
#ifdef G4UI_USE_QT
#include "G4UIQt.hh"
#endif

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "CommandLineParser.hh"

using namespace G4DNAPARSER;
CommandLineParser* parser(0);

void Parse(int& argc, char** argv);

int main(int argc,char** argv)
{

  // Parse options given in commandLine
  Parse(argc, argv);
  Command* commandLine(0);

  G4Random::setTheSeed(stol(parser->GetCommandIfActive("-seed")->GetOption())); //I've verified that setting the seed this way works for multithreading,
  //by looking at number of edeps at end of event action in each thread. 
  //Sergio confirms this works too: https://geant4-forum.web.cern.ch/t/different-random-seeds-but-same-results/324

#ifdef G4MULTITHREADED
  G4MTRunManager* runManager= new G4MTRunManager;
  if ((commandLine = parser->GetCommandIfActive("-mt")))
  {
    int nThreads = 2;
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
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);

  runManager->SetUserInitialization(new PhysicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4UIExecutive* ui(0);

  // interactive mode : define UI session
  if ((commandLine = parser->GetCommandIfActive("-gui")))
  {
    ui = new G4UIExecutive(argc, argv,
                           commandLine->GetOption());

    if(ui->IsGUI())
       UImanager->ApplyCommand("/control/execute gui.mac");

    if(parser->GetCommandIfActive("-novis") == 0)
    // visualization is used by default
    {
      if((commandLine = parser->GetCommandIfActive("-vis")))
      // select a visualization driver if needed (e.g. HepFile)
      {
        UImanager->ApplyCommand(G4String("/vis/open ")+
                                commandLine->GetOption());
      }
      else
      // by default OGL is used
      {
        UImanager->ApplyCommand("/vis/open OGL 800x600-0+0");
      }
      UImanager->ApplyCommand("/control/execute vis.mac");
    }
  }
  else
  // to be use visualization file (= store the visualization into
  // an external file:
  // ASCIITree ;  DAWNFILE ; HepRepFile ; VRML(1,2)FILE ; gMocrenFile ...
  {
    if ((commandLine = parser->GetCommandIfActive("-vis")))
    {
      UImanager->ApplyCommand(G4String("/vis/open ")+commandLine->GetOption());
      UImanager->ApplyCommand("/control/execute vis.mac");
    }
  }

  if ((commandLine = parser->GetCommandIfActive("-mac")))
  {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + commandLine->GetOption());
  }
  else
  {
    UImanager->ApplyCommand("/control/execute microdosimetry.in");
  }

  if ((commandLine = parser->GetCommandIfActive("-gui")))
  {
#ifdef G4UI_USE_QT
    G4UIQt* UIQt = static_cast<G4UIQt*> (UImanager->GetG4UIWindow());
    if ( UIQt) {
      UIQt->AddViewerTabFromFile("README", "README from "+ G4String(argv[0]));
    }
#endif
    ui->SessionStart();
    delete ui;
  }
  
  delete visManager;
  delete runManager;

  return 0;
}

// This just gets the path to the executable, mostly to have a default output filename if you don't input one manually
void GetNameAndPathOfExecutable(char** argv,
                                G4String& executable,
                                G4String& path)
{
  // Get the last position of '/'
  std::string aux(argv[0]);

  // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
  int pos = aux.rfind('\\');
#else
  int pos = aux.rfind('/');
#endif

  // Get the path and the name
  path = aux.substr(0, pos + 1);
  executable = aux.substr(pos + 1);
}

void Parse(int& argc, char** argv)
{
  //////////
  // Parse options given in commandLine
  //
  parser = CommandLineParser::GetParser();

  parser->AddCommand("-gui",
                     Command::OptionNotCompulsory,
                    "Select geant4 UI or just launch a geant4 terminal session",
                    "qt");

  parser->AddCommand("-mac",
                     Command::WithOption,
                     "Give a mac file to execute",
                     "macFile.mac");

  parser->AddCommand("-seed",
                   Command::WithOption,
                   "Provide a seed for the random engine",
                   "1");

// You can add your own command, as for instance:
//  parser->AddCommand("-seed",
//                     Command::WithOption,
//                     "Give a seed value in argument to be tested", "seed");
// it is then up to you to manage this option

#ifdef G4MULTITHREADED
  parser->AddCommand("-mt",
                     Command::WithOption,
                     "Launch in MT mode (events computed in parallel)",
                     "2");
#endif

  parser->AddCommand("-vis",
                     Command::WithOption,
                     "Select a visualization driver",
                     "OGL 600x600-0+0");

  parser->AddCommand("-novis",
                     Command::WithoutOption,
                     "Deactivate visualization when using GUI");

  G4String exec;
  G4String path;
  GetNameAndPathOfExecutable(argv,exec, path);

  parser->AddCommand("-out",
                     Command::OptionNotCompulsory,
                     "Output files",
                     exec);

  // If -h or --help is given in option : print help and exit
  if (parser->Parse(argc, argv) != 0) // help is being printed
  {
    // if you are using ROOT, create a TApplication in this condition in order
    // to print the help from ROOT as well
    CommandLineParser::DeleteInstance();
    std::exit(0);
  }

  // Kill application if wrong argument in command line
  if (parser->CheckIfNotHandledOptionsExists(argc, argv))
  {
    // if you are using ROOT, you should initialise your TApplication
    // before this condition
    abort();
  }
}
