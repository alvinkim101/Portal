// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Set.h"
#include "Portal.generated.h"

UCLASS()
class PORTAL_API APortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Initialize(class APortal* InPair, class APortalCharacter* InCharacter);

	void Move(FVector Location, FRotator Rotation, class APortalWall* Wall);

	void Teleport(AActor* Actor, FTransform RelativeActorTransform);

	UFUNCTION()
	void BeginActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Frame;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Surface;

	UPROPERTY(EditDefaultsOnly)
	class USceneCaptureComponent2D* Camera;

	class APortalWall* Wall;
	class APortal* Pair;
	class APortalCharacter* Character;

	TSet<AActor*> TeleportableSet; // Actors to check whether or not to teleport
};