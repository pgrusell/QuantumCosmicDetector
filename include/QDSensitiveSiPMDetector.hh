#ifndef QDSENSITIVESIPMDETECTOR_HH
#define QDSENSITIVESIPMDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4OpticalPhoton.hh"

class QDSensitiveSiPMDetector : public G4VSensitiveDetector{


    public:
        QDSensitiveSiPMDetector(G4String);
        ~QDSensitiveSiPMDetector();

    private:

        G4double fTotalEnergyDeposited;
        G4int fPhotonCount = 0;
        std::map<G4int, G4int> fSiPMCounts;

        // HC : hits collection only important if you want to do analysis
        // or reconstraction within G4 
        virtual void Initialize(G4HCofThisEvent *) override;
        virtual void EndOfEvent(G4HCofThisEvent *) override;

        // handles what happens to the particle in each step 
        // when it is inside of the detector

        virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;


};




#endif