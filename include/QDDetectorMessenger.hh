#ifndef QDDETECTORMESSENGER_HH
#define QDDETECTORMESSENGER_HH

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"
#include "G4UImessenger.hh"


class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcommand;
class QDDetectorConstruction;


class QDDetectorMessenger : public G4UImessenger {

    
    public:
        QDDetectorMessenger(QDDetectorConstruction* det);
        ~QDDetectorMessenger();
    
        void SetNewValue(G4UIcommand* command, G4String newValue);



    private:
        G4UIdirectory* fDir;  
        G4UIcmdWithAString* fSetModeCmd;
        QDDetectorConstruction* fDetector;
};


#endif

