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

class UImessenger : public G4UImessenger
{
    
public:

    UImessenger();
    ~UImessenger();

    virtual void SetNewValue(G4UIcommand * command, G4String newValue) override;
    int SetDetectorConstruction(DetectorConstruction* detector);
    
private:
    
    G4UIdirectory * homedir;
    G4UIcmdWithAString * word;
    G4UIcmdWithADoubleAndUnit * detector_thickness;
    
    DetectorConstruction * detector_construction;

};

#endif
