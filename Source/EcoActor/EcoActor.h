// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM()
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(EcoActor, Log, All);

#define LOG_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_S(Verbosity) UE_LOG(EcoActor, Verbosity, TEXT("%s"), *LOG_INFO)
#define LOG(Verbosity, Format, ...) UE_LOG(EcoActor, Verbosity, TEXT("%s%s"), *LOG_INFO, *FString::Printf(Format, ##__VA_ARGS__))

