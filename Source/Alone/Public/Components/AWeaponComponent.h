// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACoreTypes.h"
#include "AWeaponComponent.generated.h"

class AABaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALONE_API UAWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAWeaponComponent();

    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<AABaseWeapon> WeaponType, int32 AmmoAmount);
    bool NeedAmmo(TSubclassOf<AABaseWeapon> WeaponType);

    void Zoom(bool Enabled);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    AABaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<AABaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool CanFire() const;
    bool CanEquip() const;
    void StartEquipWeapon();
    void EquipWeapon(int32 WeaponIndex);

private:
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void SpawnWeapons();
    void AttachWeaponToSocket(AABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimations();
    void OnChangeWeapon(USkeletalMeshComponent* MeshComp);
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool CanReload() const;

    void OnClipEmpty(AABaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();
};
