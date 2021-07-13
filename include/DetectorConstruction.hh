#pragma once

#include "G4VUserDetectorConstruction.hh"

class DetectorConstructionMessenger;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	void SetSideLength(G4double sidelength);
	G4double GetSideLength();
                         
private:
	G4VPhysicalVolume* ConstructDetector();    

    DetectorConstructionMessenger* pMessenger;
	
	G4double sideLength;
	G4bool sideLengthInitialized;
};

