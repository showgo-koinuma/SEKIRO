#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayerCamera.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SEKIRO_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	
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

	// プレイヤーカメラのクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerCamera")
	TSubclassOf<APlayerCamera> PlayerCameraClass;

	// プレアイヤーカメラインスタンス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCamera")
	TObjectPtr<APlayerCamera> PlayerCamera;

	UFUNCTION(BlueprintCallable, Category = "PlayerCamera")
	void LockOn();
};
