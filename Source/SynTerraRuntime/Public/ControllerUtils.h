// Copyright (C) 2025 SynPlanet Holding.
#pragma
#include "CoreMinimal.h"

class SYNTERRARUNTIME_API ControllerUtils
{
public:
	static bool IsValidEMail( const FString& EMail );
	static FString SendSignIn( const FString& EMail, const FString& Password );
	static FString SendSignUp( const FString& UserName, const FString& EMail, const FString& Password );
	static bool IsAuthorized();
};
