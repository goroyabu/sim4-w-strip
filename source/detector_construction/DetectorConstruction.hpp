/**
   @file    DetectorConstruction.hpp
   @author  Goro Yabu
   @date    2019/12/22
**/

#ifndef DetectorConstruction_hpp
#define DetectorConstruction_hpp

#include <G4VUserDetectorConstruction.hh>
#include <globals.hh>

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    DetectorConstruction();
    ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct() override;

    int SetLayerThickness(double thickness_mm);
    
private:

    G4LogicalVolume * m_logical_world;
    G4VPhysicalVolume * m_physical_world;
    double layer_thickness;
    
};
#endif

