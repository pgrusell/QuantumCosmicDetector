#ifndef QDGENERATORBASE_HH
#define QDGENERATORBASE_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"

// abstract base class for different generators

class QDGeneratorBase 
{
public:
    QDGeneratorBase() {};
    virtual ~QDGeneratorBase() {};
    
    virtual void GeneratePrimaries(G4Event* event) = 0;
};

#endif