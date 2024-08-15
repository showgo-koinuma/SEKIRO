#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Character/CharacterBase.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SEKIRO_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;
	
	// PlayerのInputMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerIMC;

	// PlayerMoveのInputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveIA;

	// InputMove イベントハンドラー関数
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	// プレイヤーの向く方向を決める
	void SetPlayerRotation(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LocalMoveInputVector;

	// カメラアニメーションをTick管理
	void CameraAnimTick(const float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	
//---------------LockOnSystem---------------
	// LockOn出来る最大角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	float MaxLockOnAngle;
	
	// LockOn出来る最大距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	float MaxLockOnRange;

	// スクリーン座標のオフセット角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	FRotator LockOnOffsetRotation;

	// ロックオン中のカメラ回転速度(度/秒)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	float LockOnCameraRotationSpeed;
	
	// LockOnの対象
	TWeakObjectPtr<AEnemyCharacter> LockOnTarget;
	
	// 敵をロックオン、ロックオンしていたら外す
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void LockOnEnemy();
	
	// 現在ロックオンしているか
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LockOn")
	bool IsLocked() const { return LockOnTarget.IsValid(); }

	// 現在ロックオンしているEnemyCharacter
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LockOn")
	AEnemyCharacter* GetLockedOnEnemy() const {return LockOnTarget.Get();}

	void LockOnCameraControl(const float DeltaTime);

//---------------ArmLength---------------
	// アニメーションタイマー
	float ArmLengthTimer;

	// アニメーション時間
	float ArmLengthAnimDuration;

	// アニメーション後の値
	float NewArmLength;

	// アニメーション開始時の値
	float LastArmLength;
	
	// Sprig Armの長さのアニメーションを開始する
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation")
	void SetArmLengthAnim(const float NewValue, const float Duration)
	{
		ArmLengthTimer = 0;
		ArmLengthAnimDuration = Duration;
		NewArmLength = NewValue;
		LastArmLength = SpringArm->TargetArmLength;
	}

//---------------ArmLocation---------------
	// アニメーションタイマー
	float ArmLocationTimer;

	// アニメーション時間
	float ArmLocationAnimDuration;

	// アニメーション後の値
	FVector NewArmLocation;

	// アニメーション開始時の値
	FVector LastArmLocation;
	
	// Sprig Armの長さのアニメーションを開始する
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation")
	void SetArmLocationAnim(const FVector NewValue, const float Duration)
	{
		ArmLocationTimer = 0;
		ArmLocationAnimDuration = Duration;
		NewArmLocation = NewValue;
		LastArmLocation = SpringArm->GetRelativeLocation();
	}
};
