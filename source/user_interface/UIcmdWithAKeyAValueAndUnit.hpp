/**
   @file    UIcmdWithAKeyAValueAndUnit.hpp
   @author  Goro Yabu
   @date    2020/02/14
**/

#ifndef UIcmdWithAKeyAValueAndUnit_hpp
#define UIcmdWithAKeyAValueAndUnit_hpp

#include <G4UIcommand.hh>

class UIcmdWithAKeyAValueAndUnit : public G4UIcommand
{
public:
    UIcmdWithAKeyAValueAndUnit
    (const char * theCommandPath,G4UImessenger * theMessenger);
    
    virtual G4int DoIt(G4String parameterList);
    G4String GetNewKey(const char* paramString);
    G4double GetNewDoubleValue(const char* paramString);    
    G4double GetNewDoubleRawValue(const char* paramString);    
    G4double GetNewUnitValue(const char* paramString);
    
    G4String ConvertToStringWithBestUnit(G4double val);    
    G4String ConvertToStringWithDefaultUnit(G4double val);

    void SetKeyCandidates(const char* candidateList);
    void SetParameterName(const char * theName,G4bool omittable,
                          G4bool currentAsDefault=false);    
    void SetDefaultValue(G4double defVal);    
    void SetUnitCategory(const char * unitCategory);
    void SetUnitCandidates(const char * candidateList);
    void SetDefaultUnit(const char * defUnit);

    G4String GetValueString(const char* paramString);
    
private:
    G4int paramid_value;
    G4int paramid_unit;
    G4int paramid_key;
};

#endif

