#include "QDActionInitialization.hh"



QDActionInitialization::QDActionInitialization(){}

QDActionInitialization::~QDActionInitialization(){}

void QDActionInitialization::BuildForMaster() const{


}


void QDActionInitialization::Build() const{

    SetUserAction(new QDPrimaryGenerator());
    SetUserAction(new QDRunAction());
    SetUserAction(new QDEventAction());

}