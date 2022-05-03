//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"
//Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction()
{
  pMessenger = new DetectorConstructionMessenger(this);
}  

DetectorConstruction::~DetectorConstruction()
{
  delete pMessenger;
}

void DetectorConstruction::SetSideLength(const G4double& sidelength)
{
  sideLength = sidelength;
  sideLengthInitialized = true;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  if (sideLengthInitialized == false)
  {
      G4ExceptionDescription description;
      description << "Voxel side length was not defined. Set in macro before initialization with /geometry/voxelSideLength" << G4endl;
      G4Exception("DetectorConstruction::Construct()", "Voxel side length NDEF.", FatalException, description, "");
  }
  return ConstructDetector();
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  //Define water material
  G4NistManager * man = G4NistManager::Instance();
  G4Material * H2O = man->FindOrBuildMaterial("G4_WATER");

  //Create the world volume
  //The world volume takes care of killing tracks in X and Y, because it is sidelength long in those axes.
  //But in the Z axis it is 2x sidelength long, so I have to kill the particles that backscatter in steppingaction.
  //If we could offcenter the world cube we would not need the extra complexity
  //of killing in steppingaction
  G4VSolid* solidWorld = new G4Box("World", sideLength/2,sideLength/2,sideLength);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  //its solid
                                    H2O,  //its material
                                    "World");    //its name

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,      //no rotation
                                  G4ThreeVector(),  //at (0,0,0), a requirement of the mother volume
                                  "World",    //its name
                                  logicWorld,    //its logical volume
                                  0,      //its mother  volume
                                  false,      //no boolean operation
                                  0);      //copy number
  
  return physiWorld;
}
