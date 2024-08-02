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
	Dodge,
	// 体幹が崩れている
	BrokePosture,
	// 死んでる
	Dead,
	// モーション中の無敵時間 今回は忍殺時だけかな
	DuringMotion
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

	// 武器のコリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TObjectPtr<UCapsuleComponent> WeaponCollisionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter")
	UCharacterParameter* CharacterParameter;

	// 今の攻撃パラメーター
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Parameter")
	UCharacterAttackParam* CharacterAttackParam;

	// 攻撃ごとの多段ヒット回数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	int CurrentAttackHitCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState = ECharacterState::Normal;

	// キャラクターの視点から見た速度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LocalVelocity;

	void BindASCInput();

	bool bASCInputBound;

	// 攻撃を受ける
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HitAttack(const UCharacterAttackParam* AttackParam, FVector Direction, FVector HitPoint);

	// 自分視点の速度を求める
	void SetLocalVelocity();

	/** AttributeSetのTurnSpeedを基にTargetに向けて回転させる
	 * @param TargetDirection 向きたい方向
	 */
	UFUNCTION(BlueprintCallable)
	void SetRotationToTarget(FVector TargetDirection, const float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool LookingAtTarget();

private:
	// TagからStateへ反映させる
	void ApplyCharacterState();

	// 移動
	void Movement();

public:
	// 死んだときAttributeSetから呼び出される
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDead();

	// 体勢を崩したときAttributeSetから呼び出される
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBrokePosture();
};