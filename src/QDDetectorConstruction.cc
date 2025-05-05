#include "QDDetectorConstruction.hh"


QDDetectorConstruction::QDDetectorConstruction()
: flogicBar1(nullptr), flogicBar2(nullptr)  // Initialize to nullptr
{}

QDDetectorConstruction::~QDDetectorConstruction(){}


void QDDetectorConstruction::SetSimulationMode(SimulationMode mode) {
    fMode = mode;
}

SimulationMode QDDetectorConstruction::GetSimulationMode() const {
    return fMode;
}


G4Material* QDDetectorConstruction::DefineScintillatorMaterial() {

    G4Material* scintMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYSTYRENE");

    // Add scintillation properties
    const G4int nEntries = 2;
    G4double energy[nEntries] = {2.0*eV, 3.5*eV};
    G4double scint[nEntries]  = {1.0, 1.0}; // flat spectrum

    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();

    mpt->AddProperty("SCINTILLATIONCOMPONENT1", energy, scint, nEntries, true);
    mpt->AddConstProperty("SCINTILLATIONYIELD", 10000./MeV, true);
    mpt->AddConstProperty("RESOLUTIONSCALE", 1.0, true);
    mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10.*ns, true);
    mpt->AddConstProperty("SCINTILLATIONYIELD1", 1.0, true);
    mpt->AddConstProperty("SCINTILLATIONTYPE", 1, true); // 1 = fast

    scintMat->SetMaterialPropertiesTable(mpt);
    scintMat->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

    return scintMat;
}

void QDDetectorConstruction::DefineMaterials(){

    G4NistManager *nist = G4NistManager::Instance();
    fworldMat = nist-> FindOrBuildMaterial("G4_AIR");

    if (fMode == SimulationMode::PARAMETERIZED || fMode == SimulationMode::BOTH) {
        fscintillatorMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    } else {
        fscintillatorMat = DefineScintillatorMaterial();
    }
    fcoatingMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");     // plastic coating
}


void QDDetectorConstruction::ConstructScintillatorLayer1(G4LogicalVolume* motherVolume) {
    // dimensions of the first layer of scintillator bars
    G4double barX1 = 15.0 * mm;
    G4double barY1 = 15.0 * mm;
    G4double barZ1 = 900.0 * mm;

    // bar with coating for the first layer
    G4Box *solidBar1 = new G4Box("solidBar1", 
        0.5 * barX1, 
        0.5 * barY1, 
        0.5 * barZ1);

    flogicBar1 = new G4LogicalVolume(solidBar1, fscintillatorMat, "logicBar1");

    G4Box *solidCoating1 = new G4Box("solidCoating1", 
        0.5 * (barX1 + 2 * fcoatingThickness), 
        0.5 * (barY1 + 2 * fcoatingThickness), 
        0.5 * barZ1);

    G4LogicalVolume *logicCoating1 = new G4LogicalVolume(solidCoating1, fcoatingMat, "logicCoating1");

    // first layer of scintillating bars: aligned with Z direction. Y is fixed (height) and X varies with the bar
    G4int nBarsPlane1 = 60;

    G4double planeX1 = (barX1 + 2*fcoatingThickness)*nBarsPlane1; // dimension of the plane along X direction: 912 mm
    G4double planeZ1 = barZ1;

    // declaring the locations in which we will place the bars (b) and coatings (c)
    // locations are declared in such a way that the coordinate is the central point of the object in that direction
    // Y, Z locations will be fixed and we iterate over X direction to place the bars

    G4double locY1 = 0.; // fixing Y location of the first plane
    G4double locZ1 = 0.; 
    G4double start_locX1 = (- planeX1 + barX1 + 2*fcoatingThickness) * 0.5;

    for (G4int i = 0; i < nBarsPlane1; i++) {

        G4double posX = start_locX1 + i * (barX1 + 2*fcoatingThickness);

        G4String logicCoatingName = "logicCoating1_" + std::to_string(i);
        G4String coatingName = "physCoating1_" + std::to_string(i);
        G4String barName = "physBar1_" + std::to_string(i);

        // independent logic coating for each bar
        G4LogicalVolume* logicCoating1_i = new G4LogicalVolume(solidCoating1, fcoatingMat, logicCoatingName);

        logicCoating1_i->SetVisAttributes(fcoatingVisAtt); 

        // First place the coating in the world
        G4VPhysicalVolume* physCoating1 = new G4PVPlacement(0, 
            G4ThreeVector(posX, locY1, locZ1),
            logicCoating1_i,
            coatingName,
            flogicWorld,  // Parent volume is the world
            false,
            i,
            fcheckOverlaps);

        // Then place the bar inside its coating at the center (0,0,0) relative to coating
        new G4PVPlacement(0,
            G4ThreeVector(0., 0., 0.),  // Centered in coating
            flogicBar1,
            barName,
            logicCoating1_i,  // Parent volume is the coating
            false,
            i,
            fcheckOverlaps);    
        }
}

void QDDetectorConstruction::ConstructScintillatorLayer2(G4LogicalVolume* motherVolume) {

    G4double barX2 = 1350.0 * mm; // for the second layer
    G4double barY2 = 15.0 * mm;
    G4double barZ2 = 15.0 * mm;

    // bar with coating for the second layer
    G4Box *solidBar2 = new G4Box("solidBar2", 0.5 * barX2, 0.5 * barY2, 0.5 * barZ2);
    flogicBar2 = new G4LogicalVolume(solidBar2, fscintillatorMat, "logicBar2");

        
    G4Box *solidCoating2 = new G4Box("solidCoating2", 0.5 * barX2, 0.5 * (barY2 + 2 * fcoatingThickness), 0.5 * (barZ2 + 2 * fcoatingThickness));
    G4LogicalVolume *logicCoating2 = new G4LogicalVolume(solidCoating2, fcoatingMat, "logicCoating2");

    // second layer of scintillating bars
    G4int nBarsPlane2 = 90;

    G4double planeZ2 = (barZ2 + 2*fcoatingThickness)*nBarsPlane2; // dimension of the plane along Z direction
    G4double planeX2 = barX2;
    
    // X, Y locations will be fixed and we iterate over Z direction to place the bars
    G4double locY2 = -500 * mm; // fixing Y location of the first plane
    G4double locX2 = 0.; 
    
    G4double start_locZ2 = (- planeZ2 + barZ2 + 2*fcoatingThickness) * 0.5;
    
    for (G4int i = 0; i < nBarsPlane2; i++) {
        G4double posZ = start_locZ2 + i * (barZ2 + 2*fcoatingThickness);
    
        // Create unique name for each physical volume
        G4String logicCoatingName = "logicCoating2_" + std::to_string(i);
        G4String coatingName = "physCoating2_" + std::to_string(i);
        G4String barName = "physBar2_" + std::to_string(i);
            
        G4LogicalVolume* logicCoating2_i = new G4LogicalVolume(solidCoating2, fcoatingMat, logicCoatingName);
    
        logicCoating2_i->SetVisAttributes(fcoatingVisAtt);
    
        // First place the coating in the world
        G4VPhysicalVolume* physCoating2 = new G4PVPlacement(0, 
            G4ThreeVector(locX2, locY2, posZ),
            logicCoating2_i,
            coatingName,
            flogicWorld,  // Parent volume is the world
            false,
            i,
            fcheckOverlaps);
    
        // Then place the bar inside its coating at the center (0,0,0) relative to coating
        new G4PVPlacement(0, 
            G4ThreeVector(0., 0., 0.),  // Centered in coating
            flogicBar2, 
            barName, 
            logicCoating2_i, // Parent volume is the coating
            false, 
            i, 
            fcheckOverlaps);
        }

}

G4VPhysicalVolume* QDDetectorConstruction::DefineVolumes(){

    G4double xWorld = 5 * m;
    G4double yWorld = 5 * m;
    G4double zWorld = 5 * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5* zWorld);
    flogicWorld = new G4LogicalVolume(solidWorld, fworldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), flogicWorld, "physWorld", 0, false, 0, fcheckOverlaps);
 
        
    // Define visualization attributes
        
    fcoatingVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3)); // translucent blue 
        
    fscintillatorVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.7)); //translucent red

    // First layer - vertical bars (along Z)
    ConstructScintillatorLayer1(flogicWorld);
    
    // Second layer - horizontal bars (along X)
    ConstructScintillatorLayer2(flogicWorld);



    flogicBar1->SetVisAttributes(fscintillatorVisAtt);
    flogicBar2->SetVisAttributes(fscintillatorVisAtt);


    if (!physWorld) {
        G4Exception("QDDetectorConstruction::Construct()",
                   "MyCode1",
                   FatalException,
                   "World volume is null");
    }

    return physWorld;
}

G4VPhysicalVolume *QDDetectorConstruction::Construct(){ // we are defining here our main function Construct
    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();
}


void QDDetectorConstruction::ConstructSDandField(){

    if (!flogicBar1 || !flogicBar2) {
        G4Exception("QDDetectorConstruction::ConstructSDandField()",
                   "MyCode0",
                   FatalException,
                   "Logical volumes not initialized");
        return;
    };

    // Create and register the SD
    auto sdManager = G4SDManager::GetSDMpointer();
    auto sensDet = new QDSensitiveDetector("QmioSD");
    sdManager->AddNewDetector(sensDet);

    // Set the sensitive detector to logical volumes
    flogicBar1->SetSensitiveDetector(sensDet);
    flogicBar2->SetSensitiveDetector(sensDet);
    
}



