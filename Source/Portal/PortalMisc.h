#pragma once

#include "CoreMinimal.h"

#define PortalTraceChannel ECollisionChannel::ECC_GameTraceChannel2

UENUM()
enum class EPortalColor : uint8
{
	// 0 Blue
	Blue			UMETA(DisplayName = "Blue"),
	// 1 Orange
	Orange			UMETA(DisplayName = "Orange")
};