// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(DisplayName = "Bullet"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile"),
	};
}


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ShotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;
	
	UPROPERTY(Category = Ammo, EditDefaultsOnly)
	int32 reloadTime;
	
	UPROPERTY(Category = Configuration, EditDefaultsOnly)
	int32 weaponDamage;
};

UCLASS()
class PROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Fire();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Instant_Fire();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void ProjectileFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UStaticMeshComponent* MeshComponent;
	
protected:

	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo);
	
	void UseAmmo();

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);
	void ShouldDealDamage(AActor* TestActor) const;
private:
	int32 currentAmmo;
	float LastFireTime;
	/* Time between shots for repeating fire */
	float TimeBetweenShots;

	virtual void HandleFiring();

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDirection, int32 RandomSeed, float ReticleSpread);
};
