#include "Character/PlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter(): PlayerIMC(nullptr), MoveIA(nullptr), LockOnTarget(nullptr), MaxLockOnAngle(60), MaxLockOnRange(400)
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

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LockOnCameraControl(DeltaSeconds);
	CameraAnimTick(DeltaSeconds);
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

void APlayerCharacter::CameraAnimTick(const float DeltaTime)
{
	// カメラアームの長さのアニメーション
	if (ArmLengthTimer < ArmLengthAnimDuration)
	{
		ArmLengthTimer += DeltaTime;
		// lerpの割合となる値
		const float LerpFactor = FMath::Clamp(ArmLengthTimer / ArmLengthAnimDuration, 0.0f, 1.0f);
		// 値にイージングをかける
		const float EasedLerpFactor = FMath::InterpEaseInOut(0.0f, 1.0f, LerpFactor, 2);
		SpringArm->TargetArmLength = LastArmLength + (NewArmLength - LastArmLength) * EasedLerpFactor;
	}

	// カメラアームの位置のアニメーション
	if (ArmLocationTimer < ArmLocationAnimDuration)
	{
		ArmLocationTimer += DeltaTime;
		const float LerpFactor = FMath::Clamp(ArmLocationTimer / ArmLocationAnimDuration, 0.0f, 1.0f);
		const float EasedLerpFactor = FMath::InterpEaseInOut(0.0f, 1.0f, LerpFactor, 2);
		SpringArm->SetRelativeLocation(LastArmLocation + (NewArmLocation - LastArmLocation) * EasedLerpFactor);
	}
}

void APlayerCharacter::LockOnEnemy()
{
	// LockOn中なら外すだけ
	if (IsLocked())
	{
		LockOnTarget = nullptr;
		return;
	}

	// プレイヤーを中心に球状に敵オブジェクトを取得
	TArray<AActor*> OverlappingActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	TargetObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	const FVector CurrentPlayerLocation = GetActorLocation();

	// 取得出来た場合、角度を判定する
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CurrentPlayerLocation, MaxLockOnRange, TargetObjectTypes,
			AEnemyCharacter::StaticClass(), TArray<AActor*>(), OverlappingActors))
	{
		// 現在のカメラの正面ベクトル
		const FVector ControlForward = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
		
		float SmallestAngle = 0.0f;
		TObjectPtr<AEnemyCharacter> SmallestAngleTarget = nullptr;

		for (TObjectPtr<AActor> Actor : OverlappingActors)
		{
			const TObjectPtr<AEnemyCharacter> Enemy = Cast<AEnemyCharacter>(Actor);
			
			if (!Enemy) continue;
			
			const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ControlForward,
				(Enemy->GetActorLocation() - CurrentPlayerLocation).GetSafeNormal())));

			// 最初の敵、またはなす角が最も小さければ入れ替える
			if ((!SmallestAngleTarget || SmallestAngle > AngleDegrees) && AngleDegrees < MaxLockOnAngle)
			{
				SmallestAngleTarget = Enemy;
				SmallestAngle = AngleDegrees;
			}
		}

		LockOnTarget = SmallestAngleTarget;
	}
}

void APlayerCharacter::LockOnCameraControl(const float DeltaTime)
{
	// 対象がいなければ何もしない
	if (!LockOnTarget.IsValid()) return;

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		FVector2D TargetScreenPosition;
		
		if (UGameplayStatics::ProjectWorldToScreen(PlayerController, LockOnTarget->GetActorLocation(), TargetScreenPosition))
		{
			// オフセットをかける
			TargetScreenPosition += LockOnScreenPositionOffset;
			
		}
		else // スクリーン内に入って無かった場合
		{
			
		}
	}
}
