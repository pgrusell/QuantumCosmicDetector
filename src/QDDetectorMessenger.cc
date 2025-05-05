#include "QDDetectorMessenger.hh"
#include "QDDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

QDDetectorMessenger::QDDetectorMessenger(QDDetectorConstruction* det)
: fDetector(det), fDir(nullptr), fSetModeCmd(nullptr) {
    fDir = new G4UIdirectory("/g4sim/");
    fDir->SetGuidance("Simulation control commands");

    fSetModeCmd = new G4UIcmdWithAString("/g4sim/setMode", this);
    fSetModeCmd->SetGuidance("Choose simulation mode: parameterized, internal, or both");
    fSetModeCmd->SetCandidates("parameterized internal both");
    fSetModeCmd->SetDefaultValue("internal");

}

QDDetectorMessenger::~QDDetectorMessenger() {
    delete fSetModeCmd;
    delete fDir;
}

void QDDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fSetModeCmd) {
        if (newValue == "parameterized") {
            fDetector->SetSimulationMode(SimulationMode::PARAMETERIZED);
        } else if (newValue == "internal") {
            fDetector->SetSimulationMode(SimulationMode::INTERNAL);
        } else if (newValue == "both") {
            fDetector->SetSimulationMode(SimulationMode::BOTH);
        } else {
            G4cerr << "Invalid mode: " << newValue << G4endl;
        }
    }
}