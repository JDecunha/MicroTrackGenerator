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

	void SetSideLength(const G4double& sidelength);
	inline const G4double& GetSideLength() const { return sideLength; } 
                         
private:
	G4VPhysicalVolume* ConstructDetector();    

    DetectorConstructionMessenger* pMessenger{};
	G4double sideLength{};
	G4bool sideLengthInitialized{};
};

