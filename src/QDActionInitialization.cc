#include "QDActionInitialization.hh"


QDActionInitialization::QDActionInitialization(){}

QDActionInitialization::~QDActionInitialization(){}

void QDActionInitialization::BuildForMaster() const{


}


void QDActionInitialization::Build() const{
    QDPrimaryGenerator *generator = new QDPrimaryGenerator();
    SetUserAction(generator);

}