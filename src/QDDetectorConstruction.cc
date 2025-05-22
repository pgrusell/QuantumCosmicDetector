#include "QDDetectorConstruction.hh"


QDDetectorConstruction::QDDetectorConstruction()
: flogicBar1(nullptr), 
  flogicBar2(nullptr), 
  flogicSiPM(nullptr),
  fMode(SimulationMode::INTERNAL),
  fMessenger(nullptr)  
{

    fMessenger = new QDDetectorMessenger(this);
}

QDDetectorConstruction::~QDDetectorConstruction(){
    CleanupGeometry();
    if (fMessenger) {
        delete fMessenger;
        fMessenger = nullptr;
    }
}

SimulationMode QDDetectorConstruction::GetSimulationMode() const {
    return fMode;
}

void QDDetectorConstruction::SetSimulationMode(SimulationMode mode) { 
    G4cout << "=== Setting simulation mode ===" << G4endl;
                
    try {
        // Store current mode for comparison
        SimulationMode oldMode = fMode;
        fMode = mode;
                
        if (oldMode != fMode) {
            // Only cleanup and reinitialize if mode actually changed
            CleanupGeometry();
                    
            if (auto runManager = G4RunManager::GetRunManager()) {
                runManager->GeometryHasBeenModified();
                // Don't force immediate reconstruction
                // Let /run/initialize handle it
                }
            }
                
            G4cout << "Mode changed to: " 
                << (fMode == SimulationMode::INTERNAL ? "INTERNAL" :
                    fMode == SimulationMode::PARAMETERIZED ? "PARAMETERIZED" :
                    fMode == SimulationMode::BOTH ? "BOTH" : "NONE") << G4endl;
                
        } catch (const std::exception& e) {
            G4cerr << "Exception during mode change: " << e.what() << G4endl;
        }
}

void QDDetectorConstruction::CleanupGeometry() {

    G4GeometryManager::GetInstance()->OpenGeometry();

    // Limpia stores de volúmenes y sólidos
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // reset materials without deleting them !! 
    // segmentation fault occurs if we delete them
    fworldMat = nullptr;
    fscintillatorMat = nullptr;
    fcoatingMat = nullptr;
    fsipmMat = nullptr;
    fqpuMat = nullptr;

    // Cleanup visualization attributes
    delete fcoatingVisAtt;       fcoatingVisAtt = nullptr;
    delete fscintillatorVisAtt;  fscintillatorVisAtt = nullptr;
    delete fsipmVisAtt;          fsipmVisAtt = nullptr;
    delete fqpuVisAtt;           fqpuVisAtt = nullptr;

    // Cleanup optical surfaces
    delete fScintCoatingSurface; fScintCoatingSurface = nullptr;

    // Reset logical volume pointers
    flogicWorld = nullptr;
    flogicBar1 = nullptr;
    flogicBar2 = nullptr;
    flogicSiPM = nullptr;
    flogicQPU = nullptr;

    delete fSensitiveBarDetector;   fSensitiveBarDetector = nullptr;
    delete fSensitiveSiPMDetector;  fSensitiveSiPMDetector = nullptr;
    delete fSensitiveQPUDetector;   fSensitiveQPUDetector = nullptr;
}

void QDDetectorConstruction::UpdateGeometry() {
    CleanupGeometry();
    G4RunManager::GetRunManager()->DefineWorldVolume(this->Construct());
}


void QDDetectorConstruction::DefineScintillatorMaterials() {
    G4NistManager *nist = G4NistManager::Instance();

    // Get base materials
    fworldMat = nist->FindOrBuildMaterial("G4_AIR");
    fscintillatorMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    fcoatingMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    fsipmMat = nist->FindOrBuildMaterial("G4_Si");
    fqpuMat = nist->FindOrBuildMaterial("G4_Si");

    const G4int nEntries = 2;
    G4double energy[nEntries] = {2.0*eV, 3.5*eV};
    G4double scint[nEntries]  = {1.0, 1.0}; // flat spectrum

    G4MaterialPropertiesTable* scintMPT = new G4MaterialPropertiesTable();

    scintMPT->AddProperty("SCINTILLATIONCOMPONENT1", energy, scint, nEntries, true);
    scintMPT->AddConstProperty("SCINTILLATIONYIELD", 10000./MeV, true);
    scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.0, true);
    scintMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10.*ns, true);
    scintMPT->AddConstProperty("SCINTILLATIONYIELD1", 1.0, true);
    scintMPT->AddConstProperty("SCINTILLATIONTYPE", 1, true); // 1 = fast

    fscintillatorMat->SetMaterialPropertiesTable(scintMPT);
    fscintillatorMat->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
    
    G4cout << "=== Basic scintillator properties set ===" << G4endl;
    scintMPT->DumpTable();

    fScintCoatingSurface = new G4OpticalSurface("ScintCoatingSurface");
    fScintCoatingSurface->SetType(dielectric_dielectric);
    fScintCoatingSurface->SetFinish(groundfrontpainted); // superficie rugosa pintada
    fScintCoatingSurface->SetModel(unified);             // modelo UNIFIED de Geant4

    G4MaterialPropertiesTable* coatingMPT = new G4MaterialPropertiesTable();

    std::vector<G4double> photonEnergy = {1.5 * eV, 3.5 * eV};
    std::vector<G4double> reflectivity = {1.0, 1.0};  // 100% reflectante
    coatingMPT->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);

    fScintCoatingSurface->SetMaterialPropertiesTable(coatingMPT);


}

void QDDetectorConstruction::DefineMaterials(){

    G4NistManager *nist = G4NistManager::Instance();

    fworldMat = nist-> FindOrBuildMaterial("G4_AIR");
    fscintillatorMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    fcoatingMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");     // plastic coating
    fsipmMat = nist->FindOrBuildMaterial("G4_Si");
    fqpuMat = nist->FindOrBuildMaterial("G4_Si");
}

void QDDetectorConstruction::ConstructScintillatorLayer1(G4LogicalVolume* motherVolume) {
    // dimensions of the first layer of scintillator bars
    G4double barX1 = 15.0 * mm;
    G4double barY1 = 15.0 * mm;
    G4double barZ1 = 900.0 * mm;

    G4double sipmX = 15.0 * mm;  // same as barX1
    G4double sipmY = 15.0 * mm;  // same as barY1
    G4double sipmZ = 3.0 * mm;   // small thickness

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


    G4Box* solidSiPM = new G4Box("solidSiPM", 0.5 * sipmX, 0.5 * sipmY, 0.5 * sipmZ);
    flogicSiPM = new G4LogicalVolume(solidSiPM, fsipmMat, "logicSiPM");
    flogicSiPM->SetVisAttributes(fsipmVisAtt);

    // first layer of scintillating bars: aligned with Z direction. Y is fixed (height) and X varies with the bar
    G4int nBarsPlane1 = 60;

    G4double planeX1 = (barX1 + 2*fcoatingThickness)*nBarsPlane1; // dimension of the plane along X direction: 912 mm
    G4double planeZ1 = barZ1;

    // declaring the locations in which we will place the bars and coatings
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

        G4String sipmName1 = "physSiPM1_" + std::to_string(i) + "_front";
        G4String sipmName2 = "physSiPM1_" + std::to_string(i) + "_back";


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
        G4VPhysicalVolume* physBar1 = new G4PVPlacement(0,
            G4ThreeVector(0., 0., 0.),  // Centered in coating
            flogicBar1,
            barName,
            logicCoating1_i,  // Parent volume is the coating
            false,
            i,
            fcheckOverlaps);    

        // Place the SiPMs at the front and back of the bar
        G4VPhysicalVolume* physSiPM1_front = new G4PVPlacement(0,
            G4ThreeVector(posX, locY1, -0.5 * barZ1 - 0.5 * sipmZ),  // Half bar length + half SiPM thickness
            flogicSiPM,
            sipmName1,
            flogicWorld,
            false,
            2*i,    // Unique copy number for front SiPM
            fcheckOverlaps);

        G4VPhysicalVolume* physSiPM1_back = new G4PVPlacement(0,
            G4ThreeVector(posX, locY1, 0.5 * barZ1 + 0.5 * sipmZ),   // Half bar length + half SiPM thickness
            flogicSiPM,
            sipmName2,
            flogicWorld,
            false,
            2*i+1,  // Unique copy number for back SiPM
            fcheckOverlaps);
        
        if (fMode == SimulationMode::INTERNAL || fMode == SimulationMode::BOTH) {
            // Define border surface between coating and scintillator (inside coating)
            new G4LogicalBorderSurface("ScintCoatingSurface_" + std::to_string(i),
                physBar1,       // inside volume
                physCoating1,   // outside volume
                fScintCoatingSurface);

        }

        }
}

// placed perpendicular with respect to the first layer
void QDDetectorConstruction::ConstructScintillatorLayer2(G4LogicalVolume* motherVolume) {

    G4double barX2 = 1350.0 * mm; // for the second layer
    G4double barY2 = 15.0 * mm;
    G4double barZ2 = 15.0 * mm;

    G4double sipmX = 3.0 * mm;  
    G4double sipmY = 15.0 * mm;  
    G4double sipmZ = 15.0 * mm;  

    // bar with coating for the second layer
    G4Box *solidBar2 = new G4Box("solidBar2", 0.5 * barX2, 0.5 * barY2, 0.5 * barZ2);
    flogicBar2 = new G4LogicalVolume(solidBar2, fscintillatorMat, "logicBar2");

    G4Box *solidCoating2 = new G4Box("solidCoating2", 0.5 * barX2, 0.5 * (barY2 + 2 * fcoatingThickness), 0.5 * (barZ2 + 2 * fcoatingThickness));
    G4LogicalVolume *logicCoating2 = new G4LogicalVolume(solidCoating2, fcoatingMat, "logicCoating2");

    G4Box* solidSiPM = new G4Box("solidSiPM", 0.5 * sipmX, 0.5 * sipmY, 0.5 * sipmZ);
    flogicSiPM = new G4LogicalVolume(solidSiPM, fsipmMat, "logicSiPM");
    flogicSiPM->SetVisAttributes(fsipmVisAtt);

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

        G4String sipmName1 = "physSiPM2_" + std::to_string(i) + "_front";
        G4String sipmName2 = "physSiPM2_" + std::to_string(i) + "_back";
            
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
        G4VPhysicalVolume* physBar2 = new G4PVPlacement(0, 
            G4ThreeVector(0., 0., 0.),  // Centered in coating
            flogicBar2, 
            barName, 
            logicCoating2_i, // Parent volume is the coating
            false, 
            i, 
            fcheckOverlaps);

        // Place the SiPMs at the front and back of the bar
        G4VPhysicalVolume* physSiPM2_front = new G4PVPlacement(0,
            G4ThreeVector(- 0.5 * planeX2 - 0.5 * sipmX , locY2, posZ),  // Half bar length + half SiPM thickness
            flogicSiPM,
            sipmName1,
            flogicWorld,
            false,
            2*i,    // Unique copy number for front SiPM
            fcheckOverlaps);

        G4VPhysicalVolume* physSiPM2_back = new G4PVPlacement(0,
            G4ThreeVector( 0.5 * planeX2 + 0.5 * sipmX, locY2, posZ),   // Half bar length + half SiPM thickness
            flogicSiPM,
            sipmName2,
            flogicWorld,
            false,
            2*i+1,  // Unique copy number for back SiPM
            fcheckOverlaps);


        if (fMode == SimulationMode::INTERNAL || fMode == SimulationMode::BOTH) {
            // Define border surface between coating and scintillator (inside coating)
            new G4LogicalBorderSurface("ScintCoatingSurface2_" + std::to_string(i),
                physBar2,       // inside volume
                physCoating2,   // outside volume
                fScintCoatingSurface);
        }
        }

}

void QDDetectorConstruction::ConstructQPU(G4LogicalVolume* motherVolume){

    G4double qpuX = 2.0 * mm; 
    G4double qpuY = 22.0 * mm;
    G4double qpuZ = 22.0 * mm;

    G4Box *solidQPU = new G4Box("solidQPU", 0.5 * qpuX, 0.5 * qpuY, 0.5 * qpuZ);

    flogicQPU = new G4LogicalVolume(solidQPU, fqpuMat, "logicQPU");

    G4VPhysicalVolume* physQPU = new G4PVPlacement(0, 
            G4ThreeVector(0., 0.5*m, 0.),
            flogicQPU,
            "physQPU",
            flogicWorld,  // Parent volume is the world
            false,
            0,
            fcheckOverlaps);
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

    fsipmVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.8)); //translucent green

    fqpuVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.8)); //translucent white

    // First layer - vertical bars (along Z)
    ConstructScintillatorLayer1(flogicWorld);
    
    // Second layer - horizontal bars (along X)
    ConstructScintillatorLayer2(flogicWorld);

    ConstructQPU(flogicWorld);

    flogicBar1->SetVisAttributes(fscintillatorVisAtt);
    flogicBar2->SetVisAttributes(fscintillatorVisAtt);
    flogicQPU ->SetVisAttributes(fqpuVisAtt);
    

    if (!physWorld) {
        G4Exception("QDDetectorConstruction::Construct()",
                   "MyCode1",
                   FatalException,
                   "World volume is null");
    }

    return physWorld;
}

G4VPhysicalVolume *QDDetectorConstruction::Construct(){ // we are defining here our main function Construct
    
    G4cout << "\n=== Starting Detector Construction ===" << G4endl;
    G4cout << "Current simulation mode: " 
           << (fMode == SimulationMode::INTERNAL ? "INTERNAL" :
               fMode == SimulationMode::PARAMETERIZED ? "PARAMETERIZED" :
               fMode == SimulationMode::BOTH ? "BOTH" : "NONE") << G4endl;

    // Define materials
    if (fMode == SimulationMode::INTERNAL || fMode == SimulationMode::BOTH) {
        G4cout << "Initializing internal materials with optical properties..." << G4endl;
        DefineScintillatorMaterials();
    } else {
        G4cout << "Using standard materials without optical properties" << G4endl;
        DefineMaterials();
    }
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

    if (!flogicSiPM) {
    G4Exception("QDDetectorConstruction::ConstructSDandField()",
               "MyCode1",
               FatalException,
               "SiPM logical volume not initialized");
    return;
    }

    if (!flogicQPU) {
    G4Exception("QDDetectorConstruction::ConstructSDandField()",
               "MyCode2",
               FatalException,
               "QPU logical volume not initialized");
    return;
    }

    // Create and register the SD
    auto sdManager = G4SDManager::GetSDMpointer();

    fSensitiveQPUDetector = new QDSensitiveQPUDetector("/SD/QPU");
    sdManager -> AddNewDetector(fSensitiveQPUDetector);
    flogicQPU -> SetSensitiveDetector(fSensitiveQPUDetector);

    if (fMode == SimulationMode::INTERNAL || fMode == SimulationMode::BOTH) {

        fSensitiveSiPMDetector = new QDSensitiveSiPMDetector("/SD/SiPM");
        sdManager -> AddNewDetector(fSensitiveSiPMDetector);
        flogicSiPM -> SetSensitiveDetector(fSensitiveSiPMDetector);

    }
    if (fMode == SimulationMode::PARAMETERIZED || fMode == SimulationMode::BOTH) {

        fSensitiveBarDetector = new QDSensitiveBarDetector("/SD/Bar");
        sdManager -> AddNewDetector(fSensitiveBarDetector);

        flogicBar1 -> SetSensitiveDetector(fSensitiveBarDetector);
        flogicBar2 -> SetSensitiveDetector(fSensitiveBarDetector);
       
    }

}


