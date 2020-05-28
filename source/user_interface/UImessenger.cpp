/**
   @file    UImessenger.cpp
   @author  Goro Yabu
   @date 2020/02/06
**/

#include "UImessenger.hpp"

/* User Customized Class */
#include "DetectorConstruction.hpp"
#include "PrimaryGeneratorAction.hpp"
#include "PhysicsList.hpp"
#include "PhysicsConstructor.hpp"
#include "EventAction.hpp"

//#include "UIcmdWithAKeyAValueAndUnit.hpp"
#include "UIcmdWithCustomizableArguments.hpp"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithoutParameter.hh>

#include <G4SystemOfUnits.hh>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

UImessenger::UImessenger()
    :G4UImessenger(),
     dir_home(nullptr),
     detector_construction(nullptr),
     primary_generator(nullptr),
     physics_list(nullptr),
     n_called_command(0)
{
    dir_home = new G4UIdirectory( "/wstrip/" );
    dir_home->SetGuidance( "Commands specific to this app" );

    auto geom_path =  (G4String)dir_home->GetCommandPath()+"geom/" ;
    dir_geom = new G4UIdirectory( geom_path.c_str() );

    auto length_path = (G4String)dir_geom->GetCommandPath()+"setLengthOf";
    //cmd_geom_length = new UIcmdWithAKeyAValueAndUnit( length_path.c_str(), this );
    cmd_geom_length = new UIcmdWithAStringADoubleAndUnit( length_path.c_str(), this );
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

    auto prim_path =  (G4String)dir_home->GetCommandPath()+"prim/";
    dir_prim = new G4UIdirectory( prim_path.c_str() );

    auto use_gun_path = (G4String)dir_prim->GetCommandPath()+"useParticleGun";
    cmd_prim_use_gun = new G4UIcmdWithoutParameter( use_gun_path.c_str(), this );
    cmd_prim_use_gun->SetGuidance( "Enable Particle Gun as Primary Generator" );
    cmd_prim_use_gun->SetGuidance( "and disable other types of generator like GPS" );
    cmd_prim_use_gun->AvailableForStates( G4State_PreInit, G4State_Idle );    

    auto use_gps_path = (G4String)dir_prim->GetCommandPath()+"useGPS";
    cmd_prim_use_gps = new G4UIcmdWithoutParameter( use_gps_path.c_str(), this );
    cmd_prim_use_gps->SetGuidance( "Enable GPS as Primary Generator" );
    cmd_prim_use_gps->SetGuidance( "and disable other types of generator like Particle Gun" );
    cmd_prim_use_gps->AvailableForStates( G4State_PreInit, G4State_Idle );    

    
    auto phys_path = (G4String)dir_home->GetCommandPath()+"phys/";
    dir_phys = new G4UIdirectory( phys_path.c_str() );

    auto use_livcomp_path
	= (G4String)dir_phys->GetCommandPath()+"useLivermoreCompton";
    cmd_phys_use_livcomp
	= new G4UIcmdWithABool( use_livcomp_path.c_str(), this);
    cmd_phys_use_livcomp->SetGuidance( "Enable LivermoreComptonModel for ComptonScattering");
    cmd_phys_use_livcomp->AvailableForStates( G4State_PreInit, G4State_Idle );    

    auto comp_model_path = (G4String)dir_phys->GetCommandPath()+"comptonModel";
    cmd_phys_compton_model = new G4UIcmdWithAString( comp_model_path.c_str(), this );
    cmd_phys_compton_model->SetGuidance( "Set ComptonModel" );
    cmd_phys_compton_model->AvailableForStates( G4State_PreInit, G4State_Idle );

    
    auto act_path = (G4String)dir_home->GetCommandPath()+"act/";
    dir_act = new G4UIdirectory( act_path.c_str() );

    auto save_min_nhits_path
	= (G4String)dir_act->GetCommandPath()+"saveMinNhits";
    cmd_act_save_min_nhits
	= new G4UIcmdWithAnInteger( save_min_nhits_path.c_str(), this );
    cmd_act_save_min_nhits->SetGuidance( "Minimum N-hits of events to be saved");
    cmd_act_save_min_nhits->AvailableForStates( G4State_PreInit, G4State_Idle );
    
    std::cout << "UImessenger::UImessenger()" << std::endl;
}

UImessenger::~UImessenger()
{
    delete this->dir_home;
}

void UImessenger::SetNewValue(G4UIcommand * command, G4String newValue)
{
    // cout << " UImessenger::SetNewValue" << endl;

    if ( n_called_command==0 ) {
	cout << "***** Reading .mac File... *****" << endl;
	cout << endl;
    }
    cout <<     "  Command Path[" << n_called_command << "]";
    cout <<     " : " << command->GetCommandPath() << endl;
    cout <<     "  Input Value";
    cout <<     " : " << newValue << endl;
    
    ++n_called_command;        
    
    if (!detector_construction) {
	cout << " Pointer of DetectorConstruction is not given to UImessenger" << endl;
	return;
    }
    
    if ( command == this->cmd_geom_length ) {

	auto key   = this->cmd_geom_length->GetNewString( newValue );
	auto value = this->cmd_geom_length->GetNewDoubleValue( newValue );	
	detector_construction->SetLengthOf( key, value );
	
    }
    else if ( command == this->cmd_geom_layer ) {

	auto key   = this->cmd_geom_layer->GetNewString( newValue );
	auto value = this->cmd_geom_layer->GetNewDoubleValue( newValue );
	detector_construction->AddDetectorLayers( key, value );
	
    }
    else if ( command == this->cmd_prim_use_gun ) {
	
	primary_generator->EnableParticleGun();
	
    }
    else if ( command == this->cmd_prim_use_gps ) {
	
	primary_generator->EnableGPS();
	
    }
    else if ( command == this->cmd_phys_use_livcomp ) {
	
	auto value = this->cmd_phys_use_livcomp->GetNewBoolValue( newValue );
	auto constructor
	    = (PhysicsConstructor*)physics_list->GetPhysics("PhysicsConstructor");
	constructor->SetParameter( "EnableLivermoreCompton", value );

    }
    else if ( command == this->cmd_phys_compton_model ) {

	auto value = newValue;
	auto constructor
	    = (PhysicsConstructor*)physics_list->GetPhysics("PhysicsConstructor");
	constructor->SetParameter( "ComptonModel", value );

    }
    else if ( command == this->cmd_act_save_min_nhits ) {

	auto value = this->cmd_act_save_min_nhits->GetNewIntValue( newValue );
	event_action->SetParameter( "SaveMinNhits", value );
	
    }
    else {
	cout << "***Error*** There is not such a command named "
	     << command->GetCommandPath() << endl;
    }

    cout << endl;
    //cout << " UImessenger::SetNewValue End" << endl;    
}

int UImessenger::SetDetectorConstruction(DetectorConstruction* detector)
{
    if ( !detector ) {
	cout << "***Error*** DetectorConstruction is nullptr" << endl;
	return -1;
    }
    detector_construction = new DetectorConstruction();
    detector_construction = detector;
    return 0;
}
int UImessenger::SetPrimaryGeneratorAction(PrimaryGeneratorAction* generator)
{
    if ( !generator ) {
	cout << "***Error*** PrimaryGeneratorAction is nullptr" << endl;
	return -1;
    }
    primary_generator = new PrimaryGeneratorAction();
    primary_generator = generator;
    return 0;
}
int UImessenger::SetPhysicsList(PhysicsList* physics)
{
    if ( !physics ) {
	cout << "***Error*** PhysicsList is nullptr" << endl;
	return -1;
    }
    physics_list = new PhysicsList();
    physics_list = physics;
    return 0;
}
int UImessenger::SetEventAction(EventAction* event)
{
    if ( !event ) {
	cout << "***Error*** EventAction is nullptr" << endl;
	return -1;
    }
    event_action = new EventAction();
    event_action = event;
    return 0;
}
