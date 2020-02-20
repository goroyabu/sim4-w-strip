/**
   @file    UIcmdWithCustomizableArguments.cpp
   @author  Goro Yabu
**/

#include "UIcmdWithCustomizableArguments.hpp"

#include <iostream>

#include <G4Tokenizer.hh>
#include <G4UnitsTable.hh>
#include <G4UIcommandStatus.hh>

using std::cout;
using std::endl;

UIcmdWithCustomizableArguments::UIcmdWithCustomizableArguments
(const char* theCommandPath, G4UImessenger* theMessenger)
    :G4UIcommand(theCommandPath, theMessenger)
{
    defaultStringID  = -1;
    defaultIntegerID = -1;
    defaultDoubleID  = -1;
    defaultBoolID    = -1;
    default3VectorID = -1;
    defaultUnitID    = -1;
    defaultDoubleHasUnit  = false;
    default3VectorHasUnit = false;
}

G4int UIcmdWithCustomizableArguments::DoIt(G4String parameterList)
{
    auto token_vector = Tokenize( parameterList );
    auto status = ConvertToDefaultUnit( token_vector, &parameterList );
    if ( !!status ) return status;
    return G4UIcommand::DoIt( parameterList );
}

G4String UIcmdWithCustomizableArguments::GetNewString(const char* paramString)
{
    return GetString( defaultStringID, paramString );
}

G4int UIcmdWithCustomizableArguments::GetNewIntValue(const char* paramString)
{
    return GetIntValue( defaultIntegerID, paramString );
}

G4double UIcmdWithCustomizableArguments::GetNewDoubleValue(const char* paramString)
{
    if ( defaultDoubleHasUnit )
	return GetDoubleValue( defaultDoubleID, paramString );
    return GetDoubleRawValue( defaultDoubleID, paramString );
}

G4double UIcmdWithCustomizableArguments::GetNewDoubleRawValue(const char* paramString)
{
    return GetDoubleRawValue( defaultDoubleID, paramString ); 
}

G4bool UIcmdWithCustomizableArguments::GetNewBoolValue(const char* paramString)
{
    return GetBoolValue( defaultBoolID, paramString );
}

G4ThreeVector UIcmdWithCustomizableArguments::GetNew3VectorValue
(const char* paramString)
{
    if ( default3VectorHasUnit )
	return Get3VectorValue( default3VectorID, paramString );
    return Get3VectorRawValue( default3VectorID, paramString );
}

G4ThreeVector UIcmdWithCustomizableArguments::GetNew3VectorRawValue
(const char* paramString)
{
    return Get3VectorRawValue( default3VectorID, paramString );
}

void UIcmdWithCustomizableArguments::SetParameterStringName
(const char *theName, G4bool omittable, G4bool currentAsDefault)
{
    auto param = GetParameter( defaultStringID );
    if ( !param ) return;
    param->SetParameterName(theName);
    param->SetOmittable(omittable);
    param->SetCurrentAsDefault(currentAsDefault);
}

void UIcmdWithCustomizableArguments::SetParameterIntName
(const char *theName, G4bool omittable, G4bool currentAsDefault)
{
    auto param = GetParameter( defaultIntegerID );
    if ( !param ) return;
    param->SetParameterName(theName);
    param->SetOmittable(omittable);
    param->SetCurrentAsDefault(currentAsDefault);
}

void UIcmdWithCustomizableArguments::SetParameterDoubleName
(const char *theName, G4bool omittable, G4bool currentAsDefault)
{
    auto param = GetParameter( defaultDoubleID );
    if ( !param ) return;
    param->SetParameterName(theName);
    param->SetOmittable(omittable);
    param->SetCurrentAsDefault(currentAsDefault);
}

void UIcmdWithCustomizableArguments::SetParameterBoolName
(const char *theName, G4bool omittable, G4bool currentAsDefault)
{
    auto param = GetParameter( defaultBoolID );
    if ( !param ) return;
    param->SetParameterName(theName);
    param->SetOmittable(omittable);
    param->SetCurrentAsDefault(currentAsDefault);
}

void UIcmdWithCustomizableArguments::SetParameter3VectorName
(const char *theNameX, const char *theNameY, const char *theNameZ,
 G4bool omittable, G4bool currentAsDefault)
{
    auto x = GetParameter( default3VectorID );
    x->SetParameterName(theNameX);
    x->SetOmittable(omittable);
    x->SetCurrentAsDefault(currentAsDefault);
    auto y = GetParameter( default3VectorID+1 );
    y->SetParameterName(theNameY);
    y->SetOmittable(omittable);
    y->SetCurrentAsDefault(currentAsDefault);
    auto z = GetParameter( default3VectorID+2 );
    z->SetParameterName(theNameZ);
    z->SetOmittable(omittable);
    z->SetCurrentAsDefault(currentAsDefault);
}

void UIcmdWithCustomizableArguments::SetDefaultString(const char *defVal)
{
    auto param = GetParameter( defaultStringID );
    if ( !param ) return;
    param->SetDefaultValue(defVal);
}

void UIcmdWithCustomizableArguments::SetDefaultIntValue(G4int defVal)
{
    auto param = GetParameter( defaultIntegerID );
    if ( !param ) return;
    param->SetDefaultValue(defVal);
}

void UIcmdWithCustomizableArguments::SetDefaultDoubleValue(G4double defVal)
{
    auto param = GetParameter( defaultDoubleID );
    if ( !param ) return;
    param->SetDefaultValue(defVal);
}

void UIcmdWithCustomizableArguments::SetDefaultBoolValue(G4bool defVal)
{
    auto param = GetParameter( defaultBoolID );
    if ( !param ) return;
    param->SetDefaultValue(defVal);
}

void UIcmdWithCustomizableArguments::SetDefault3VectorValue(G4ThreeVector defVal)
{
    auto x = GetParameter( default3VectorID );
    if ( !x ) return;
    x->SetDefaultValue( defVal.x() );
    
    auto y = GetParameter( default3VectorID+1 );
    if ( !y ) return;
    y->SetDefaultValue( defVal.y() );
    
    auto z = GetParameter( default3VectorID+2 );
    if ( !z ) return;
    z->SetDefaultValue( defVal.z() );
}

void UIcmdWithCustomizableArguments::SetUnitCategory(const char *unitCategory)
{
    SetUnitCandidates( UnitsList( unitCategory ) );
}

void UIcmdWithCustomizableArguments::SetUnitCandidates(const char *candidateList)
{
    auto unit = GetParameter( defaultUnitID );
    if ( !unit ) return;
    G4String canList = candidateList;
    unit->SetParameterCandidates( canList );    
}

void UIcmdWithCustomizableArguments::SetDefaultUnit(const char *defUnit)
{
    auto unit = GetParameter( defaultUnitID );
    if ( !unit ) return;
    unit->SetOmittable( true );
    unit->SetDefaultValue( defUnit );
    SetUnitCategory( CategoryOf( defUnit ) );
}

int UIcmdWithCustomizableArguments::DefineParameter
(char theType, const char* theName)
{
    int id = GetParameterEntries();
    auto param = new G4UIparameter( theType );
    SetParameter( param );
    if ( !!theName ) param->SetParameterName( theName );
    return id;
}

/* End of public functions */

/* protected functions */

std::pair<G4int, char> UIcmdWithCustomizableArguments::DefineAString
(const char* theNameOfValue)
{
    auto id
	= DefineParameter( theStringType, theNameOfValue );
    auto p = std::make_pair( id, theStringType );
    AddNewParameter( p );
    return p;
}

std::pair<G4int, char> UIcmdWithCustomizableArguments::DefineAInteger
(const char* theNameOfValue)
{
    auto id
	= DefineParameter( theIntegerType, theNameOfValue );
    auto p = std::make_pair( id, theIntegerType );
    AddNewParameter( p );
    return p;
}

std::pair<G4int, char> UIcmdWithCustomizableArguments::DefineADouble
(const char* theNameOfValue)
{
    auto id
	= DefineParameter( theDoubleType, theNameOfValue );
    auto p = std::make_pair( id, theDoubleType );
    AddNewParameter( p );
    return p;
}

std::pair<G4int, char> UIcmdWithCustomizableArguments::DefineADoubleAndUnit
(const char* theNameOfValue, const char* theNameOfUnit)
{
    auto id
	= DefineParameter( theDoubleType, theNameOfValue );
    if ( !!theNameOfUnit ) this->DefineParameter( theStringType, theNameOfUnit );
    else this->DefineParameter( theStringType, "Unit" );
    auto p = std::make_pair( id, theDoubleAndUnitType );
    AddNewParameter( p );
    return p;
}

std::pair<G4int, char> UIcmdWithCustomizableArguments::Define3Vector
(const char* theNameOfX, const char* theNameOfY, const char* theNameOfZ)
{
    auto id
	= DefineParameter( theDoubleType, theNameOfX );
    this->DefineParameter( theDoubleType, theNameOfY );
    this->DefineParameter( theDoubleType, theNameOfZ );
    auto p = std::make_pair( id, the3VectorType );
    AddNewParameter( p );
    return p;
}

std::pair<G4int, char> UIcmdWithCustomizableArguments::Define3VectorAndUnit
(const char* theNameOfX, const char* theNameOfY, const char* theNameOfZ, const char* theNameOfUnit)
{
    auto id
	= DefineParameter( theDoubleType, theNameOfX );
    this->DefineParameter( theDoubleType, theNameOfY );
    this->DefineParameter( theDoubleType, theNameOfZ );
    if ( !!theNameOfUnit ) this->DefineParameter( theStringType, theNameOfUnit );
    else this->DefineParameter( theStringType, "Unit" );
    auto p =  std::make_pair( id, the3VectorAndUnitType );
    AddNewParameter( p );
    return p;
}

G4int UIcmdWithCustomizableArguments::GetIntValue
(G4int dictID, const char* paramString)
{
    auto str = GetString(dictID, paramString);
    return ConvertToInt(str);
}

G4double UIcmdWithCustomizableArguments::GetDoubleValue
(G4int dictID, const char* paramString)
{
    auto str = GetString(dictID, paramString);
    return ConvertToDimensionedDouble(str);
}

G4double UIcmdWithCustomizableArguments::GetDoubleRawValue
(G4int dictID, const char* paramString)
{
    auto str = GetString(dictID, paramString);
    return ConvertToDouble(str);
}

G4String UIcmdWithCustomizableArguments::GetString
(G4int dictID, const char* paramString)
{
    G4String str;
    auto token_vector = Tokenize(paramString);
    auto [ id, type ] = dictParameters[dictID];

    int token_number = 1;
    if ( type == theDoubleAndUnitType )        token_number = 2;
    else if ( type == the3VectorType )         token_number = 3;
    else if ( type == the3VectorAndUnitType )  token_number = 4;

    for ( int i=0; i<token_number; ++i ) {
	if ( i!=0 ) str += " ";
	str += token_vector[ id + i ];
    }
    return str;
}

G4bool UIcmdWithCustomizableArguments::GetBoolValue
(G4int dictID, const char* paramString)
{
    auto str = GetString(dictID, paramString);
    return ConvertToBool(str);
}

G4ThreeVector UIcmdWithCustomizableArguments::Get3VectorValue
(G4int dictID, const char* paramString)
{
    auto str = GetString(dictID, paramString);
    return ConvertToDimensioned3Vector(str);
}

G4ThreeVector UIcmdWithCustomizableArguments::Get3VectorRawValue
(G4int dictID, const char* paramString)
{
    auto str = GetString(dictID, paramString);
    return ConvertTo3Vector(str);
}

G4double UIcmdWithCustomizableArguments::GetUnitValue
(G4int dictID, const char* paramString)
{
    auto token_vector = Tokenize(paramString);
    auto [ id, type ] = dictParameters[dictID];
    
    int id_unit = 0;
    if      ( id == theDoubleAndUnitType  )  id_unit = 1;
    else if ( id == the3VectorAndUnitType )  id_unit = 3;
    else return 1.0; // a command without a unit

    return ValueOf(token_vector[ id_unit ]);
}

std::vector<G4String> UIcmdWithCustomizableArguments::Tokenize
(G4String parameterList)
{
    std::vector<G4String> token_vector;
    G4Tokenizer tkn(parameterList); G4String str;
    while( (str = tkn()) != "" ) {
	token_vector.push_back(str);
    }
    return token_vector;
}

G4int UIcmdWithCustomizableArguments::ConvertToDefaultUnit
(std::vector<G4String>& tokenVector, G4String* parameterList)
{    
    G4String converted_parameter;
    
    for ( auto [ id, type ] : dictParameters ) {

	if ( id >= (int)tokenVector.size() ) break;
	
	if( type == theDoubleAndUnitType ) {

	    auto id_unit = id + 1;
	    G4String default_unit  = GetParameter( id_unit )->GetDefaultValue();

	    if ( default_unit == "" || (int)tokenVector.size()<=id_unit ) {
		converted_parameter += tokenVector[ id ];
		converted_parameter += " ";
		continue;
	    }	    
	    
	    G4String given_unit    = tokenVector[ id_unit ];
	    
	    if ( CategoryOf( given_unit ) != CategoryOf( default_unit ) ) {
		return fParameterOutOfCandidates+1;
	    }
	    
	    G4double value_given   = ValueOf( given_unit   );
	    G4double value_default = ValueOf( default_unit );
	    G4double value = ConvertToDouble( tokenVector[ id] )
		* value_given / value_default;
	    converted_parameter += ConvertToString( value );
	    converted_parameter += " ";
	    converted_parameter += default_unit;
	    
	}
	else if ( type == the3VectorAndUnitType ) {

	    auto id_x    = id;
	    auto id_y    = id_x + 1;
	    auto id_z    = id_y + 1;
	    auto id_unit = id_z + 1;
	    G4String default_unit  = GetParameter( id_unit )->GetDefaultValue();

	    if ( default_unit == "" || (int)tokenVector.size() <= id_unit ) {
		converted_parameter += tokenVector[ id_x ];
		converted_parameter += " ";
		converted_parameter += tokenVector[ id_y ];
		converted_parameter += " ";
		converted_parameter += tokenVector[ id_z ];
		continue;
	    }	    
	    
	    G4String given_unit    = tokenVector[ id_unit ];
	    
	    if ( CategoryOf( given_unit ) != CategoryOf( default_unit ) ) {
		return fParameterOutOfCandidates+3;
	    }
	    
	    G4double value_given   = ValueOf( given_unit   );
	    G4double value_default = ValueOf( default_unit );
	    G4double x = ConvertToDouble( tokenVector[ id_x ] )
		* value_given / value_default;
	    G4double y = ConvertToDouble( tokenVector[ id_y ] )
		* value_given / value_default;
	    G4double z = ConvertToDouble( tokenVector[ id_z ] )
		* value_given / value_default;
	    converted_parameter += ConvertToString( x );
	    converted_parameter += " ";
	    converted_parameter += ConvertToString( y );
	    converted_parameter += " ";	    
	    converted_parameter += ConvertToString( z );
	    converted_parameter += " ";
	    converted_parameter += default_unit; 
	    
	}
	else {
	    converted_parameter += tokenVector[id];
	    converted_parameter += " ";
	}

    }

    *parameterList = converted_parameter;
    return 0;
    
// if ( default_unit != "" && (int)token_vector.size() >= this->GetParameterEntries() ){
// 	if ( CategoryOf(token_vector[ this->paramid_unit ])!=CategoryOf(default_unit) )
// 	    return fParameterOutOfCandidates+1;
// 	G4double value_given = ValueOf(token_vector[ this->paramid_unit ]);
// 	G4double value_default = ValueOf(default_unit);
// 	G4double value
// 	    = ConvertToDouble(token_vector[ this->paramid_value ])
// 	    * value_given / value_default;
// 	// reconstruct parameter list
// 	converted_parameter += ConvertToString(value);
// 	converted_parameter += " ";
// 	converted_parameter += default_unit;
	
// 	for ( auto i=this->GetParameterEntries(); i< (int)token_vector.size(); i++ ) {
// 	    converted_parameter += " ";
// 	    converted_parameter += token_vector[i];
// 	}
	
//     } else {
// 	converted_parameter = parameterList;
//     }

}

/* End of protected functions */

/* private functions */

void UIcmdWithCustomizableArguments::AddNewParameter(std::pair<G4int, char>& p)
{
    dictParameters.emplace_back( p );

    auto [ id, type ] = p;
    
    if ( type == theStringType && defaultStringID == -1 ) 
	defaultStringID = id;
    else if ( type == theIntegerType && defaultIntegerID == -1 )
	defaultIntegerID = id;
    else if ( type == theDoubleType && defaultDoubleID == -1 )
	defaultDoubleID = id;
    else if ( type == theDoubleAndUnitType && defaultDoubleID == -1 ) {
	defaultDoubleID = id;
	defaultDoubleHasUnit = true;
	if ( defaultUnitID == -1 ) defaultUnitID = id+1;
    }
    else if ( type == theBoolType && defaultBoolID == -1 )
	defaultBoolID = id;
    else if ( type == the3VectorType && default3VectorID == -1 )
	default3VectorID = id;
    else if ( type == the3VectorAndUnitType && default3VectorID == -1 ) {
	default3VectorID = id;
	default3VectorHasUnit = true;
	if ( defaultUnitID == -1 ) defaultUnitID = id+3;
    }    
}
