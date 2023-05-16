// Bu Notify sınıfı, animasyon blueprint'inde ayak sesi ve parçacık efekti oluşturmak için kullanılır.
/**
@ Thyke
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "EternityGameInstance.h"
#include "FootStepNotify.generated.h"


USTRUCT(BlueprintType)
struct FSurfaceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Data")
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Data")
	USoundBase* SoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Data")
	UNiagaraSystem* NiagaraSystem;
};

UCLASS()
class ETERNITY_API UFootStepNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UFootStepNotify();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UFUNCTION(BlueprintCallable)
	void PlaySoundAndSpawnParticles(USkeletalMeshComponent* MeshComp, USoundBase* SoundCue, UNiagaraSystem* NiagaraSystem, FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Surface Data")
	TArray<FSurfaceData> SurfaceDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Settings")
	FName FootSocketName;

	UFUNCTION()
	void LineTraceFootstepSoundAndParticles(USkeletalMeshComponent* MeshComp);
};
