#include "Character/PlayerCamera.h"
#include "Character/EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCamera::APlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArmComponent->SetWorldRotation(PlayerController->GetControlRotation());
	FollowOwnerPlayer(DeltaTime);
	LockOnCameraControl(DeltaTime);
	CameraAnimControl(DeltaTime);
}

void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	// get owner
	OwnerPlayer = GetOwner();
	if (!OwnerPlayer) UE_LOG(LogTemp, Warning, TEXT("Failed to GetOwner"));
	
	FindMyComponents();
	// get player controller
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to PlayerController"));
	}
	else
	{
		// このカメラを有効にする
		PlayerController->SetViewTarget(this);
	}
}

void APlayerCamera::FindMyComponents()
{
	SpringArmComponent = FindComponentByClass<USpringArmComponent>();
	CameraComponent = FindComponentByClass<UCameraComponent>();

	if (!SpringArmComponent || !CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find Component"));
		return;
	}

	// 初期値をデフォルトとして保存
	DefaultArmLocation = SpringArmComponent->GetComponentLocation();
	DefaultArmLength = SpringArmComponent->TargetArmLength;
}

void APlayerCamera::FollowOwnerPlayer(float DeltaTime)
{
	const FVector OffsetVector = OwnerPlayer->GetActorRotation().RotateVector(TargetLocationOffset);
	const float DistanceMag = FVector::Distance(OwnerPlayer->GetActorLocation() + OffsetVector, GetActorLocation());
	// Curveに反映させる
	const float Alpha = CameraAnimCurve->GetFloatValue(FMath::Clamp(DistanceMag / MaxFollowSpeedDistance, 0.f, 1.f));
	
	SetActorLocation(GetActorLocation() + FollowOwnerSpeed * Alpha * DeltaTime * (OwnerPlayer->GetActorLocation() + OffsetVector - GetActorLocation()));
}

void APlayerCamera::LockOnCameraControl(const float DeltaTime)
{
	// 対象がいないなら何もしない
	if (LockOnTarget == nullptr) return;

	// 対象がロックオン不可になった、またはロックオン可能な最大距離を超えたらロックオンを解除し終了
	if (!LockOnTarget->IsTargetable() ||
		FVector::DistSquared(LockOnTarget->GetTargetLocation(), GetActorLocation()) > MaxLockOnRange * MaxLockOnRange)
	{
		LockOn();
		return;
	}

	// 特定の状態ならロックオンでカメラ制御しない
	if (!LookToLockOnTarget) return;

	// 現在の正面ベクトル
	FVector CurrentForward = PlayerController->GetControlRotation().Vector();
	// ターゲットへの方向ベクトル
	FVector TargetVector = ((LockOnTarget->GetTargetLocation() - SpringArmComponent->GetComponentLocation()).Rotation() + LockOnOffsetRotation).Vector();
	TargetVector.Normalize();
	// 現在の向きからターゲットへのなす角
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CurrentForward, TargetVector)));
	FVector SlerpedVector;
	
	if (Angle >= MaxRotationSpeedAngle)
	{
		// 最大回転速度
		SlerpedVector = FVector::SlerpNormals(CurrentForward, TargetVector,
			FMath::Clamp(LockOnCameraRotationMaxSpeed * DeltaTime / Angle, 0, 1));
	}
	else
	{
		const float CurveValue = CameraAnimCurve->GetFloatValue(Angle / MaxRotationSpeedAngle);
		SlerpedVector = FVector::SlerpNormals(CurrentForward, TargetVector,
			FMath::Clamp(LockOnCameraRotationMaxSpeed * CurveValue * DeltaTime / Angle, 0, 1));
	}
	
	// Offsetを加算しながらコントローラーにセットする
	PlayerController->SetControlRotation(SlerpedVector.Rotation());
}

void APlayerCamera::LockOn()
{
	// LockOn中なら外すだけ
	if (IsLocked())
	{
		LockOnTarget->SetIsTargeting(false);
		LockOnTarget = nullptr;
		LockOnTargetActor = nullptr;
		return;
	}

	// プレイヤーを中心に球状に敵オブジェクトを取得
	TArray<AActor*> OverlappingActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	TargetObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	const FVector CurrentCameraLocation = CameraComponent->GetComponentLocation();

	// 取得出来た場合、角度を判定する
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CurrentCameraLocation, MaxLockOnRange, TargetObjectTypes,
			AActor::StaticClass(), TArray<AActor*>(), OverlappingActors))
	{
		// 現在のカメラの正面ベクトル
		const FVector ControlForward = FRotationMatrix(PlayerController->GetControlRotation()).GetUnitAxis(EAxis::X);
		
		float SmallestAngle = 0.0f;
		// 最も正面にいるターゲット
		IITargetableInterface* SmallestAngleTarget = nullptr;
		AActor* SmallestAngleTargetActor = nullptr;

		// 範囲内のActorをループ
		for (const TObjectPtr<AActor> Actor : OverlappingActors)
		{
			IITargetableInterface* Target = Cast<IITargetableInterface>(Actor);

			// Cast失敗、またはロックオン不可ならcontinue
			if (!Target || !Target->IsTargetable()) continue;

			// 正面からのなす角
			const float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ControlForward,
				(Target->GetTargetLocation() - CurrentCameraLocation).GetSafeNormal())));

			// 最初の敵、またはなす角が最も小さければ入れ替える
			if ((!SmallestAngleTarget || SmallestAngle > AngleDegrees) && AngleDegrees < MaxLockOnAngle)
			{
				SmallestAngleTarget = Target;
				SmallestAngleTargetActor = Actor;
				SmallestAngle = AngleDegrees;
			}
		}

		LockOnTarget = SmallestAngleTarget;
		LockOnTargetActor = SmallestAngleTargetActor;

		// ロックオン成功した時の処理
		if (LockOnTarget)
		{
			LockOnTarget->SetIsTargeting(true);
			LookToLockOnTarget = true;
		}
	}
}

const AActor* APlayerCamera::GetLockedOnTargetActor() const
{
	return LockOnTargetActor;
}

void APlayerCamera::SetArmLengthAnim(const float RelativeValue, const float Duration)
{
	TargetLength = DefaultArmLength + RelativeValue;
	LastLength = SpringArmComponent->TargetArmLength;
	LengthAnimDuration = Duration;
	LengthAnimTimer = 0;
}

void APlayerCamera::SetArmLocationAnim(const FVector RelativeValue, const float Duration)
{
	TargetLocation = RelativeValue;
	LastLocation = TargetLocationOffset;
	LocationAnimDuration = Duration;
	LocationAnimTimer = 0;
}

void APlayerCamera::CameraAnimControl(const float DeltaTime)
{
	// TargetArmLengthのアニメーション
	if (LengthAnimTimer < LengthAnimDuration) // 維持時間中
	{
		LengthAnimTimer += DeltaTime;

		if (LengthAnimTimer <= CameraAnimTime) // アニメーション中
		{
			// カーブを掛けて適用する
			const float Alpha = CameraAnimCurve->GetFloatValue(LengthAnimTimer / CameraAnimTime);
			SpringArmComponent->TargetArmLength = LastLength + (TargetLength - LastLength) * Alpha;
		}
		else
		{
			SpringArmComponent->TargetArmLength = TargetLength;
		}

		// 維持時間終了時にデフォルトに戻ってなかったらリセットを掛ける
		if (LengthAnimTimer >= LengthAnimDuration &&
			!FMath::IsNearlyEqual(DefaultArmLength, SpringArmComponent->TargetArmLength))
		{
			SetArmLengthAnim(0, CameraAnimTime);
		}
	}

	// ArmLocationのアニメーション
	if (LocationAnimTimer < LocationAnimDuration)
	{
		LocationAnimTimer += DeltaTime;

		if (LocationAnimTimer <= CameraAnimTime) // アニメーション中
		{
			const float Alpha = CameraAnimCurve->GetFloatValue(LocationAnimTimer / CameraAnimTime);
			TargetLocationOffset = LastLocation + Alpha * (TargetLocation - LastLocation);
		}
		else
		{
			TargetLocationOffset = TargetLocation;
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *TargetLocationOffset.ToString());

		// 維持時間終了時にデフォルトに戻ってなかったらリセットする
		if (!FMath::IsNearlyEqual(0.f, TargetLocationOffset.Length()))
		{
			SetArmLocationAnim(FVector(0, 0, 0), CameraAnimTime);
		}
	}
}
