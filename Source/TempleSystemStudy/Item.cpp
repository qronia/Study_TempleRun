// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>("Root");
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));

	RootComponent = Scene;
	Body->SetupAttachment(Scene);
	PointLight->SetupAttachment(Scene);

	PointLight->SetIntensity(2000.f);
	PointLight->SetLightColor(FLinearColor::Yellow);

	RotatingMovement->RotationRate = FRotator{ 0.f,180.f, 0.f };
	Body->SetRelativeLocation(FVector{ 0.f,15.f,0.f });


}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

