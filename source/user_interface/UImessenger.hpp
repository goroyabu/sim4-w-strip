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
class G4VUserDetectorConstruction;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class UIcmdWithAKeyAValueAndUnit;

class UIcmdWithAStringADoubleAndUnit;

class UImessenger : public G4UImessenger
{
    
public:

    UImessenger();
    ~UImessenger();

    virtual void SetNewValue(G4UIcommand * command, G4String newValue) override;
    int SetDetectorConstruction(DetectorConstruction* detector);
    
private:
    
    G4UIdirectory * dir_home;

    G4UIdirectory              * dir_geom;
    UIcmdWithAKeyAValueAndUnit * cmd_geom_length;
    //UIcmdWithAKeyAValueAndUnit * cmd_geom_layer;
    UIcmdWithAStringADoubleAndUnit * cmd_geom_layer;
    
    DetectorConstruction * detector_construction;

    // G4UIcmdWithAString * word;
    // G4UIcmdWithADoubleAndUnit * detector_thickness;

    G4int n_called_command;
    
};

#endif
