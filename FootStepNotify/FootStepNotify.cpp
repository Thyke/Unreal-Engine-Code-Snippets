// Bu Notify sınıfı, animasyon blueprint'inde ayak sesi ve parçacık efekti oluşturmak için kullanılır.
/**
@ Thyke
*/


#include "FootStepNotify.h"
#include "Sound/SoundCue.h"
#include <Kismet/GameplayStatics.h>
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UFootStepNotify::UFootStepNotify()
{

}
void UFootStepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !Animation)
    {
        return;
    }
    LineTraceFootstepSoundAndParticles(MeshComp);
}

void UFootStepNotify::PlaySoundAndSpawnParticles(USkeletalMeshComponent* MeshComp, USoundBase* SoundCue, UNiagaraSystem* NiagaraSystem, FVector Location)
{
    if (SoundCue)
    {
        UGameplayStatics::PlaySoundAtLocation(MeshComp, SoundCue, Location);
    }
    if (NiagaraSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), NiagaraSystem, Location);
    }
}

void UFootStepNotify::LineTraceFootstepSoundAndParticles(USkeletalMeshComponent* MeshComp)
{
    // Mesh component'in sahibi olan karakter alınır
    AActor* Owner = MeshComp->GetOwner();
    // Eğer sahibi yoksa, işlem sonlandırılır
    if (Owner == nullptr)
    {
        return;
    }
    // Skeletal mesh'in socket pozisyonu alınır(foot_l_socket gibi)
    FVector SocketLocation = MeshComp->GetSocketLocation(FootSocketName);

    // Line trace için hedef pozisyonu ayarla (socket pozisyonunun 50 birim altında, bu değeri artırabiliriz.)
    FVector TargetLocation = SocketLocation - FVector(0, 0, 50.0f);

    // Line trace için çarpışma sorgusu parametreleri ayarla
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    QueryParams.bTraceComplex = false;
    QueryParams.bReturnPhysicalMaterial = true;

    // Socket pozisyonundan hedef pozisyona line trace yap
    // Eğer çarpışma olursa, çarpışma bilgilerini HitResult değişkenine ata
    FHitResult HitResult;
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, SocketLocation, TargetLocation, ECC_Visibility, QueryParams);
    if (bHit)
    {
        UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
        EPhysicalSurface SurfaceType = SurfaceType_Default;
        if (PhysMaterial)
        {
            SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMaterial);
            // Loop through SurfaceData array to find a matching surface
            for (const FSurfaceData& SurfaceData : SurfaceDataTable)
            {
                if (SurfaceData.SurfaceType == SurfaceType)
                {
                    // Play corresponding sound and spawn particles
                    PlaySoundAndSpawnParticles(MeshComp, SurfaceData.SoundCue, SurfaceData.NiagaraSystem, HitResult.Location);
                    break;
                }
            }
        }
    }
}