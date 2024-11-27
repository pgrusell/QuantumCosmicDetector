#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction(){

};

MyDetectorConstruction::~MyDetectorConstruction(){


};

G4VPhysicalVolume *MyDetectorConstruction::Construct(){

	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist-> FindOrBuildMaterial("G4_AIR");
	
	G4Box *solidWorld = new G4Box("solidWorld",1.*m,1.*m, 1*m);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	// G4PVPlacement( #rotation , #center, #logicVolume, #name, #motherVolume, #booleanOperation, #numCopies, #checkOverlaps)
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	return physWorld; 

}
