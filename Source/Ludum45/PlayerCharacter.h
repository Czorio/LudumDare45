// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpotLightComponent;
class UCameraComponent;
class USoundBase;

UCLASS()
class LUDUM45_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	USpotLightComponent* FlashlightComp;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	bool bHasFlashlight;
	bool bFlashlightOn;

	UPROPERTY(EditDefaultsOnly, Category = Audio)
	USoundBase* FlashlightToggleSound;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float FlashlightIntensity;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float InteractionRange;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void EndCrouch();
	void BeginCrouch();
	void ToggleFlashlight();
	void Interact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void ReceiveFlashlight();

};
