//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "QDPrimaryGeneratorMessenger.hh"
#include "QDPrimaryGeneratorCRY.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

QDPrimaryGeneratorMessenger::QDPrimaryGeneratorMessenger(
                                          QDPrimaryGeneratorCRY* cryGen)
  :fCryAction(cryGen)
{
  fCryDir = new G4UIdirectory("/CRY/");
  fCryDir -> SetGuidance("CRY initialization");
   
  fFileCmd = new G4UIcmdWithAString("/CRY/file",this);
  fFileCmd -> SetGuidance("This reads the CRY definition from a file");
  fFileCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);
   
  fInputCmd = new G4UIcmdWithAString("/CRY/input",this);
  fInputCmd -> SetGuidance("CRY input lines");
  fInputCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  fUpdateCmd = new G4UIcmdWithoutParameter("/CRY/update",this);
  fUpdateCmd -> SetGuidance("Update CRY definition.");
  fUpdateCmd -> SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd -> SetGuidance("if you changed the CRY definition.");
  fUpdateCmd -> AvailableForStates(G4State_Idle);

  fMessInput = new std::string;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

QDPrimaryGeneratorMessenger::~QDPrimaryGeneratorMessenger()
{
  delete fCryDir;
  delete fInputCmd;
  delete fUpdateCmd;
  delete fFileCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void QDPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == fInputCmd )
   { 
     fCryAction -> InputCRY();
     (*fMessInput).append(newValue);
     (*fMessInput).append(" ");
   }

  if( command == fUpdateCmd )
   { 
     fCryAction -> UpdateCRY(fMessInput); 
     *fMessInput = "";
   }

  if( command == fFileCmd )
   { 
    fCryAction -> CRYFromFile(newValue); 
   }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

