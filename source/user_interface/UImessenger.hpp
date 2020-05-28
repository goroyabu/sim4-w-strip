/** 
    @file    UImessenger.hpp
    @author  Goro Yabu
    @date    2020/02/06
**/

#ifndef UImessenger_hpp
#define UImessenger_hpp

#include <G4UImessenger.hh>
#include <globals.hh>

class DetectorConstruction;
class PrimaryGeneratorAction;
class PhysicsList;
class EventAction;

class G4VUserDetectorConstruction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

//class UIcmdWithAKeyAValueAndUnit;
class UIcmdWithAStringADoubleAndUnit;

class UImessenger : public G4UImessenger
{
    
public:

    UImessenger();
    ~UImessenger();

    virtual void SetNewValue(G4UIcommand * command, G4String newValue) override;
    int SetDetectorConstruction(DetectorConstruction* detector);
    int SetPrimaryGeneratorAction(PrimaryGeneratorAction* generator);
    int SetPhysicsList(PhysicsList* physics);
    int SetEventAction(EventAction* event);
    
private:

    /* UIdirectory */
    G4UIdirectory * dir_home;
    G4UIdirectory * dir_geom;
    G4UIdirectory * dir_prim;
    G4UIdirectory * dir_phys;
    G4UIdirectory * dir_act;

    /* UIcmd */
    UIcmdWithAStringADoubleAndUnit * cmd_geom_length;
    UIcmdWithAStringADoubleAndUnit * cmd_geom_layer;

    G4UIcmdWithoutParameter * cmd_prim_use_gun;
    G4UIcmdWithoutParameter * cmd_prim_use_gps;

    G4UIcmdWithABool * cmd_phys_use_livcomp;
    G4UIcmdWithAString * cmd_phys_compton_model;

    G4UIcmdWithAnInteger * cmd_act_save_min_nhits;
    
    /* User Customized Class */
    DetectorConstruction * detector_construction;
    PrimaryGeneratorAction * primary_generator;
    PhysicsList * physics_list;
    EventAction * event_action;
    
    G4int n_called_command;
    
    //UIcmdWithAKeyAValueAndUnit * cmd_geom_length;
    //UIcmdWithAKeyAValueAndUnit * cmd_geom_layer;
    // G4UIcmdWithAString * word;
    // G4UIcmdWithADoubleAndUnit * detector_thickness;
};

#endif
