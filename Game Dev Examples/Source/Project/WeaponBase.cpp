// Fill out your copyright notice in the Description page of Project Settings.

#include "Project.h"
#include "WeaponBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MeshComponent"));
	MeshComponent->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWeaponBase::Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Fire() Called");
	//Instant_Fire();
}

void AWeaponBase::Instant_Fire()
{

	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
	const FVector AimDir = MeshComponent->GetSocketRotation("MF").Vector();
	const FVector StartTrace = MeshComponent->GetSocketLocation("MF");
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
}

void AWeaponBase::ProjectileFire()
{
}

FHitResult AWeaponBase::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo)
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void AWeaponBase::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDirection, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDirection * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 10000, 10.f);
}

void AWeaponBase::UseAmmo()
{
	currentAmmo--;
}


void AWeaponBase::HandleFiring(){

	//Aim direction

	//Trace weapon impact

	//Process hit
}
