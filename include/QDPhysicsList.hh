#ifndef QDPHYSICSLISTS_HH
#define QDPHYSICSLISTS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"

class QDPhysicsList : public G4VModularPhysicsList{

    public: 
    QDPhysicsList();
    ~QDPhysicsList();
};


#endif