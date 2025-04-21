#ifndef QDACTIONINITIALIZATION_HH
#define QDACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "QDPrimaryGenerator.hh"

class QDActionInitialization : public G4VUserActionInitialization{

    public:
        QDActionInitialization();
        ~QDActionInitialization();

        virtual void BuildForMaster() const;
        virtual void Build() const;


};



#endif