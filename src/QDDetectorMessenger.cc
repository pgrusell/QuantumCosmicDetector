#include "QDDetectorMessenger.hh"
#include "QDDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

QDDetectorMessenger::QDDetectorMessenger(QDDetectorConstruction* det)
: fDetector(det), fDir(nullptr), fSetModeCmd(nullptr) {
    fDir = new G4UIdirectory("/detector/");
    fDir->SetGuidance("Detector construction control commands");

    fSetModeCmd = new G4UIcmdWithAString("/detector/setMode", this);
    fSetModeCmd->SetGuidance("Choose simulation mode: parameterized, internal, both or none");
    fSetModeCmd->SetCandidates("none parameterized internal both");
    fSetModeCmd->SetParameterName("Mode", false);
    fSetModeCmd->SetDefaultValue("internal");

}

QDDetectorMessenger::~QDDetectorMessenger() {
    delete fSetModeCmd;
    delete fDir;
}

void QDDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fSetModeCmd) {
        G4cout << "\n=== Detector Messenger receiving mode change ===" << G4endl;
        G4cout << "Current mode before change: " 
               << (fDetector->GetSimulationMode() == SimulationMode::INTERNAL ? "INTERNAL" :
                   fDetector->GetSimulationMode() == SimulationMode::PARAMETERIZED ? "PARAMETERIZED" :
                   fDetector->GetSimulationMode() == SimulationMode::BOTH ? "BOTH" : "NONE") << G4endl;
        
        G4cout << "Attempting to set mode to: " << newValue << G4endl;
        
        if (newValue == "internal") {
            fDetector->SetSimulationMode(SimulationMode::INTERNAL);
        } else if (newValue == "parameterized") {
            fDetector->SetSimulationMode(SimulationMode::PARAMETERIZED);
        } else if (newValue == "both") {
            fDetector->SetSimulationMode(SimulationMode::BOTH);
        } else if (newValue == "none") {
            fDetector->SetSimulationMode(SimulationMode::NONE);
        }
        
        G4cout << "Mode after change: " 
               << (fDetector->GetSimulationMode() == SimulationMode::INTERNAL ? "INTERNAL" :
                   fDetector->GetSimulationMode() == SimulationMode::PARAMETERIZED ? "PARAMETERIZED" :
                   fDetector->GetSimulationMode() == SimulationMode::BOTH ? "BOTH" : "NONE") << G4endl;
        G4cout << "NOTE: Run /run/initialize to apply changes" << G4endl;
        G4cout << "=======================================\n" << G4endl;
    
    }
}