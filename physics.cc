#include "physics.hh"

MyPhysicsList::MyPhysicsList(){
	
	// includin em interactions
	RegisterPhysics (new G4EmStandardPhysics()); 


};

MyPhysicsList::~MyPhysicsList(){


};


