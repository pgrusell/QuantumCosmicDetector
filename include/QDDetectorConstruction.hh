#ifndef QDDETECTORCONSTRUCTION
#define QDDETECTORCONSTRUCTION

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "QDSensitiveDetector.hh"
#include "QDDetectorMessenger.hh"


enum class SimulationMode { PARAMETERIZED, INTERNAL, BOTH };

class QDDetectorConstruction : public G4VUserDetectorConstruction{

    public: 
        QDDetectorConstruction();
        virtual ~QDDetectorConstruction(); // virtual function cause its already defined in the parent class 

        virtual G4VPhysicalVolume *Construct() override;
        virtual void ConstructSDandField() override;

        void SetSimulationMode(SimulationMode mode);
        SimulationMode GetSimulationMode() const;


    private:

        void DefineMaterials();
        G4VPhysicalVolume* DefineVolumes();
        G4Material* DefineScintillatorMaterial();

        void ConstructScintillatorLayer1(G4LogicalVolume* motherVolume);
        void ConstructScintillatorLayer2(G4LogicalVolume* motherVolume);


        SimulationMode fMode = SimulationMode::INTERNAL;

        G4Material* fworldMat = nullptr;  // pointer to the target  material
        G4Material* fscintillatorMat = nullptr;  // pointer to the chamber material
        G4Material* fcoatingMat = nullptr;

        G4LogicalVolume *flogicWorld = nullptr;  // pointer to the world logical volume
        G4LogicalVolume *flogicBar1 = nullptr;  
        G4LogicalVolume *flogicBar2 = nullptr; 

        // Visualization attributes
        G4VisAttributes* fcoatingVisAtt = nullptr;
        G4VisAttributes* fscintillatorVisAtt = nullptr;

        G4double fcoatingThickness = 0.1 * mm; // Coating thickness
        G4bool fcheckOverlaps = true;  // option to activate checking of volumes overlaps

        


};


#endif