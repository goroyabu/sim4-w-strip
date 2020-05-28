/**
   @file    main.cpp
   @author  Goro Yabu
   @date    2019/12/22
**/

/** C++ Standard Library **/
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

/** Geant4 Header **/
#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <Randomize.hh>

/** User's customized files **/
#include "DetectorConstruction.hpp"
#include "PhysicsList.hpp"
#include "ActionInitialization.hpp"
#include "UImessenger.hpp"

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    /* Setting of Random */
    G4Random::setTheEngine( new CLHEP::RanecuEngine );
    G4Random::setTheSeed  ( time(NULL) );
    /* Random */

    /* Setting of Run Manager */
    auto run_manager  = new G4RunManager;
    
    auto detector = new DetectorConstruction;
    run_manager->SetUserInitialization( detector );
    
    auto physics  = new PhysicsList;
    run_manager->SetUserInitialization( physics  );

    auto action   = new ActionInitialization;       
    run_manager->SetUserInitialization( action   );

    run_manager->Initialize();
    /* Run Manager */

    
    /* Setting of UI Messenger */
    auto ui_messenger = new UImessenger;

    ui_messenger->SetDetectorConstruction( detector );

    auto primary_generator = action->GetPrimaryGeneratorAction();
    ui_messenger->SetPrimaryGeneratorAction( primary_generator );

    auto event_action = action->GetEventAction();
    ui_messenger->SetEventAction( event_action );
    
    ui_messenger->SetPhysicsList( physics );
    /* UI Messenger */
    
    
    auto ui_executive = new G4UIExecutive(argc,argv);
    auto ui_manager   = G4UImanager::GetUIpointer();
    
    auto vis_executive = new G4VisExecutive;
    vis_executive->Initialize();
    
    if ( argc<=1 ) {
        ui_executive->SessionStart();
    }
    else {
	G4String command = "/control/execute ";
    	G4String fileName = argv[1];
    	ui_manager->ApplyCommand(command+fileName);
    }

    delete run_manager;
    delete ui_executive;
    delete vis_executive;
    delete ui_messenger;
    
    return 0;    
}
