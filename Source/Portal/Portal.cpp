// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portal.h"
#include "Modules/ModuleManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PortalCharacter.h"
#include "PortalWall.h"
#include "Kismet/KismetMathLibrary.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Portal, "Portal" );

// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Surface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Surface"));
	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));

	SetRootComponent(Frame);
	Surface->SetupAttachment(Frame);
	Camera->SetupAttachment(Frame);

	Camera->bEnableClipPlane = true;
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Character)
	{
		return;
	}

	FTransform PlayerTransform = Cast<APlayerController>(Character->GetController())->PlayerCameraManager->GetTransform();
	FRotator RelativeInversePortalRotation = (GetActorForwardVector() * -1.0).Rotation();
	FTransform PortalTransform(RelativeInversePortalRotation, GetActorLocation(), GetActorScale());
	
	FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(PlayerTransform, PortalTransform);
	Pair->Camera->SetRelativeTransform(RelativeTransform);
}

void APortal::Initialize(APortal* InPair, APortalCharacter* InCharacter)
{
	Pair = InPair;
	Character = InCharacter;
}

void APortal::Move(FVector InLocation, FRotator InRotation, class APortalWall* InWall)
{
	SetActorLocation(InLocation);
	SetActorRotation(InRotation);
	Wall = InWall;

	Camera->ClipPlaneBase = InLocation;
	Camera->ClipPlaneNormal = InRotation.Vector();
}