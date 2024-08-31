#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Enemy/ITargetableInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCamera.generated.h"

// プレイヤーのカメラを制御
UCLASS()
class SEKIRO_API APlayerCamera : public AActor
{
	GENERATED_BODY()

public:
	APlayerCamera();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	// SpringArm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	// カメラのアニメーションカーブ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> CameraAnimCurve;

	// 追従するプレイヤー
	TSoftObjectPtr<AActor> OwnerPlayer;

private:
	// Blueprintで生成したコンポネントを取得する
	void FindMyComponents();

//-------------------------------NormalCamera-------------------------------

	// カメラをOwnerに追従させる
	void FollowOwnerPlayer(float DeltaTime);
	
	TSoftObjectPtr<APlayerController> PlayerController;

protected:
	// 感度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraParam")
	float CameraSens = 0.f;
	
	// 対象を追従するスピード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraParam")
	float FollowOwnerSpeed = 0.f;

	// 追従スピードが最大となる距離
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraParam")
	float MaxFollowSpeedDistance = 0.f;

//-------------------------------LockOnCamera-------------------------------
	
private:
	// ロックオン時のカメラ制御
	void LockOnCameraControl(const float DeltaTime);
	
	// LockOnの対象
	IITargetableInterface* LockOnTarget = nullptr;

	// LockOn対象のActor
	AActor* LockOnTargetActor = nullptr;

public:
	// 敵をロックオン、ロックオンしていたら外す
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void LockOn();
	
	// 現在ロックオンしているか
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LockOn")
	bool IsLocked() const { return LockOnTarget != nullptr; }
	
	// 現在ロックオンしているActor
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LockOn")
	const AActor* GetLockedOnTargetActor() const;

protected:
	// LockOn出来る最大角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	float MaxLockOnAngle;
	
	// LockOn出来る最大距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	float MaxLockOnRange;
	
	// スクリーン座標のオフセット角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	FRotator LockOnOffsetRotation;
	
	// ロックオン中のカメラ最大回転速度(度/秒)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn")
	float LockOnCameraRotationMaxSpeed;

	// ロックオンカメラの最大速度になる角度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn")
	float MaxRotationSpeedAngle;
	
	// ロックオン中でも一時的にカメラ操作を解放するためのもの
	// falseだとカメラを操作できる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	bool LookToLockOnTarget;

//-------------------------------CameraAnimation-------------------------------

public:
	/** Armの長さのアニメーション
	 * @param RelativeValue 目標のデフォルトからの差分
	 * @param Duration 目標の値である時間 */
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation")
	void SetArmLengthAnim(const float RelativeValue, const float Duration);

	/** Arm位置のアニメーション
	 * @param RelativeValue 目標のデフォルトからの差分
	 * @param Duration 目標の値である時間 */
	UFUNCTION(BlueprintCallable, Category = "CameraAnimation")
	void SetArmLocationAnim(const FVector RelativeValue, const float Duration);

	// カメラアニメーションの長さ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraAnimation")
	float CameraAnimTime = 1.f;

private:
	// Tick内でカメラアニメーションを制御する
	void CameraAnimControl(const float DeltaTime);
	
	// デフォルトのSpringArmの長さ
	float DefaultArmLength = 0.f;

	// Animの最終値
	float TargetLength;

	// Animの初期値
	float LastLength;

	float LengthAnimDuration;

	float LengthAnimTimer;
	
	// デフォルトのSpringArm位置
	FVector DefaultArmLocation;

	// 追従位置のオフセット
	FVector TargetLocationOffset = FVector();

	// Animの最終値
	FVector TargetLocation;

	// Animの初期値
	FVector LastLocation;

	float LocationAnimDuration;

	float LocationAnimTimer;
};
