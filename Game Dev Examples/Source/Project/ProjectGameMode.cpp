// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Project.h"
#include "ProjectGameMode.h"
#include "ProjectPawn.h"
#include "Engine.h"

AProjectGameMode::AProjectGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AProjectPawn::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintPC(TEXT("Blueprint'/Game/JustBusiness/PlayerController_BP/JustBusinessPC.JustBusinessPC'"));
	if (BlueprintPC.Object) {
		PlayerControllerClass = (UClass*)BlueprintPC.Object->GeneratedClass;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, "Failed To Change Default Player Controller");
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintP(TEXT("Blueprint'/Game/player_ship/Blueprint/TP_FlyingPawn_Blueprint.TP_FlyingPawn_Blueprint'"));
	if (BlueprintP.Object) {
		DefaultPawnClass = (UClass*)BlueprintP.Object->GeneratedClass;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, "Failed To Change Default Player Controller");
	}

}
