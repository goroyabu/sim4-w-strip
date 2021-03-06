/**
   @file    SensitiveDetector.cpp
   @author  Goro Yabu
   @date    2020/01/30
**/

#include "SensitiveDetector.hpp"
#include "AnalysisManager.hpp"

#include <iostream>
#include <vector>

#include <G4ParticleDefinition.hh>
#include <G4UnitsTable.hh>

#include <G4Track.hh>
#include <G4Step.hh>
#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>

#include <G4SDManager.hh>

#include <TROOT.h>
#include <TString.h>
#include <TH1D.h>

using std::cout;
using std::endl;

SensitiveDetector::SensitiveDetector( G4String name )
    : G4VSensitiveDetector(name),
      hits_collection(nullptr),
      detector_id(0),
      grid_xaxis(nullptr),
      grid_yaxis(nullptr),
      grid_zaxis(nullptr)
{
    grid_xaxis_name = GetUniqueObjectName("grid_xaxis");
    grid_yaxis_name = GetUniqueObjectName("grid_yaxis");
    grid_zaxis_name = GetUniqueObjectName("grid_zaxis");    
    
    grid_xaxis = new TH1D( grid_xaxis_name, ";X", 100, -5, 5);
    grid_yaxis = new TH1D( grid_yaxis_name, ";Y", 100, -5, 5);
    grid_zaxis = new TH1D( grid_zaxis_name, ";Z",   1, -5, 5);

    // std::cout << "SensitiveDetector::SensitiveDetector()" << std::endl;
}

SensitiveDetector::~SensitiveDetector()
{
    DeleteObject( grid_xaxis );
    DeleteObject( grid_yaxis );
    DeleteObject( grid_zaxis );
    // cout << "SensitiveDetector::~SensitiveDetector()" << endl;
}

void SensitiveDetector::Initialize( G4HCofThisEvent* hce )
{
    // std::cout << "SensitiveDetector::Initialize()" << std::endl;
    
    //this->hits_collection = new HitsCollection("detector", "collection");
    this->hits_collection = new HitsCollection( GetName(), GetName()+"_hc" );
    auto index_of_hits_collection
	= G4SDManager::GetSDMpointer()->GetCollectionID( GetName()+"_hc" );
	//= G4SDManager::GetSDMpointer()->GetCollectionID( "collection" );
    hce->AddHitsCollection( index_of_hits_collection, this->hits_collection );
}

void SensitiveDetector::EndOfEvent( G4HCofThisEvent* )
{
    // std::cout << "SensitiveDetector::EndOfEvent()" << std::endl;
    
    auto analysis_manager = AnalysisManager::Instance();
        
    std::vector<DsdHit*> merged_hits;
    auto nhits = hits_collection->entries();
    auto hits  = hits_collection->GetVector();

    for ( int i=0; i<nhits; ++i ) {
       	
	if ( i == 0 ) {
	    merged_hits.emplace_back( (*hits)[i] );
	    continue;
	}
	
	auto current  = (*hits)[i];
	int mode = 1;

	int nhits_merged = (int)merged_hits.size();
	for ( int j=0; j<nhits_merged; ++j ) {
	    if ( mode == 0 ) {
		if ( merged_hits[j]->IsSamePixel( *current ) ) {
		    merged_hits[j]->MergeSamePixel( *current );
		    j = nhits_merged++;		    
		}
	    }
	    else {
		if ( merged_hits[j]->IsAdjacentPixel( *current ) ) {
		    merged_hits[j]->MergeAdjacentPixel( *current );
		    j = nhits_merged++;		    
		}
	    }		
	}
	if ( nhits_merged == (int)merged_hits.size() )
	    merged_hits.emplace_back( current );	

    }

    //analysis_manager->FillNtupleIColumnName( "nhits", (int)merged_hits.size() );
    nhits = (int)merged_hits.size();
    analysis_manager->AddNtupleIColumnName( "nhits", nhits );
    if ( nhits>0 )
	analysis_manager->AddNtupleIColumnName( "hit_pattern", (int)pow(10, detector_id) );
    
    for ( auto hit : merged_hits ) {	
	analysis_manager->FillNtupleDColumnVName( "edep", hit->edep );
	analysis_manager->FillNtupleIColumnVName( "detid",    hit->detid );
	analysis_manager->FillNtupleIColumnVName( "strip_x",  hit->strip_x   );
	analysis_manager->FillNtupleIColumnVName( "strip_y",  hit->strip_y   );
	analysis_manager->FillNtupleDColumnVName( "pos_x",  hit->pos.x() );
	analysis_manager->FillNtupleDColumnVName( "pos_y",  hit->pos.y() );
	analysis_manager->FillNtupleDColumnVName( "pos_z",  hit->pos.z() );
	analysis_manager->AddNtupleDColumnName( "etotal", hit->edep );
	analysis_manager->AddNtupleSColumnName( "proc_name", hit->proc_name );

	auto [ x, y ] = GetCenterOfPixel( hit->strip_x, hit->strip_y );
	analysis_manager->FillNtupleDColumnVName( "pixel_center_x", x );
	analysis_manager->FillNtupleDColumnVName( "pixel_center_y", y );
    }
    //if ( (int)merged_hits.size()!=0 )
    //analysis_manager->FillNtupleDColumnName( "etotal", merged_hits[0]->total_energy );
    // analysis_manager->FillH1( 0, total_energy );
}

G4bool SensitiveDetector::ProcessHits( G4Step* step, G4TouchableHistory* )
{
    // std::cout << "SensitiveDetector::ProcessHits()" << std::endl;

    // cout << "detid=" << GetDetectorID() << endl;
    // auto analysis_manager = AnalysisManager::Instance();
    
    G4StepPoint* pre = step->GetPreStepPoint();
    G4StepPoint* post = step->GetPostStepPoint();

    // auto process = pre->GetProcessDefinedStep();
    auto process = post->GetProcessDefinedStep();
    G4String process_name = "";
    if ( process!=nullptr ) {
	process_name = process->GetProcessName();
	// if ( process_name!="msc" && process_name!="Transportation" )
	//     cout << process_name << endl;
    }
    
    G4double edep = step->GetTotalEnergyDeposit();
    G4double length = step->GetStepLength();

    G4ThreeVector pos = pre->GetPosition();

    // auto touchable = step->GetPreStepPoint()->GetTouchable();
    // auto cell_id   = touchable->GetReplicaNumber(1);

    if ( edep <= 0.0 || length <= 0.0 ) return true;

    auto hit = new DsdHit;
    auto [ strip_x , strip_y ] = GetStripID( pos );
    hit->SetDetectorID( GetDetectorID() )
	->SetProcessName( process_name )
	->SetStripID( strip_x, strip_y )
	->SetPosition( pos/CLHEP::mm )
	->SetEnergy( edep/CLHEP::keV );
    
    hits_collection->insert( hit );
    (*hits_collection)[0]->AddTotalEnergy( edep/CLHEP::keV );

    //hit->Add(edep, length);
    // if ( ! hit ) {
    // 	G4ExceptionDescription msg;
    // 	msg << "Cannot access hit " << cell_id;
    // 	G4Exception("B4cCalorimeterSD::ProcessHits()",
    // 		    "MyCode0004", FatalException, msg);
    // }

    return true;
}

G4int SensitiveDetector::GetDetectorID()
{
    return detector_id;
}
G4int SensitiveDetector::SetDetectorID( G4int id )
{
    detector_id = id; return 0;
}

std::pair<G4int, G4int> SensitiveDetector::GetStripID( const G4ThreeVector& pos )
{
    auto x = grid_xaxis->FindBin( pos.x() );
    auto y = grid_yaxis->FindBin( pos.y() );
    return std::make_pair(x, y);
}
G4int SensitiveDetector::SetGridXaxis( G4int nbins, G4double min, G4double max )
{
    if ( !grid_xaxis ) return -1;
    grid_xaxis->SetBins( nbins, min, max );    
    return 0;
}
G4int SensitiveDetector::SetGridYaxis( G4int nbins, G4double min, G4double max )
{
    if ( !grid_yaxis ) return -1;
    grid_yaxis->SetBins( nbins, min, max );	
    return 0;
}
G4int SensitiveDetector::SetGridZaxis( G4int nbins, G4double min, G4double max )
{
    if ( !grid_zaxis ) return -1;
    grid_zaxis->SetBins( nbins, min, max );	
    return 0;
}
std::pair<G4double, G4double> SensitiveDetector::GetCenterOfPixel( G4int strip_x, G4int strip_y )
{
    auto x = grid_xaxis->GetBinCenter( strip_x );
    auto y = grid_xaxis->GetBinCenter( strip_y );
    return std::make_pair(x, y);
}


TString SensitiveDetector::GetUniqueObjectName(const TString& name)
{
    TObject* obj = gROOT->FindObject(name);
    if ( !obj ) return name;

    TString newname; int i = 1;
    while ( i<100 ) {
	newname = name+Form( "_%d", i);
	TObject* tmp = gROOT->FindObject(newname);
	if ( !tmp ) return newname;
	++i;
    }
    
    return "";
}
void SensitiveDetector::DeleteObject(TObject* obj)
{
    gROOT->Remove(obj);
    if ( !!obj ) obj->Delete();
}

