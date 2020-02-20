/**
   @file    UIcmdWithCustomizableArguments.hpp
   @author  Goro Yabu
   @date
**/

#ifndef UIcmdWithCustomizableArguments_hpp
#define UIcmdWithCustomizableArguments_hpp

#include <utility>
#include <vector>

#include <G4UIcommand.hh>

class UIcmdWithCustomizableArguments : public G4UIcommand
{

public:

    UIcmdWithCustomizableArguments
    (const char* theCommandPath, G4UImessenger* theMessenger);

    virtual G4int DoIt(G4String parameterList);
    
    G4String      GetNewString         (const char* paramString);
    G4int         GetNewIntValue       (const char* paramString);
    G4double      GetNewDoubleValue    (const char* paramString);
    G4double      GetNewDoubleRawValue (const char* paramString);
    G4bool        GetNewBoolValue      (const char* paramString);
    G4ThreeVector GetNew3VectorValue   (const char* paramString);
    G4ThreeVector GetNew3VectorRawValue(const char* paramString);
    
    void SetParameterStringName
    (const char *theName, G4bool omittable, G4bool currentAsDefault=false);
    void SetParameterIntName
    (const char *theName, G4bool omittable, G4bool currentAsDefault=false);
    void SetParameterDoubleName
    (const char *theName, G4bool omittable, G4bool currentAsDefault=false);
    void SetParameterBoolName
    (const char *theName, G4bool omittable, G4bool currentAsDefault=false);    
    void SetParameter3VectorName
    (const char *theNameX, const char *theNameY, const char *theNameZ,
     G4bool omittable, G4bool currentAsDefault=false);
    
    void SetDefaultString      (const char *defVal);
    void SetDefaultIntValue    (G4int defVal);
    void SetDefaultDoubleValue (G4double defVal);
    void SetDefaultBoolValue   (G4bool defVal);
    void SetDefault3VectorValue(G4ThreeVector defVal);
    
    void SetUnitCategory  (const char *unitCategory);
    void SetUnitCandidates(const char *candidateList);	
    void SetDefaultUnit   (const char *defUnit);    
    
    
protected:

    const char theIntegerType        = 'i';
    const char theDoubleType         = 'd';
    const char theStringType         = 's';
    const char theBoolType           = 'b';
    
    const char theDoubleAndUnitType  = 'f';
    const char the3VectorType        = 'v';
    const char the3VectorAndUnitType = 'u';
    
    G4int DefineParameter(char theType, const char* theName=nullptr);

    std::pair<G4int, char> DefineAInteger
    (const char* theNameOfValue=nullptr);

    std::pair<G4int, char> DefineADouble
    (const char* theNameOfValue=nullptr);
    
    std::pair<G4int, char> DefineADoubleAndUnit
    (const char* theNameOfValue=nullptr, const char* theNameOfUnit=nullptr);

    std::pair<G4int, char> Define3Vector
    (const char* theNameOfX=nullptr, const char* theNameOfY=nullptr,
     const char* theNameOfZ=nullptr);

    std::pair<G4int, char> Define3VectorAndUnit
    (const char* theNameOfX=nullptr, const char* theNameOfY=nullptr,
     const char* theNameOfZ=nullptr, const char* theNameOfUnit=nullptr);

    std::pair<G4int, char> DefineAString(const char* theNameOfString=nullptr);

    std::pair<G4int, char> DefineABool(const char* theNameOfBool=nullptr);

    G4int         GetIntValue       (G4int dictID, const char* paramString);
    G4double      GetDoubleValue    (G4int dictID, const char* paramString);
    G4double      GetDoubleRawValue (G4int dictID, const char* paramString);
    G4String      GetString         (G4int dictID, const char* paramString);
    G4bool        GetBoolValue      (G4int dictID, const char* paramString);
    G4ThreeVector Get3VectorValue   (G4int dictID, const char* paramString);
    G4ThreeVector Get3VectorRawValue(G4int dictID, const char* paramString);
    G4double      GetUnitValue      (G4int dictID, const char* paramString);
           
    std::vector<G4String> Tokenize(G4String parameterList);
    G4int ConvertToDefaultUnit
    (std::vector<G4String>& tokenVector, G4String* parameterList);

private:

    std::vector< std::pair<G4int, char> > dictParameters;
    void AddNewParameter(std::pair<G4int, char>& p);
    
    G4int defaultStringID;
    G4int defaultIntegerID; 
    G4int defaultDoubleID; 
    G4int defaultBoolID;
    G4int default3VectorID;
    G4int defaultUnitID;
    G4bool defaultDoubleHasUnit;
    G4bool default3VectorHasUnit;
};

class UIcmdWithAStringADoubleAndUnit : public UIcmdWithCustomizableArguments
{
public:    
    UIcmdWithAStringADoubleAndUnit
    (const char* theCommandPath, G4UImessenger* theMessenger)
	: UIcmdWithCustomizableArguments(theCommandPath, theMessenger)
    { DefineAString(); DefineADoubleAndUnit(); }
    
    inline void SetParameterName
    (const char * theName, G4bool omittable, G4bool currentAsDefault)
    { SetParameterDoubleName( theName, omittable, currentAsDefault ); }
    
    inline void SetDefaultValue(G4double defVal)
    { SetDefaultDoubleValue( defVal ); }
};

class UIcmdWithAString3VectorAndUnit : public UIcmdWithCustomizableArguments
{    
public:
    UIcmdWithAString3VectorAndUnit
    (const char* theCommandPath, G4UImessenger* theMessenger)
	: UIcmdWithCustomizableArguments(theCommandPath, theMessenger)
    { DefineAString(); Define3VectorAndUnit(); }

    inline void SetParameterName
    (const char *theNameX, const char *theNameY, const char *theNameZ,
     G4bool omittable, G4bool currentAsDefault=false)
    { SetParameter3VectorName( theNameX, theNameY, theNameZ,
			       omittable, currentAsDefault); }

    inline void SetDefaultValue(G4ThreeVector defVal)
    { SetDefault3VectorValue( defVal ); }    
};

class UIcmdWithAStringAInteger : public UIcmdWithCustomizableArguments
{
public:
    UIcmdWithAStringAInteger
    (const char* theCommandPath, G4UImessenger* theMessenger)
	: UIcmdWithCustomizableArguments(theCommandPath, theMessenger)
    { DefineAString(); DefineAInteger(); }

    inline void SetParameterName
    (const char * theName, G4bool omittable, G4bool currentAsDefault)
    { SetParameterIntName( theName, omittable, currentAsDefault ); }
    
    inline void SetDefaultValue(G4double defVal)
    { SetDefaultIntValue( defVal ); }    
};

#endif
