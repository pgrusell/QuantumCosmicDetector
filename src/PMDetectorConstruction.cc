#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction(){


}

PMDetectorConstruction::~PMDetectorConstruction(){

    
}

G4VPhysicalVolume *PMDetectorConstruction::Construct(){ // we are defining here our main function Construct

    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist-> FindOrBuildMaterial("G4_AIR");
    G4Material *scintillatorMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE"); // scintillator bars
    G4Material *plasticMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");     // plastic coating

    G4double xWorld = 2.5 * m;
    G4double yWorld = 2.5 * m;
    G4double zWorld = 2.5 * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5* zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // dimensions of the first layer of scintillator bars
    G4double barX1 = 15.0 * mm;
    G4double barY1 = 15.0 * mm;
    G4double barZ1 = 900.0 * mm;

    G4double barX2 = 1350.0 * mm; // for the second layer
    G4double barY2 = 15.0 * mm;
    G4double barZ2 = 15.0 * mm;
    
    G4double coatingThickness = 0.1 * mm;

    // bar with coating for the first layer
    G4Box *solidBar1 = new G4Box("solidBar1", 0.5 * barX1, 0.5 * barY1, 0.5 * barZ1);
    G4LogicalVolume *logicBar1 = new G4LogicalVolume(solidBar1, scintillatorMat, "logicBar1");

    G4Box *solidCoating1 = new G4Box("solidCoating1", 0.5 * (barX1 + 2 * coatingThickness), 0.5 * (barY1 + 2 * coatingThickness), 0.5 * barZ1);
    G4LogicalVolume *logicCoating1 = new G4LogicalVolume(solidCoating1, plasticMat, "logicCoating1");


    // new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBar1, "physBar1", logicCoating1, false, 0, checkOverlaps);


    // first layer of scintillating bars
    G4int nBarsPlane1 = 60;
    G4double plane1Length = nBarsPlane1 * (barX1 + 2*coatingThickness); //912 mm
    G4double startX1 = - (plane1Length + barX1 + 2*coatingThickness)  /2 ; // center in 0 ?
    G4double yPlane1 = 0.; // location of the first plane

    for (G4int i = 0; i < nBarsPlane1; i++) {
        G4double posX = startX1 + i * (barX1+ 2 * coatingThickness);
        new G4PVPlacement(0, G4ThreeVector(posX, yPlane1, 0), logicCoating1, "physCoatingBar1", logicWorld, false, i, checkOverlaps);
    }


    // bar with coating for the second layer
    G4Box *solidBar2 = new G4Box("solidBar2", 0.5 * barX2, 0.5 * barY2, 0.5 * barZ2);
    G4LogicalVolume *logicBar2 = new G4LogicalVolume(solidBar2, scintillatorMat, "logicBar2");

    G4Box *solidCoating2 = new G4Box("solidCoating2", 0.5 * barX2, 0.5 * (barY2 + 2 * coatingThickness), 0.5 * (barZ2 + 2 * coatingThickness));
    G4LogicalVolume *logicCoating2 = new G4LogicalVolume(solidCoating2, plasticMat, "logicCoating2");

    //new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBar2, "physBar2", logicCoating2, false, 0, checkOverlaps);

    // second layer of scintillating bars
    G4int nBarsPlane2 = 90;
    G4double plane2Length = nBarsPlane2 * (barZ2 + 2*coatingThickness);
    G4double startZ2 = - (plane2Length + barZ2 + 2*coatingThickness)/2;
    G4double yPlane2 = -500.0 * mm; // location of the second plane

    for (G4int i = 0; i < nBarsPlane2; i++) {
        G4double posZ = startZ2 + i * (barZ2 + 2 * coatingThickness);
        new G4PVPlacement(0, G4ThreeVector(0, startZ2, posZ), logicCoating2, "physCoatingBar2", logicWorld, false, i, checkOverlaps);
    }

    return physWorld;


}