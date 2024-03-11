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

	Surface->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	OnActorBeginOverlap.AddUniqueDynamic(this, &APortal::BeginActorOverlap);
	OnActorEndOverlap.AddUniqueDynamic(this, &APortal::EndActorOverlap);
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

	TSet<AActor*> LocalTeleportableSet = TeleportableSet;
	for (const auto& Actor : LocalTeleportableSet)
	{
		FTransform ActorTransform = Actor->GetTransform();
		FTransform FutureActorTransform(Actor->GetActorRotation(), Actor->GetActorLocation() + Actor->GetVelocity() * DeltaTime, Actor->GetActorScale());
		FTransform RelativeActorTransform = UKismetMathLibrary::MakeRelativeTransform(ActorTransform, PortalTransform);
		FTransform RelativeFutureActorTransform = UKismetMathLibrary::MakeRelativeTransform(FutureActorTransform, PortalTransform);

		if (RelativeActorTransform.GetLocation().X < 0 && RelativeFutureActorTransform.GetLocation().X > 0)
		{
			Teleport(Actor, RelativeActorTransform);
		}
	}
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

void APortal::Teleport(AActor* Actor, FTransform RelativeActorTransform)
{
	FVector Velocity = Actor->GetVelocity() * GetWorld()->DeltaTimeSeconds;
	Actor->SetActorLocation(Pair->GetActorLocation() + RelativeActorTransform.Rotator().Vector() * Velocity.Length());
	Actor->SetActorRotation(Pair->GetActorRotation() + RelativeActorTransform.Rotator());
	Actor->GetRootComponent()->ComponentVelocity = RelativeActorTransform.Rotator().Vector() * Velocity.Length();

	if (Actor == Character)
	{
		Character->GetController()->SetControlRotation(Pair->GetActorRotation() + RelativeActorTransform.Rotator());
	}
}

void APortal::BeginActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Wall)
	{
		Cast<UPrimitiveComponent>(OtherActor->GetRootComponent())->IgnoreActorWhenMoving(Wall, true);
	}

	TeleportableSet.Add(OtherActor);
}

void APortal::EndActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Wall)
	{
		Cast<UPrimitiveComponent>(OtherActor->GetRootComponent())->IgnoreActorWhenMoving(Wall, false);
	}

	TeleportableSet.Remove(OtherActor);
}