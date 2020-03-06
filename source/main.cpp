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
    // cout << "main()" << endl;

    // G4String cmd = "/test/doit 1.0 2 gamma";
    // G4int i = cmd.index(" ");
    // if( i != G4int(std::string::npos) ){
    // 	cout << "command = " << cmd(0,i) << endl;
    // 	cout << "parameters = " << cmd(i+1,cmd.length()-(i+1)) << endl;
    // 	cout << "i=" << i << ", length=" << cmd.length() << ", end=" << cmd.length()-(i+1) << endl;
    // }
    
    // auto now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
    // std::stringstream ss;
    // ss << std::put_time( std::localtime(&now), "%Y%m%d_%H%M%S" );
    // cout << ss.str() << endl;
    
    G4Random::setTheEngine( new CLHEP::RanecuEngine );
    G4Random::setTheSeed  ( time(NULL) );
    
    auto run_manager  = new G4RunManager;
    auto ui_messenger = new UImessenger;
    
    auto detector = new DetectorConstruction;
    auto physics  = new PhysicsList;
    auto action   = new ActionInitialization;
    run_manager->SetUserInitialization( detector );
    run_manager->SetUserInitialization( physics  );
    run_manager->SetUserInitialization( action   );
    run_manager->Initialize();
    
    ui_messenger->SetDetectorConstruction( detector );
    
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
