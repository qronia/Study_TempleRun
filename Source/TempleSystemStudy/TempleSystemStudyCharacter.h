// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "TempleSystemStudyCharacter.generated.h"

UCLASS(config=Game)
class ATempleSystemStudyCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ATempleSystemStudyCharacter();

	virtual void Tick(float DeltaTimes) override;

	
	///** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	//float BaseTurnRate;

	///** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	//float BaseLookUpRate;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnLeft();
	void TurnRight();
	void TurnCornor();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void AddCoin();

	UPROPERTY(BlueprintReadOnly)
	int32 TotalCoins;

	UPROPERTY(BlueprintReadWrite)
	bool bCanTurn;
	bool bIsDead;
	FRotator DesiredRotation;

	UPROPERTY()
	UParticleSystemComponent* ExplosionParticle;

	UPROPERTY()
	UAudioComponent* Audio;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

