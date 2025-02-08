// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"

struct UserInfo
{
	FString UserName;
	FString EMail;
	FString Password;
	int CurrenLimit = 0;
	FString UpdateLimitTime;
};

