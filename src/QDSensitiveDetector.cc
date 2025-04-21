#include "QDSensitiveDetector.hh"



QDSensitiveDetector::QDSensitiveDetector(G4String name) : G4VSensitiveDetector(name) {

    fTotalEnergyDeposited = 0;

}

QDSensitiveDetector::~QDSensitiveDetector(){

}


void QDSensitiveDetector::Initialize(G4HCofThisEvent *){

    fTotalEnergyDeposited = 0;

}


void QDSensitiveDetector::EndOfEvent(G4HCofThisEvent *){

    G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;

}

G4bool QDSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory * ){

    G4double fEnergyDeposited = aStep -> GetTotalEnergyDeposit();

    if(fEnergyDeposited > 0){

        fTotalEnergyDeposited += fEnergyDeposited;

        G4cout << "Energy deposited in this step: " << fEnergyDeposited << " GeV" << G4endl;
    }

    return true;
}