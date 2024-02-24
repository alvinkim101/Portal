// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portal.h"
#include "Modules/ModuleManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PortalCharacter.h"
#include "PortalWall.h"

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
	Camera->SetupAttachment(Surface);
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
}