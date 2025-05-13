#include "QDRunAction.hh"
#include "G4AnalysisManager.hh"

QDRunAction::QDRunAction() : G4UserRunAction() {}

QDRunAction::~QDRunAction() {}

void QDRunAction::BeginOfRunAction(const G4Run*) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv"); // usa CSV
  analysisManager->SetVerboseLevel(1);
  analysisManager->OpenFile("output/salida");
}

void QDRunAction::EndOfRunAction(const G4Run*) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}