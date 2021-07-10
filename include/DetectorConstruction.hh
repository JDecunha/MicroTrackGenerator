#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

class DetectorConstructionMessenger;

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	void SetSideLength(G4double sidelength);
	G4double GetSideLength();
                         
private:

	void DefineMaterials();
	G4VPhysicalVolume* ConstructDetector();    

    DetectorConstructionMessenger* pMessenger;
	
	G4Material*        fpWaterMaterial; 
	G4double sideLength;
};
#endif
