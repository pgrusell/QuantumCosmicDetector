#include "QDDetectorConstruction.hh"

QDDetectorConstruction::QDDetectorConstruction(){


}

QDDetectorConstruction::~QDDetectorConstruction(){

    
}

G4VPhysicalVolume *QDDetectorConstruction::Construct(){ // we are defining here our main function Construct

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
    logicBar1 = new G4LogicalVolume(solidBar1, scintillatorMat, "logicBar1");

    G4Box *solidCoating1 = new G4Box("solidCoating1", 0.5 * (barX1 + 2 * coatingThickness), 0.5 * (barY1 + 2 * coatingThickness), 0.5 * barZ1);
    G4LogicalVolume *logicCoating1 = new G4LogicalVolume(solidCoating1, plasticMat, "logicCoating1");

    // bar with coating for the second layer
    G4Box *solidBar2 = new G4Box("solidBar2", 0.5 * barX2, 0.5 * barY2, 0.5 * barZ2);
    logicBar2 = new G4LogicalVolume(solidBar2, scintillatorMat, "logicBar2");
    
    G4Box *solidCoating2 = new G4Box("solidCoating2", 0.5 * barX2, 0.5 * (barY2 + 2 * coatingThickness), 0.5 * (barZ2 + 2 * coatingThickness));
    G4LogicalVolume *logicCoating2 = new G4LogicalVolume(solidCoating2, plasticMat, "logicCoating2");

    // first layer of scintillating bars: aligned with Z direction. Y is fixed (height) and X varies with the bar
    G4int nBarsPlane1 = 60;

    G4double planeX1 = (barX1 + 2*coatingThickness)*nBarsPlane1; // dimension of the plane along X direction: 912 mm
    G4double planeZ1 = barZ1;

    // declaring the locations in which we will place the bars (b) and coatings (c)
    // locations are declared in such a way that the coordinate is the central point of the object in that direction
    // Y, Z locations will be fixed and we iterate over X direction to place the bars

    G4double locY1 = 0.; // fixing Y location of the first plane
    G4double locZ1 = 0.; 
    G4double start_locX1 = (- planeX1 + barX1) * 0.5;

    for (G4int i = 0; i < nBarsPlane1; i++) {

        G4double posX = start_locX1 + i * barX1;
        new G4PVPlacement(0, G4ThreeVector(posX, locY1, locZ1), logicCoating1, "physCoatingBar1", logicWorld, false, i, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(posX, locY1, locZ1), logicBar1, "physBar1", logicWorld, false, i, checkOverlaps);
    }
   

    // second layer of scintillating bars
    G4int nBarsPlane2 = 90;

    G4double planeZ2 = (barZ2 + 2*coatingThickness)*nBarsPlane2; // dimension of the plane along Z direction
    G4double planeX2 = barX2;

    // X, Y locations will be fixed and we iterate over Z direction to place the bars

    G4double locY2 = -500 * mm; // fixing Y location of the first plane
    G4double locX2 = 0.; 

    G4double start_locZ2 = (- planeZ2 + barZ2) * 0.5;

    for (G4int i = 0; i < nBarsPlane2; i++) {
        G4double posZ = start_locZ2 + i * barZ2;
        new G4PVPlacement(0, G4ThreeVector(locX2, locY2, posZ), logicCoating2, "physCoatingBar2", logicWorld, false, i, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(locX2, locY2, posZ), logicBar2, "physBar2", logicWorld, false, i, checkOverlaps);
    }

    // setting colors for the objects
    G4VisAttributes *coatingVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3)); // translucent blue 
    G4VisAttributes *scintillatorVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.7)); //translucent red

    logicCoating1->SetVisAttributes(coatingVisAtt);
    logicCoating2->SetVisAttributes(coatingVisAtt);
    logicBar1->SetVisAttributes(scintillatorVisAtt);
    logicBar2->SetVisAttributes(scintillatorVisAtt);

    return physWorld;

}


void QDDetectorConstruction::ConstructSDandField(){

    if (!logicBar1 || !logicBar2) {
        G4cerr << "Error: logicBar1 o logicBar2 no están inicializados en ConstructSDandField()." << G4endl;
        return;
    };

    G4SDManager *sdManager = G4SDManager::GetSDMpointer();

    if (!sdManager->FindSensitiveDetector("QmioSD")) {
        QDSensitiveDetector *sensDet = new QDSensitiveDetector("QmioSD");

        logicBar1->SetSensitiveDetector(sensDet);
        logicBar2->SetSensitiveDetector(sensDet);

        sdManager->AddNewDetector(sensDet);

    } else {
        G4cerr << "Warning: Sensitive Detector QmioSD ya registrado en SDManager." << G4endl;
    };
    
}



