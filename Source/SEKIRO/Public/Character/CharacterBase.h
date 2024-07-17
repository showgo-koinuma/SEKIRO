#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterAttackParam.h"
#include "CharacterAttributeSet.h"
#include "CharacterParameter.h"
#include "GAS/WaitInputGameplayAbility.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	// 通常状態
	Normal,
	// 攻撃中
	Attack,
	// 防御中
	Defense,
	// 攻撃を食らっている
	Hit,
	// Dodge中
	Dodge
};

UCLASS()
class SEKIRO_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AbilitySystemコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	// IAbilitySystemInterfaceからのoverride
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterAttributeSet* CharacterAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CharacterGameplayAbilities;

	// WaitInputのTaskを使うAbility
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<UWaitInputGameplayAbility>> WaitInputAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TObjectPtr<UCapsuleComponent> WeaponCollisionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter")
	UCharacterParameter* CharacterParameter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parameter")
	UCharacterAttackParam* CharacterAttackParam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECharacterState CharacterState = ECharacterState::Normal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LocalVelocity;

	void BindASCInput();

	bool bASCInputBound;

	// 攻撃を受ける
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HitAttack(const UCharacterAttackParam* AttackParam, FVector Direction, FVector HitPoint);
	
	void SetLocalVelocity();

private:
	// TagからStateへ反映させる
	void ApplyCharacterState();
};