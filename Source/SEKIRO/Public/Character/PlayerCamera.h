#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UCameraComponent> CameraComponent;

	// 追従するプレイヤー
	TSoftObjectPtr<AActor> OwnerPlayer;

private:
	// Blueprintで生成したコンポネントを取得する
	void FindMyComponents();

//-------------------------------NormalCamera-------------------------------

	// カメラをOwnerに追従させる
	void FollowOwnerPlayer(float DeltaTime);
	
	TSoftObjectPtr<APlayerController> PlayerController;
	
	// デフォルトのSpringArm位置
	FVector DefaultArmLocation;
	
	// デフォルトのSpringArmの長さ
	float DefaultArmLength = 0.f;

protected:
	// 感度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraParam")
	float CameraSens = 0.f;
	
	// 対象を追従するスピード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraParam")
	float FollowOwnerSpeed = 0.f;

//-------------------------------LockOnCamera-------------------------------

	// // LockOn出来る最大角度
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	// float MaxLockOnAngle;
	//
	// // LockOn出来る最大距離
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	// float MaxLockOnRange;
	//
	// // スクリーン座標のオフセット角度
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	// FRotator LockOnOffsetRotation;
	//
	// // ロックオン中のカメラ回転速度(度/秒)
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	// float LockOnCameraRotationSpeed;
	//
	// // LockOnの対象
	// TWeakObjectPtr<AEnemyCharacter> LockOnTarget;
	//
	// // 敵をロックオン、ロックオンしていたら外す
	// UFUNCTION(BlueprintCallable, Category = "LockOn")
	// void LockOnEnemy();
	//
	// // 現在ロックオンしているか
	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LockOn")
	// bool IsLocked() const { return LockOnTarget.IsValid(); }
	//
	// // 現在ロックオンしているEnemyCharacter
	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LockOn")
	// AEnemyCharacter* GetLockedOnEnemy() const {return LockOnTarget.Get();}
	//
	// void LockOnCameraControl(const float DeltaTime);
	//
	// // ロックオン中でも一時的にカメラ操作を解放するためのもの
	// // falseだとカメラを操作できる
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn")
	// bool LookToLockOnTarget;
};
