/**
   @file    UImessenger.cpp
   @author  Goro Yabu
   @date 2020/02/06
**/

#include "UImessenger.hpp"
#include "DetectorConstruction.hpp"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

#include <iostream>

UImessenger::UImessenger()
    :G4UImessenger(),
     homedir(nullptr),
     detector_construction(nullptr)
{
    this->homedir = new G4UIdirectory( "/myapp/" );
    this->homedir->SetGuidance( "Commands specific to this app" );

    word = new G4UIcmdWithAString( "/myapp/setName", this );
    word->SetGuidance( "nandemo iiyo" );
    word->SetParameterName( "your_choice", true );
    word->AvailableForStates( G4State_PreInit, G4State_Idle );
    word->SetToBeBroadcasted( false );

    detector_thickness
	= new G4UIcmdWithADoubleAndUnit( "/myapp/setThick", this );
    detector_thickness->SetGuidance( "Thickness of layers" );
    detector_thickness->SetParameterName( "Thick", true, true );
    detector_thickness->SetRange( "Thick>0.0" );
    detector_thickness->SetDefaultUnit( "mm" );
    detector_thickness->SetUnitCategory( "Length" );
    detector_thickness->AvailableForStates( G4State_PreInit, G4State_Idle );
    detector_thickness->SetToBeBroadcasted( false );

    // SizeCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setSize",this);
    // fSizeCmd->SetGuidance("Set size of the box");
    // fSizeCmd->SetParameterName("Size",false);
    // fSizeCmd->SetRange("Size>0.");
    // fSizeCmd->SetUnitCategory("Length");
    // fSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    // fSizeCmd->SetToBeBroadcasted(false);
    
    std::cout << "UImessenger::UImessenger()" << std::endl;
}

UImessenger::~UImessenger()
{
    delete this->homedir;
    delete this->word;
    delete this->detector_thickness;
}

void UImessenger::SetNewValue(G4UIcommand * command, G4String newValue)
{
    std::cout << "set... " << newValue << std::endl;
    std::cout << this->detector_thickness->GetCurrentValue() << std::endl;
    if (!detector_construction) {
	std::cout << "nuulllll" << std::endl;
    }
    
    if ( command == this->word ) {
	std::cout << "Param=" << newValue << std::endl;
    }
    else if ( command == this->detector_thickness ) {
	std::cout << "TTTThick=" << newValue << std::endl;
	detector_construction
	    ->SetLayerThickness( detector_thickness->GetNewDoubleValue( newValue ) );
    }
    else {
	std::cout << "there is not such command" << std::endl;
    }
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
