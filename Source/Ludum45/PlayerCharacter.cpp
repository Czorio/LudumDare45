// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SpotLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "InteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(RootComponent);

	FlashlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComp"));
	FlashlightComp->SetIntensity(FlashlightIntensity);
	FlashlightComp->SetupAttachment(CameraComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	bHasFlashlight = false;
	bFlashlightOn = false;
}

void APlayerCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::BeginCrouch()
{
	Crouch();
}


void APlayerCharacter::EndCrouch()
{
	UnCrouch();
}

void APlayerCharacter::ToggleFlashlight()
{
	if (bHasFlashlight)
	{
		if (bFlashlightOn)
		{
			FlashlightComp->SetIntensity(0.f);
			bFlashlightOn = false;
		}
		else
		{
			FlashlightComp->SetIntensity(FlashlightIntensity);
			bFlashlightOn = true;
		}

		UGameplayStatics::PlaySound2D(this, FlashlightToggleSound);
	}
}

void APlayerCharacter::Interact()
{
	FHitResult Hit;

	FRotator Direction = CameraComp->GetComponentRotation();
	Direction.Normalize();
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + Direction.Vector() * InteractionRange;

	FCollisionQueryParams Params;

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, Params);
	if (bHasHit)
	{
		AInteractableActor* Interactable = Cast<AInteractableActor>(Hit.GetActor());
		if (Interactable)
		{
			Interactable->OnInteract(this);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);

	PlayerInputComponent->BindAction("ToggleFlashlight", IE_Pressed, this, &APlayerCharacter::ToggleFlashlight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
}

void APlayerCharacter::ReceiveFlashlight()
{
	bHasFlashlight = true;
}

