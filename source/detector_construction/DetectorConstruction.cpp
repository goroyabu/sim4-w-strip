/**
   @file    DetectorConstruction.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#include "DetectorConstruction.hpp"
#include "SensitiveDetector.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4Element.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4SDManager.hh>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>

#include <G4RunManager.hh>
#include <G4StateManager.hh>

using std::cout;
using std::endl;

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{
    std::cout << "DetectorConstruction::DetectorConstruction()" << std::endl;
    
    map_key2str["worldMate"]        = "AIR";
    map_key2val["worldSize"]        = 10*cm;
    map_key2vec["worldCenter"]      = G4ThreeVector(0,0,0);

    map_key2str["detectorMate"]     = "CdTe";
    map_key2val["detectorSize"]     = 32*mm;
    map_key2val["detectorGap"]      = 4*mm;
    map_key2vec["detectorCenter"]   = G4ThreeVector(0,0,0);

    G4NistManager * nist_manager = G4NistManager::Instance();
    if ( !nist_manager->FindOrBuildMaterial("CdTe") ) {
	auto Cd = new G4Element("Cadmium",   "Cd", 48., 112.41*g/mole);
	auto Te = new G4Element("Tellurium", "Te", 52., 127.60*g/mole);
	auto CdTe = new G4Material("CdTe", 5.85*g/cm3, 2);
	CdTe->AddElement(Cd, 1);
	CdTe->AddElement(Te, 1);
    }
    if ( !nist_manager->FindOrBuildMaterial("Si") ) {
	new G4Material("Si", 14., 28.09*g/mole, 2.330*g/cm3);
    }
    
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    cout << "DetectorConstruction::Construct()" << endl;
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    G4NistManager * nist_manager = G4NistManager::Instance();
    G4SDManager   * sd_manager   = G4SDManager::GetSDMpointer();

    /* Definition of World */
    auto size_wld     = this->map_key2val["worldSize"]*0.5;
    auto matestr_wld  = "G4_"+this->map_key2str["worldMate"];
    auto material_wld = nist_manager->FindOrBuildMaterial( matestr_wld );
    auto box_wld      = new G4Box( "BoxWorld", size_wld, size_wld, size_wld );
    auto logical_wld  = new G4LogicalVolume( box_wld, material_wld, "LogicalWorld" );
    auto physical_wld = new G4PVPlacement( 0, G4ThreeVector(0,0,0), logical_wld, "PhysicalWorld", 0, false, 0 ); 
    
    /* Definition of Detector */
    auto size_det     = this->map_key2val["detectorSize"]*0.5;
    auto gap_det      = this->map_key2val["detectorGap"];

    auto ndet = (int)vec_detthic.size();
    if ( ndet > 0 ) cout << "***** Construct Geometry *****" << endl;
    
    for ( int i=0; i<ndet; ++i) {
	auto idstr = std::to_string(i);
	auto thick_det = vec_detthic[i];
	auto matestr_det = vec_detmate[i];
	auto material_det = nist_manager->FindOrBuildMaterial( matestr_det );

	cout << "  DETID     : " << i << std::right << std::setw(5) << endl;
	cout << "  Material  : " << matestr_det << std::setw(10) << endl;
	cout << "  Thickness : " << thick_det << std::setw(5) << " mm " << endl;
	cout << endl;
	
	auto box_det_itr            = new G4Box( "BoxDetector", size_det, size_det, thick_det );
	auto logical_det_itr        = new G4LogicalVolume( box_det_itr, material_det, "LogicalDetector"+idstr );
	new G4PVPlacement( 0, G4ThreeVector( 0, 0, -i*gap_det ), logical_det_itr, "PhysicalDetector"+idstr, logical_wld, false, 0 );
	auto sensitive_detector_itr = new SensitiveDetector( "mySensitiveDetector"+idstr );
	sensitive_detector_itr->SetGridXaxis( 128, -size_det*0.5, size_det*0.5 );
	sensitive_detector_itr->SetGridYaxis( 128, -size_det*0.5, size_det*0.5 );
	sensitive_detector_itr->SetGridZaxis(   1,             0,    thick_det );
	sensitive_detector_itr->SetDetectorID(i);
	sd_manager->AddNewDetector( sensitive_detector_itr );
	this->SetSensitiveDetector( logical_det_itr, sensitive_detector_itr );	
    }    
    
    // cout << "DetectorConstruction::Construct() End" << endl;
    return physical_wld;
}

int DetectorConstruction::SetNumberOf(const G4String& key, G4int number)
{
    if ( this->map_key2int.find(key) == this->map_key2int.end() ){
	G4cerr << "*Warning* in DetectorConstruction::SetNumberOf : "
	       << "There is no command key named " << key << G4endl;
	return -1;
    }
    this->map_key2int[key] = number;
    
    if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
	G4RunManager::GetRunManager()->ReinitializeGeometry();
    return 0;    
}
int DetectorConstruction::SetLengthOf(const G4String& key, G4double value)
{
    if ( this->map_key2val.find(key) == this->map_key2val.end() ){
	G4cerr << "*Warning* in DetectorConstruction::SetLengthOf : "
	       << "There is no command key named " << key << G4endl;
	return -1;
    }
    this->map_key2val[key] = value;
    
    if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
	G4RunManager::GetRunManager()->ReinitializeGeometry();
    return 0;
}

int DetectorConstruction::AddDetectorLayers(const G4String& material, G4double thickness)
{
    vec_detmate.emplace_back(material);
    vec_detthic.emplace_back(thickness);
    
    return 0;
}
