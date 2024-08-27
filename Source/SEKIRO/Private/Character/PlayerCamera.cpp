#include "Character/PlayerCamera.h"

APlayerCamera::APlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArmComponent->SetWorldRotation(PlayerController->GetControlRotation());
	FollowOwnerPlayer(DeltaTime);
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
