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



class QDDetectorConstruction : public G4VUserDetectorConstruction{

    public: 
        QDDetectorConstruction();
        virtual ~QDDetectorConstruction(); // virtual function cause its already defined in the parent class 

        virtual G4VPhysicalVolume *Construct();


    private:
        
        G4LogicalVolume *logicBar1;  
        G4LogicalVolume *logicBar2; 

        virtual void ConstructSDandField();


};


#endif