#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4UserLimits.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction()
{
  sideLength = 0;
  sideLengthInitialized = false;

  pMessenger = new DetectorConstructionMessenger(this);
}  

DetectorConstruction::~DetectorConstruction()
{
  delete pMessenger;
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

void DetectorConstruction::SetSideLength(G4double sidelength)
{
  sideLength = sidelength;
  sideLengthInitialized = true;
}

G4double DetectorConstruction::GetSideLength()
{
  return sideLength;
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{
  //Define water material
  G4NistManager * man = G4NistManager::Instance();
  G4Material * H2O = man->FindOrBuildMaterial("G4_WATER");

  // Create a voxel within which to confine the tracks
  G4VSolid* solidWorld = new G4Box("World", sideLength/2,sideLength/2,sideLength/2);

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
  
  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //White
  worldVisAtt->SetVisibility(true);
  logicWorld->SetVisAttributes(worldVisAtt);

  return physiWorld;
}
