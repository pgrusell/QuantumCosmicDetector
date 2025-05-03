#ifndef QDPRIMARYGENERATOR_HH
#define QDPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalConstants.hh"

#include "QDGeneratorBase.hh"
#include "QDPrimaryGeneratorCRY.hh"

class G4Event;

class QDPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
    public:

        QDPrimaryGenerator();
        virtual ~QDPrimaryGenerator();

        virtual void GeneratePrimaries(G4Event* event);

        void SetGeneratorMode(G4String mode) { fGeneratorMode = mode; }

        void InitializeCRY(); // Add this method

    private:

        QDPrimaryGeneratorCRY* cryGenerator;
        G4ParticleGun *fParticleGun;
        G4String fGeneratorMode;  // "particleGun" or "CRY"
};


#endif