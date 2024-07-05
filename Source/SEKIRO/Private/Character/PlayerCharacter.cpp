#include "Character/PlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IO/IoStore.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter(): PlayerIMC(nullptr), MoveIA(nullptr)
{
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// IMCの登録
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerIMC, 0);
		}
	}

	// InputActionと処理のバインド
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move
		EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponentがない"));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;
	
	// 向いている方向
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// input vector2d
	const FVector2D input = Value.Get<FVector2D>();
	
	// 入力の方向 XY逆
	FVector Direction = UKismetMathLibrary::MakeVector(input.Y, input.X, 0);
	Direction.Normalize();
	Direction = YawRotation.RotateVector(Direction);

	const FRotator PlayerRotator = UKismetMathLibrary::MakeRotator(0, 0, -GetActorRotation().Yaw);
	LocalMoveInputVector = PlayerRotator.RotateVector(Direction);

	//GetCharacterMovement()->Velocity = Direction * MoveSpeed;
}
