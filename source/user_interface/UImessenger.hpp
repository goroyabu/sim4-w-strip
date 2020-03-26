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

class G4VUserDetectorConstruction;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

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
    
private:

    /* UIdirectory */
    G4UIdirectory * dir_home;
    G4UIdirectory * dir_geom;
    G4UIdirectory * dir_prim;

    /* UIcmd */
    UIcmdWithAStringADoubleAndUnit * cmd_geom_length;
    UIcmdWithAStringADoubleAndUnit * cmd_geom_layer;
    G4UIcmdWithoutParameter * cmd_prim_use_gun;
    G4UIcmdWithoutParameter * cmd_prim_use_gps;

    /* User Customized Class */
    DetectorConstruction * detector_construction;
    PrimaryGeneratorAction * primary_generator;
    
    G4int n_called_command;
    
    //UIcmdWithAKeyAValueAndUnit * cmd_geom_length;
    //UIcmdWithAKeyAValueAndUnit * cmd_geom_layer;
    // G4UIcmdWithAString * word;
    // G4UIcmdWithADoubleAndUnit * detector_thickness;
};

#endif
