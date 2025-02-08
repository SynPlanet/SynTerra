// Copyright (C) 2025 SynPlanet Holding.
#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "FUserInfo.h"

class SUserInfo : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SUserInfo ) {}
		SLATE_ATTRIBUTE( UserInfo, UserInfoAttr )
	SLATE_END_ARGS()

	virtual void Construct( const FArguments& InArgs );
	virtual ~SUserInfo() override {}

private:
	TAttribute<UserInfo> UserInfoAttr;
};
