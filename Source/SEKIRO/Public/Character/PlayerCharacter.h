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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraAnimation")
	TObjectPtr<USpringArmComponent> SpringArm;

	// カメラのデフォルトアーム距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraAnimation")
	float ArmDefaultLength;
	
	// カメラのデフォルト位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraAnimation")
	FVector ArmDefaultLocation;

	// カメラアニメーションのイージングの強さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation")
	float ArmAnimEaseExponent;

	// カメラ位置をリセットするまでの時間
	float ResetCameraTime;

	float ResetCameraTimer;

	// カメラリセットアニメーションの長さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraAnimation")
	float ResetCameraDuration;

	// カメラリセットのタイマーを始める
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation")
	void StartResetArm(const float ResetTime);

	// カメラのSpringArmのLengthとLocationを初期値にリセットする
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation")
	void ResetArmLengthAndLocation();
	
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

	// ロックオン中でも一時的にカメラ操作を解放するためのもの
	// falseだとカメラを操作できる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	bool LookToLockOnTarget;

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
		LookToLockOnTarget = false;
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
		LookToLockOnTarget = false;
	}

//---------------パリィ忍殺のカメラアニメーション---------------
	// パリィ忍殺のカメラ移動の長さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation|ParryDeathblowAnim")
	float ParryDeathblowAnimDuration;

	// パリィ忍殺のカメラが戻るまでの時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation|ParryDeathblowAnim")
	float ParryDeathblowZoomDuration;

	// 固定値ピッチ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation|ParryDeathblowAnim")
	float ParryDeathblowFixedPitch;

	// 追加されるYaw値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation|ParryDeathblowAnim")
	float ParryDeathblowYawToBeAdded;

	// パリィ忍殺時のSpringArmの長さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation|ParryDeathblowAnim")
	float ParryDeathblowArmLength;

	// パリィ忍殺時のSpringArmの位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraAnimation|ParryDeathblowAnim")
	FVector ParryDeathblowArmLocation;
	
	// パリィ忍殺タイマー
	float ParryDeathblowTimer = 10;

	// アニメーション開始時の角度
	FRotator LastCameraRotator;

	// パリィ忍殺のカメラアニメーションを開始する
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation|ParryDeathblowAnim")
	void StartParryDeathblowCameraAnim()
	{
		ParryDeathblowTimer = 0;
		LastCameraRotator = GetController()->GetControlRotation();
		// 正確にLerpするために半分以上はマイナスにする
		if (LastCameraRotator.Pitch > 180) LastCameraRotator.Pitch = LastCameraRotator.Pitch - 360;

		// SpringArmのアニメーション
		SetArmLengthAnim(ParryDeathblowArmLength, ParryDeathblowAnimDuration);
		SetArmLocationAnim(ParryDeathblowArmLocation, ParryDeathblowAnimDuration);
		
		LookToLockOnTarget = false;
		StartResetArm(ParryDeathblowZoomDuration);
	}
};
