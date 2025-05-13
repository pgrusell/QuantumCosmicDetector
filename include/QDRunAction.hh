#ifndef QDRUNACTION_HH
#define QDRUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class QDRunAction : public G4UserRunAction {
public:
  QDRunAction();
  virtual ~QDRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
};

#endif