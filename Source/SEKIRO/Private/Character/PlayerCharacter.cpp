#include "Character/PlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	// SetPlayerRotation(DeltaSeconds);
	// LockOnCameraControl(DeltaSeconds);
	// CameraAnimTick(DeltaSeconds);
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

void APlayerCharacter::SetPlayerRotation(float DeltaTime)
{
	// ロックオンしているか ロックオンしていなければ
	if (IsLocked())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetRotationToTarget(LockOnTarget->GetActorLocation() - GetActorLocation(), DeltaTime);
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
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
		const float EasedLerpFactor = FMath::InterpEaseOut(0.0f, 1.0f, LerpFactor, ArmAnimEaseExponent);
		SpringArm->TargetArmLength = LastArmLength + (NewArmLength - LastArmLength) * EasedLerpFactor;
	}

	// カメラアームの位置のアニメーション
	if (ArmLocationTimer < ArmLocationAnimDuration)
	{
		ArmLocationTimer += DeltaTime;
		const float LerpFactor = FMath::Clamp(ArmLocationTimer / ArmLocationAnimDuration, 0.0f, 1.0f);
		const float EasedLerpFactor = FMath::InterpEaseOut(0.0f, 1.0f, LerpFactor, ArmAnimEaseExponent);
		SpringArm->SetRelativeLocation(LastArmLocation + (NewArmLocation - LastArmLocation) * EasedLerpFactor);
	}

	// パリィ忍殺のカメラアニメーション
	if (ParryDeathblowTimer < ParryDeathblowAnimDuration)
	{
		ParryDeathblowTimer += DeltaTime;

		// Pitch
		float EasedLerpPitch = FMath::InterpEaseOut(static_cast<float>(LastCameraRotator.Pitch), ParryDeathblowFixedPitch,
			ParryDeathblowTimer / ParryDeathblowAnimDuration, ArmAnimEaseExponent);
		// Yaw
		const float LastYaw = static_cast<float>(LastCameraRotator.Yaw);
		const float EasedLerpYaw = FMath::InterpEaseOut(LastYaw, LastYaw + ParryDeathblowYawToBeAdded,
			ParryDeathblowTimer / ParryDeathblowAnimDuration, ArmAnimEaseExponent);
		// set rotation
		const FRotator EasedLerpRotator = FRotator(EasedLerpPitch, EasedLerpYaw, 0);
		GetController()->SetControlRotation(EasedLerpRotator);
	}

	// reset timer
	if (ResetCameraTimer < ResetCameraTime)
	{
		ResetCameraTimer += DeltaTime;

		if (ResetCameraTimer >= ResetCameraTime)
		{
			ResetArmLengthAndLocation();
		}
	}
}

void APlayerCharacter::StartResetArm(const float ResetTime)
{
	ResetCameraTime = ResetTime;
	ResetCameraTimer = 0;
}

void APlayerCharacter::ResetArmLengthAndLocation()
{
	SetArmLengthAnim(ArmDefaultLength, ResetCameraDuration);
	SetArmLocationAnim(ArmDefaultLocation, ResetCameraDuration);
	// カメラリセット時にロックオン可能に戻す
	LookToLockOnTarget = true;
}

void APlayerCharacter::LockOnEnemy()
{
	// LockOn中なら外すだけ
	if (IsLocked())
	{
		LockOnTarget->OnLockOned = false;
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

			// Cast失敗、またはロックオン不可ならcontinue
			if (!Enemy || !Enemy->IsTargetable()) continue;
			
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

		// ロックオン成功した時の処理
		if (LockOnTarget.IsValid())
		{
			LockOnTarget->OnLockOned = true;
			LookToLockOnTarget = true;
		}
	}
}

void APlayerCharacter::LockOnCameraControl(const float DeltaTime)
{
	// 対象がいないなら何もしない
	if (!LockOnTarget.IsValid()) return;

	// 対象がロックオン不可になった、またはロックオン可能な最大距離を超えたらロックオンを解除し終了
	if (!LockOnTarget->IsTargetable() ||
		FVector::DistSquared(LockOnTarget->GetActorLocation(), GetActorLocation()) > MaxLockOnRange * MaxLockOnRange)
	{
		LockOnEnemy();
		return;
	}

	// 特定の状態ならロックオンでカメラ制御しない
	if (!LookToLockOnTarget) return;

	// 現在の正面ベクトル
	FVector CurrentForward = GetController()->GetControlRotation().Vector();
	// ターゲットへの方向ベクトル
	FVector TargetVector = ((LockOnTarget->GetActorLocation() - SpringArm->GetComponentLocation()).Rotation() + LockOnOffsetRotation).Vector();
	TargetVector.Normalize();
	// 現在の向きからターゲットへのなす角
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CurrentForward, TargetVector)));
	// Slerpをかける
	FVector SlerpedVector = FVector::SlerpNormals(CurrentForward, TargetVector, FMath::Clamp(LockOnCameraRotationSpeed * DeltaTime / Angle, 0, 1));
	
	// Offsetを加算しながらコントローラーにセットする
	GetController()->SetControlRotation(SlerpedVector.Rotation());
}
