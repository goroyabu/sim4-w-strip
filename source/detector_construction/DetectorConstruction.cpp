/**
   @file    DetectorConstruction.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

/** User defined classes **/
#include "DetectorConstruction.hpp"
#include "SensitiveDetector.hpp"
//#include "SteppingAction.hpp"

/** Headers for units **/
#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

/** Classes for materials **/
#include <G4NistManager.hh>

/** Classes for solids and volumes **/
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

/** Classes for sensitive detector **/
#include <G4SDManager.hh>

#include <G4RunManager.hh>
#include <G4StateManager.hh>

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      m_logical_world(nullptr),
      m_physical_world(nullptr)
{
    this->layer_thickness = 1*mm;
    std::cout << "DetectorConstruction::DetectorConstruction()" << std::endl;
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager * nist_manager = G4NistManager::Instance();

    /** Definition of the world **/
    auto box_world = new G4Box("BoxWorld", 10*cm, 10*cm, 10*cm);
    G4Material * material_world = nist_manager->FindOrBuildMaterial( "G4_AIR" );    
    m_logical_world
	= new G4LogicalVolume( box_world, material_world, "LogicalWorld" );
    m_physical_world
	= new G4PVPlacement(0, G4ThreeVector(0,0,0), m_logical_world,
			    "PhysicalWorld", 0, false, 0 ); 

    /** Definition of the detector **/
    auto thin_plate
	= new G4Box("ThinPlate", this->layer_thickness, 5*cm, 5*cm);
    G4Material * material_detector = nist_manager->FindOrBuildMaterial( "G4_Si" );
    auto logical_detector
	= new G4LogicalVolume( thin_plate, material_detector, "LogicalDetector" );
    auto physical_detector
	= new G4PVPlacement(0, G4ThreeVector(0,0,0), logical_detector,
			    "PhysicalDetector", m_logical_world, false, 0 );
    
    /** Register the detector as the sensitive detector **/
    auto sensitive_detector = new SensitiveDetector("mySensitiveDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector( sensitive_detector );
    this->SetSensitiveDetector( logical_detector, sensitive_detector );

    /** or **/
    //auto stepping_action = SteppingAction::Instance();
    //stepping_action->SetVolume( logical_detector );
    
    std::cout << "DetectorConstruction::Construct()" << std::endl;
    std::cout << "Thickness of Layer = " << layer_thickness << std::endl;
    std::cout << "ptr= " << this << std::endl;
    return m_physical_world;
}

int DetectorConstruction::SetLayerThickness(double thickness_mm)
{
    this->layer_thickness = thickness_mm;
    std::cout << "set layer thickness = " << this->layer_thickness << std::endl;
    if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
	G4RunManager::GetRunManager()->ReinitializeGeometry();
    }
    std::cout << "ptr=" << this << std::endl;
    return 0;
}
