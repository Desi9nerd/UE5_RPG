#include "Components/CTargetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Global.h"
#include "Characters/CBaseCharacter.h"
#include "GameFramework/Controller.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Character = Cast<ACBaseCharacter>(GetOwner());
	if (Character.IsValid())
	{
		Controller = Cast<AController>(Character->GetController());
	}
} 

void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(Target);

	float distanceToTarget = Character->GetDistanceTo(&(*Target));

	if (distanceToTarget <= TraceDistance)
	{
		Tick_Targeting();
	}
	else
	{
		End_Targeting();
	}
}

void UCTargetComponent::Toggle_Target()
{
	if (bTargeting)
	{
		End_Targeting();
	}
	else
	{
		Begin_Targeting();
	}
}

void UCTargetComponent::Begin_Targeting()
{
	const TArray<AActor*> ignores{ Character.Get() };
	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),
		Character->GetActorLocation(),
		Character->GetActorLocation(),
		TraceDistance,
		"Targeting",
		false,
		ignores,
		EDrawDebugTrace::None,
		hitResults,
		true);

	TArray<TWeakObjectPtr<ACBaseCharacter>> Targets;

	for (const auto& i : hitResults)
	{
		if (i.GetActor()->GetClass() != Character->GetClass())
		{
			Targets.Push(Cast<ACBaseCharacter>(i.GetActor()));
			bTargeting = true;
		}
	}

	const TWeakObjectPtr<ACBaseCharacter> target = NearlyForward(Character, Targets, Character->GetController());

	ChangeTarget(target.Get());
}

void UCTargetComponent::End_Targeting()
{
	CheckNull(Target);

	if(IsValid(Particle))
	{
		Particle->DestroyComponent();
	}

	//TargetCameraComp->DisableFixedCamera();
	Target = nullptr;
	bTargeting = false;
}

void UCTargetComponent::ChangeTarget(ACBaseCharacter* InCandidate)
{
	if (IsValid(InCandidate))
	{
		if (IsValid(Particle))
		{
			Particle->DestroyComponent();
		}


		Target = InCandidate;

		if (Target.IsValid())
		{
			Particle = UGameplayStatics::SpawnEmitterAttached(
				ParticleAsset,
				Target->GetMesh(),
				"Root",
				FVector(0, 0, -10),
				FRotator(0.0f, 0.0f, 0.0f),
				FVector(0.5f, 0.5f, 0.25f),
				EAttachLocation::KeepRelativeOffset,
				true,
				EPSCPoolMethod::None,
				true);

			//if (TargetCameraComp.IsValid())
				//TargetCameraComp->EnableFixedCamera();
		}
	}
	else
	{
		bTargeting = false;
		End_Targeting();
	}
}

void UCTargetComponent::Tick_Targeting()
{
	CheckNull(Target);

	bool bIsValid = true;
	bIsValid &= (true == (Character->GetDistanceTo(Target.Get()) <= TraceDistance));

	if (false == bIsValid)
	{
		End_Targeting();
		return;
	}

	if (nullptr == Controller.Get())
		Controller = Character->GetController();

	const FRotator controlRotation = Controller->GetControlRotation();
	const FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(
		Character->GetActorLocation(), Target->GetActorLocation());

	if (true == UKismetMathLibrary::EqualEqual_RotatorRotator(
		controlRotation, ownerToTarget, FinishAngle))
	{
		Controller->SetControlRotation(ownerToTarget);

		if (true == bMovingFocus)
			bMovingFocus = false;
	}
	else
	{
		const FRotator rotator = UKismetMathLibrary::RInterpTo(
			controlRotation,
			ownerToTarget,
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			InterpSpeed);

		Controller->SetControlRotation(rotator);
	}
}

void UCTargetComponent::ChangeFocus(bool InbRight)
{
	CheckNull(Target);

	const TArray<AActor*> ignores{ Character.Get(), Target.Get() };
	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),
		Character->GetActorLocation(),
		Character->GetActorLocation(),
		TraceDistance,
		"Targeting",
		false,
		ignores,
		EDrawDebugTrace::None,
		hitResults,
		true);

	TArray<TWeakObjectPtr<ACBaseCharacter>> targets;

	CheckTrue(hitResults.Num() == 0);

	for (const auto& elem : hitResults)
	{
		bool bCheck = true;
		bCheck &= IsValid(elem.GetActor());
		bCheck &= (elem.GetActor()->GetClass() != Character->GetClass());

		if (bCheck)
		{
			targets.AddUnique(Cast<ACBaseCharacter>(elem.GetActor()));
			bTargeting = true;
		}
	}

	TMap<float, TWeakObjectPtr<ACBaseCharacter>> nearCharacters;

	AddNearSideCharacters(Character,targets,Character->GetController(), nearCharacters);

	float minAngle = 1e9;

	TWeakObjectPtr<ACBaseCharacter> candidate;

	for (const auto& i : nearCharacters)
	{
		const float currAngle = i.Key;

		bool bCheck = false;
		bCheck |= (InbRight && currAngle > 0.0f);
		bCheck |= (false == InbRight && currAngle < 0.0f);
		bCheck &= (minAngle > abs(currAngle));

		if (bCheck)
		{
			minAngle = abs(currAngle);
			candidate = i.Value;
		}
	}

	if (candidate.IsValid())
	{
		if (false == bMovingFocus)
		{
			bMovingFocus = true;
			ChangeTarget(candidate.Get());
		}
	}
}

//////////////////////////////////////////////////////////////////////

TWeakObjectPtr<ACBaseCharacter> UCTargetComponent::NearlyForward(TWeakObjectPtr<ACBaseCharacter> InCenterTarget,
	TArray<TWeakObjectPtr<ACBaseCharacter>> InArray, TWeakObjectPtr<AController> InController)
{
	CheckTrueResult(InArray.Num() <= 0, nullptr);

	const float limitLeastAngle = 0.7f;
	float maxAngle = 0.0f;
	TWeakObjectPtr<ACBaseCharacter> outTarget;

	for (const auto& otherCharacter : InArray)
	{
		if (otherCharacter.IsValid())
		{
			FVector diff = (otherCharacter->GetActorLocation() - InCenterTarget->GetActorLocation());
			diff.Normalize();

			const FVector forward = UKismetMathLibrary::GetForwardVector(InController->GetControlRotation());

			float curAngle = diff.DotProduct(diff, forward);

			if (curAngle > limitLeastAngle && curAngle > maxAngle)
			{
				curAngle = maxAngle;
				outTarget = otherCharacter;
			}
		}
	}

	return outTarget;
}

void UCTargetComponent::AddNearSideCharacters(TWeakObjectPtr<ACBaseCharacter> InCenter,
	TArray<TWeakObjectPtr<ACBaseCharacter>> InArray, TWeakObjectPtr<AController> InController,
	TMap<float, TWeakObjectPtr<ACBaseCharacter>>& OutNearCharacters)
{
	CheckTrue(InArray.Num() <= 0);

	for (auto& otherCharacter : InArray)
	{
		if (otherCharacter.IsValid())
		{
			FVector toTarget = (otherCharacter->GetActorLocation() - InCenter->GetActorLocation());
			toTarget.Normalize();

			const FVector forward = UKismetMathLibrary::GetForwardVector(InController->GetControlRotation());

			const FVector up = InCenter->GetActorUpVector();

			const float direction = UKismetMathLibrary::Dot_VectorVector(up, UKismetMathLibrary::Cross_VectorVector(forward, toTarget));

			OutNearCharacters.Add(direction, otherCharacter);
		}
	}
}