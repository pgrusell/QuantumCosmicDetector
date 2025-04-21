#ifndef QDPRIMARYGENERATOR_HH
#define QDPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"


class QDPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
    public:

        QDPrimaryGenerator();
        ~QDPrimaryGenerator();

        virtual void GeneratePrimaries(G4Event *);

    private:

        G4ParticleGun *fParticleGun;
};


#endif