#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
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
	
	// PlayerのInputMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerIMC;

	// PlayerMoveのInputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveIA;

	// InputMove イベントハンドラー関数
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paramater")
	float MoveSpeed = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LocalMoveInputVector;
};
