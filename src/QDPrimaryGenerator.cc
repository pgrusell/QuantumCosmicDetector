#include "QDPrimaryGenerator.hh"

QDPrimaryGenerator::QDPrimaryGenerator()
: fParticleGun(nullptr), cryGenerator(nullptr), fGeneratorMode("CRY")
{
    // Initialize particle gun
    fParticleGun = new G4ParticleGun(1);

    // Default particle gun setup
    G4ThreeVector pos(0.*m, 0.*m, 0.*m);
    G4ThreeVector mom(0., -1., 0.);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("mu-");

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(4.*GeV);
    fParticleGun->SetParticleDefinition(particle);

    // Initialize CRY generator
    cryGenerator = new QDPrimaryGeneratorCRY();
}

QDPrimaryGenerator::~QDPrimaryGenerator()
{
    delete fParticleGun;
    delete cryGenerator;
}

void QDPrimaryGenerator::InitializeCRY()
{
    if(cryGenerator) {
        cryGenerator->Initialize();
    }
}

void QDPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    if(fGeneratorMode == "CRY") {
        // Initialize CRY if not already initialized
        if(!cryGenerator->IsInitialized()) {
            cryGenerator->Initialize();
        }
        cryGenerator->GeneratePrimaries(anEvent);
    }
    else { // Default particle gun mode
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}