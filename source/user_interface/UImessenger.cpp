/**
   @file    UImessenger.cpp
   @author  Goro Yabu
   @date 2020/02/06
**/

#include "UImessenger.hpp"
#include "DetectorConstruction.hpp"
#include "UIcmdWithAKeyAValueAndUnit.hpp"

#include "UIcmdWithCustomizableArguments.hpp"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

#include <G4SystemOfUnits.hh>

#include <iostream>

using std::cout;
using std::endl;

UImessenger::UImessenger()
    :G4UImessenger(),
     dir_home(nullptr),
     detector_construction(nullptr)
{
    dir_home = new G4UIdirectory( "/wstrip/" );
    dir_home->SetGuidance( "Commands specific to this app" );

    auto geom_path =  (G4String)dir_home->GetCommandPath()+"geom/" ;
    dir_geom = new G4UIdirectory( geom_path.c_str() );

    auto length_path = (G4String)dir_geom->GetCommandPath()+"setLengthOf";
    cmd_geom_length = new UIcmdWithAKeyAValueAndUnit( length_path.c_str(), this );
    cmd_geom_length->SetGuidance( "Command to Change a Length" );
    cmd_geom_length->SetParameterName( "Length", false, true );
    // cmd_geom_length->SetRange( "Size>0.0" );
    // cmd_geom_length
    // 	->SetKeyCandidates("detectorSize detectorThick detectorGap worldSize");
    cmd_geom_length->SetDefaultUnit( "mm" );
    cmd_geom_length->SetUnitCategory( "Length" );
    cmd_geom_length->AvailableForStates( G4State_PreInit, G4State_Idle );
    cmd_geom_length->SetToBeBroadcasted( false );
    
    auto layers_path = (G4String)dir_geom->GetCommandPath()+"addDetectorLayer";
    //cmd_geom_layer = new UIcmdWithAKeyAValueAndUnit( layers_path.c_str(), this );
    cmd_geom_layer = new UIcmdWithAStringADoubleAndUnit( layers_path.c_str(), this );
    cmd_geom_layer->SetGuidance( "Command to Add a Detector Layer");
    cmd_geom_layer->SetParameterName( "Size", false, true );
    cmd_geom_layer->SetDefaultUnit( "um" );
    cmd_geom_layer->SetDefaultValue( 500*CLHEP::um );
    cmd_geom_layer->SetUnitCategory( "Length" );
    cmd_geom_layer->AvailableForStates( G4State_PreInit, G4State_Idle );
    cmd_geom_layer->SetToBeBroadcasted( false ); 
    
    std::cout << "UImessenger::UImessenger()" << std::endl;
}

UImessenger::~UImessenger()
{
    delete this->dir_home;
}

void UImessenger::SetNewValue(G4UIcommand * command, G4String newValue)
{
    using std::cout;
    using std::endl;
    cout << " UImessenger::SetNewValue" << endl;
    cout << " Command Path = " << command->GetCommandPath() << endl;
    cout << " Input Value  = " << newValue << endl;
    
    if (!detector_construction) {
	cout << " Pointer of DetectorConstruction is not given to UImessenger" << endl;
	return;
    }
    
    if ( command == this->cmd_geom_length ) {
	auto key   = this->cmd_geom_length->GetNewKey( newValue );
	auto value = this->cmd_geom_length->GetNewDoubleValue( newValue );	
	detector_construction->SetLengthOf( key, value );
    }
    else if ( command == this->cmd_geom_layer ) {
	//auto key   = this->cmd_geom_layer->GetNewKey( newValue );
	auto key   = this->cmd_geom_layer->GetNewString( newValue );
	auto value = this->cmd_geom_layer->GetNewDoubleValue( newValue );
	detector_construction->AddDetectorLayers( key, value );
    }
    else {
	cout << "***Error*** There is not such a command named "
	     << command->GetCommandPath() << endl;
    }
    
    cout << " UImessenger::SetNewValue End" << endl;
    
}

int UImessenger::SetDetectorConstruction(DetectorConstruction* detector)
{
    if ( !detector ) {
	std::cout << "***Error*** DetectorConstruction is nullptr" << std::endl;
	return -1;
    }
    detector_construction = new DetectorConstruction();
    detector_construction = detector;
    return 0;
}
