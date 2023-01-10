// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//macro function for debugging that shows from where the function is called, what line it is etc
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define PRINT_CALL_INFO() UE_LOG(LogTemp, Warning, TEXT("%s"), *CALLINFO)

#define PRINT_LOG(fmt, ...)  UE_LOG(LogTemp, Warning, TEXT("%s %s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))