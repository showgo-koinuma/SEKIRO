#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LocalMoveInputVector;

	// カメラアニメーションをTick管理
	void CameraAnimTick(const float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraAnimation")
	TObjectPtr<USpringArmComponent> SpringArm;

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
};
