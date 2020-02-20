/**
   @file    UIcmdWithAKeyAValueAndUnit.cpp
   @author  Goro Yabu
   @date    2020/02/14
**/

#include "UIcmdWithAKeyAValueAndUnit.hpp"

#include <G4Tokenizer.hh>
#include <G4UnitsTable.hh>
#include <G4UIcommandStatus.hh>

#include <sstream>
#include <vector>

UIcmdWithAKeyAValueAndUnit::UIcmdWithAKeyAValueAndUnit
(const char * theCommandPath,G4UImessenger * theMessenger)
    :G4UIcommand(theCommandPath,theMessenger)
{
    this->paramid_key         =  GetParameterEntries();
    G4UIparameter * keyParam  =  new G4UIparameter('s');
    SetParameter(keyParam);
    keyParam->SetParameterName("Key");

    this->paramid_value       =  GetParameterEntries();
    G4UIparameter * dblParam  =  new G4UIparameter('d');
    SetParameter(dblParam);

    this->paramid_unit        =  GetParameterEntries();
    G4UIparameter * untParam  =  new G4UIparameter('s');
    SetParameter(untParam);
    untParam->SetParameterName("Unit");
}

G4int UIcmdWithAKeyAValueAndUnit::DoIt(G4String parameterList)
{
    using std::cout;
    using std::endl;
    
    std::vector<G4String> token_vector;
    G4Tokenizer tkn(parameterList);
    G4String str;
    
    while( (str = tkn()) != "" ) {
	token_vector.push_back(str);
    }

    // cout << "key=" << token_vector[this->paramid_key] << endl;
    // cout << "value=" << token_vector[this->paramid_value] << endl;
    // cout << "unit=" << token_vector[this->paramid_unit]<< endl;
    
    // convert a value in default unit
    G4String converted_parameter = token_vector[ this->paramid_key ]+" ";
    G4String default_unit = GetParameter( this->paramid_unit )-> GetDefaultValue();
    
    if ( default_unit != "" && (int)token_vector.size() >= this->GetParameterEntries() ){
	if ( CategoryOf(token_vector[ this->paramid_unit ])!=CategoryOf(default_unit) )
	    return fParameterOutOfCandidates+1;
	G4double value_given = ValueOf(token_vector[ this->paramid_unit ]);
	G4double value_default = ValueOf(default_unit);
	G4double value
	    = ConvertToDouble(token_vector[ this->paramid_value ])
	    * value_given / value_default;
	// reconstruct parameter list
	converted_parameter += ConvertToString(value);
	converted_parameter += " ";
	converted_parameter += default_unit;
	
	for ( auto i=this->GetParameterEntries(); i< (int)token_vector.size(); i++ ) {
	    converted_parameter += " ";
	    converted_parameter += token_vector[i];
	}
	
    } else {
	converted_parameter = parameterList;
    }
    
    return G4UIcommand::DoIt(converted_parameter);
}

G4String UIcmdWithAKeyAValueAndUnit::GetNewKey(const char* paramString)
{
    std::vector<G4String> token_vector;
    G4Tokenizer tkn(paramString);
    G4String str;
    
    while( (str = tkn()) != "" ) {
	token_vector.push_back(str);
    }

    return token_vector[0];
}

G4double UIcmdWithAKeyAValueAndUnit::GetNewDoubleValue(const char* paramString)
{
    auto str = GetValueString( paramString );
    return ConvertToDimensionedDouble(str);
}

G4double UIcmdWithAKeyAValueAndUnit::GetNewDoubleRawValue(const char* paramString)
{
    auto str = GetValueString( paramString );    
    G4double vl; char unts[30];    
    std::istringstream is(str);
    is >> vl >> unts;    
    return vl;
}

G4double UIcmdWithAKeyAValueAndUnit::GetNewUnitValue(const char* paramString)
{
    auto str = GetValueString( paramString );
    G4double vl; char unts[30];
    std::istringstream is(str);
    is >> vl >> unts;
    G4String unt = unts;    
    return ValueOf(unt);
}

G4String UIcmdWithAKeyAValueAndUnit::ConvertToStringWithBestUnit(G4double val)
{
    G4UIparameter* unitParam = GetParameter( this->paramid_unit );
    G4String canList = unitParam->GetParameterCandidates();
    G4Tokenizer candidateTokenizer(canList);
    G4String aToken = candidateTokenizer();
    std::ostringstream os;
    os << G4BestUnit(val,CategoryOf(aToken));    
    G4String st = os.str();
    return st;
}

G4String UIcmdWithAKeyAValueAndUnit::ConvertToStringWithDefaultUnit(G4double val)
{
    G4UIparameter* unitParam = GetParameter( this->paramid_unit );
    G4String st;
    if(unitParam->IsOmittable())
	{ st = ConvertToString(val,unitParam->GetDefaultValue()); }
    else
	{ st = ConvertToStringWithBestUnit(val); }
    return st;
}

void UIcmdWithAKeyAValueAndUnit::SetKeyCandidates(const char* candidateList)
{
    G4UIparameter * key = GetParameter( this->paramid_key );
    G4String canList = candidateList;
    key->SetParameterCandidates(canList);
}

void UIcmdWithAKeyAValueAndUnit::SetParameterName
(const char * theName,G4bool omittable,G4bool currentAsDefault)
{
    G4UIparameter * theParam = GetParameter( this->paramid_value );
    theParam->SetParameterName(theName);
    theParam->SetOmittable(omittable);
    theParam->SetCurrentAsDefault(currentAsDefault);
}

void UIcmdWithAKeyAValueAndUnit::SetDefaultValue(G4double defVal)
{
    G4UIparameter * theParam = GetParameter( this->paramid_value );
    theParam->SetDefaultValue(defVal);
}

void UIcmdWithAKeyAValueAndUnit::SetUnitCategory(const char * unitCategory)
{
    SetUnitCandidates(UnitsList(unitCategory));
}

void UIcmdWithAKeyAValueAndUnit::SetUnitCandidates(const char * candidateList)
{
    G4UIparameter * untParam = GetParameter( this->paramid_unit );
    G4String canList = candidateList;
    untParam->SetParameterCandidates(canList);
}

void UIcmdWithAKeyAValueAndUnit::SetDefaultUnit(const char * defUnit)
{
    G4UIparameter * untParam = GetParameter( this->paramid_unit );
    untParam->SetOmittable(true);
    if ( defUnit == 0 ) return;   
    untParam->SetDefaultValue(defUnit);
    SetUnitCategory(CategoryOf(defUnit));
}

G4String UIcmdWithAKeyAValueAndUnit::GetValueString(const char* paramString)
{
    std::vector<G4String> token_vector;
    G4Tokenizer tkn(paramString);
    G4String str;
    
    while( (str = tkn()) != "" ) {
	token_vector.push_back(str);
    }
    
    auto value_str = token_vector[ this->paramid_value ];
    auto unit_str  = token_vector[ this->paramid_unit  ];
    return value_str + " " + unit_str;
}
