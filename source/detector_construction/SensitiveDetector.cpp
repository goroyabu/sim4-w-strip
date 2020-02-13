/**
   @file    SensitiveDetector.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "SensitiveDetector.hpp"
#include "AnalysisManager.hpp"

#include <iostream>

#include <G4ParticleDefinition.hh>
#include <G4UnitsTable.hh>

#include <G4Track.hh>
#include <G4Step.hh>
#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>

#include <G4SDManager.hh>

SensitiveDetector::SensitiveDetector( G4String name )
    : G4VSensitiveDetector(name),
      hits_collection(nullptr)
{
    std::cout << "SensitiveDetector::SensitiveDetector()" << std::endl;
}

SensitiveDetector::~SensitiveDetector()
{
}

void SensitiveDetector::Initialize( G4HCofThisEvent* hce )
{
    std::cout << "SensitiveDetector::Initialize()" << std::endl;

    this->hits_collection = new HitsCollection("detector", "collection");
    auto index_of_hits_collection
	= G4SDManager::GetSDMpointer()->GetCollectionID( "collection" );
    hce->AddHitsCollection( index_of_hits_collection, this->hits_collection );
}

void SensitiveDetector::EndOfEvent( G4HCofThisEvent* )
{
    std::cout << "SensitiveDetector::EndOfEvent()" << std::endl;
    auto analysis_manager = AnalysisManager::Instance();
    auto nhits = hits_collection->entries();

    //analysis_manager->FillNtupleIColumn(0, nhits);
    analysis_manager->FillNtupleIColumnName("nhits", nhits);
    
    for ( G4int i=0; i<nhits; i++ ) {
	(*hits_collection)[i]->Print();
	analysis_manager
	    ->FillNtupleDColumnVName( "edep", (*hits_collection)[i]->GetEdep() );
	analysis_manager->FillH1( 0, (*hits_collection)[i]->GetEdep() );
    }
}

G4bool SensitiveDetector::ProcessHits( G4Step* step, G4TouchableHistory* )
{
    std::cout << "SensitiveDetector::ProcessHits()" << std::endl;
    G4StepPoint* pre = step->GetPreStepPoint();
    
    G4double edep = step->GetTotalEnergyDeposit();
    G4double length = step->GetStepLength();
    //std::cout << "  - TotalEnergyDeposit=" << edep << G4BestUnit(edep,"Energy") << std::endl;

    G4ThreeVector pos = pre->GetPosition();
    // std::cout << "  - Position=(" << pos.x() << G4BestUnit(edep,"Length") << ",";
    // std::cout << pos.y() << G4BestUnit(edep,"Length") << ",";
    // std::cout << pos.z() << G4BestUnit(edep,"Length") << ")" << std::endl;    

    auto touchable = step->GetPreStepPoint()->GetTouchable();
    auto cell_id   = touchable->GetReplicaNumber(1);
    //std::cout << "CellID=" << cell_id << std::endl;

    if ( edep == 0.0 || length == 0.0 ) return true;
    auto hit = new class Hit;
    hit->Add(edep, length);
    if ( ! hit ) {
  	G4ExceptionDescription msg;
  	msg << "Cannot access hit " << cell_id;
  	G4Exception("B4cCalorimeterSD::ProcessHits()",
  		    "MyCode0004", FatalException, msg);
    }
    hits_collection->insert( hit );
    
    return true;
}






