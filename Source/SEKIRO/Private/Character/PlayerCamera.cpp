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
	const FVector DistanceToOwner = OwnerPlayer->GetActorLocation() - GetActorLocation();
	//const float Distance = DistanceToOwner.Length();
	SetActorLocation(GetActorLocation() + FollowOwnerSpeed * DeltaTime * DistanceToOwner);
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
	// Slerpをかける
	FVector SlerpedVector = FVector::SlerpNormals(CurrentForward, TargetVector, FMath::Clamp(LockOnCameraRotationSpeed * DeltaTime / Angle, 0, 1));
	
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
		UE_LOG(LogTemp, Log, TEXT("ロックオン解除"));
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

			UE_LOG(LogTemp, Log, TEXT("%f, %f"), AngleDegrees, FVector::Distance(Target->GetTargetLocation(), CurrentCameraLocation));

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
			UE_LOG(LogTemp, Log, TEXT("ロックオン成功"));
		}
	}
}

const AActor* APlayerCamera::GetLockedOnTargetActor() const
{
	return LockOnTargetActor;
}
