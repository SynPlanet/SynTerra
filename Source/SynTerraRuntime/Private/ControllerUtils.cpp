// Copyright (C) 2025 SynPlanet Holding.
#include "ControllerUtils.h"

THIRD_PARTY_INCLUDES_START
#include "stp_utils.h"
THIRD_PARTY_INCLUDES_END

bool ControllerUtils::IsValidEMail( const FString& EMail )
{
	return is_valid_email( TCHAR_TO_UTF8( *EMail ) );
}

FString ControllerUtils::SendSignIn( const FString& EMail, const FString& Password )
{
	const char* msg = send_sign_in( TCHAR_TO_UTF8( *EMail ), TCHAR_TO_UTF8( *Password ) );
	FString Msg( msg );
	free_string( msg );
	return Msg;
}

FString ControllerUtils::SendSignUp( const FString& UserName, const FString& EMail, const FString& Password )
{
	const char* msg = send_sign_up( TCHAR_TO_UTF8( *UserName ), TCHAR_TO_UTF8( *EMail ), TCHAR_TO_UTF8( *Password ) );
	FString Msg( msg );
	free_string( msg );
	return Msg;
}

bool ControllerUtils::IsAuthorized()
{
	return is_authorized();
}

