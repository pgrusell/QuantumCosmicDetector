#include "QDPhysicsList.hh"



QDPhysicsList::QDPhysicsList(){
    // EM Physics 
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4DecayPhysics());



}

QDPhysicsList::~QDPhysicsList(){}