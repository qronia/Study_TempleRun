// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TempleSystemStudyCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

//////////////////////////////////////////////////////////////////////////
// ATempleSystemStudyCharacter

ATempleSystemStudyCharacter::ATempleSystemStudyCharacter()
{
	bCanTurn = false;
	bIsDead = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector{ 0.f,0.f,100.f };

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>("Explosion_Emitter");
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_EXPLOSION(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (PS_EXPLOSION.Succeeded())
	{
		ExplosionParticle->SetAutoActivate(false);
		ExplosionParticle->SetTemplate(PS_EXPLOSION.Object);
		ExplosionParticle->SetupAttachment(GetMesh());
	}

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_EXPLOSION(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (SC_EXPLOSION.Succeeded())
	{
		Audio->SetSound(SC_EXPLOSION.Object);
		Audio->SetActive(false);
	}


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATempleSystemStudyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ATempleSystemStudyCharacter::TurnLeft);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ATempleSystemStudyCharacter::TurnRight);

	PlayerInputComponent->BindAxis("MoveRight", this, &ATempleSystemStudyCharacter::MoveRight);
}

void ATempleSystemStudyCharacter::MoveForward(float Value)
{
	if (Controller == NULL) return;
	
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ATempleSystemStudyCharacter::MoveRight(float Value)
{
	if (Controller == NULL && Value==0.f) return;

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ATempleSystemStudyCharacter::TurnLeft()
{
	if ((Controller == NULL) || !bCanTurn) return;
	DesiredRotation += FRotator{ 0.f,-90.f,0.f }; // BLUEPRINT : Combine Rotators
}

void ATempleSystemStudyCharacter::TurnRight()
{
	if ((Controller == NULL) || !bCanTurn) return;
	DesiredRotation += FRotator{ 0.f,90.f,0.f }; // BLUEPRINT : Combine Rotators
}

void ATempleSystemStudyCharacter::TurnCornor()
{
	if (GetActorRotation() == DesiredRotation) return;

	GetController()->SetControlRotation(
		FMath::RInterpTo(GetController()->GetControlRotation(),
			DesiredRotation, 
			GetWorld()->GetDeltaSeconds(),
			10.f)
	);

	bCanTurn = false;
}

void ATempleSystemStudyCharacter::Death()
{
	if (bIsDead) return;

	bIsDead = true;
	DisableInput(UGameplayStatics::GetPlayerController(this,0));
	GetMesh()->SetVisibility(false);
	ExplosionParticle->Activate(false);
	Audio->Play();
}

void ATempleSystemStudyCharacter::AddCoin()
{
	++TotalCoins;
}

void ATempleSystemStudyCharacter::Tick(float DeltaTimes)
{
	if (bIsDead) return;

	TurnCornor();
	MoveForward(1.f);
}
