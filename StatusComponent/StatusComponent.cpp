#include "StatusComponent.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StatusComponent)

UStatusComponent::UStatusComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
   StatusFlags = 0;
}

void UStatusComponent::BeginPlay()
{
   Super::BeginPlay();
   InitializeStatusComponent();
}

void UStatusComponent::InitializeStatusComponent()
{
   OwnerCharacter = Cast<ACharacter>(GetOwner());
   if (OwnerCharacter)
   {

   }
   OnStatusComponentInitialized.Broadcast();
}

void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Single flag operations
void UStatusComponent::AddStatusFlag(EStatusFlags Flag)
{
   if (!IsValidFlag(Flag)) return;

   const uint8 FlagValue = static_cast<uint8>(Flag);
   if (!HasStatusFlag(Flag))
   {
       StatusFlags |= FlagValue;
       OnStatusFlagAdded.Broadcast(Flag);
   }
}

void UStatusComponent::ClearStatusFlag(EStatusFlags Flag)
{
   if (!IsValidFlag(Flag)) return;

   const uint8 FlagValue = static_cast<uint8>(Flag);
   if (HasStatusFlag(Flag))
   {
       StatusFlags &= ~FlagValue;
       OnStatusFlagRemoved.Broadcast(Flag);
   }
}

// Multiple flag operations
void UStatusComponent::AddStatusFlags(int32 FlagsToAdd)
{
   if (FlagsToAdd == 0) return;

   const uint8 OldFlags = StatusFlags;
   StatusFlags |= static_cast<uint8>(FlagsToAdd);

   if (StatusFlags != OldFlags)
   {
       OnStatusFlagAdded.Broadcast(static_cast<EStatusFlags>(FlagsToAdd));
   }
}

void UStatusComponent::RemoveStatusFlags(int32 FlagsToRemove)
{
   if (FlagsToRemove == 0) return;

   const uint8 OldFlags = StatusFlags;
   StatusFlags &= ~static_cast<uint8>(FlagsToRemove);

   if (StatusFlags != OldFlags)
   {
       OnStatusFlagRemoved.Broadcast(static_cast<EStatusFlags>(FlagsToRemove));
   }
}

void UStatusComponent::ToggleStatusFlags(int32 FlagsToToggle)
{
   if (FlagsToToggle == 0) return;

   const uint8 OldFlags = StatusFlags;
   StatusFlags ^= static_cast<uint8>(FlagsToToggle);

   if (StatusFlags != OldFlags)
   {
       const uint8 Added = (~OldFlags) & StatusFlags;
       const uint8 Removed = OldFlags & (~StatusFlags);

       if (Added) OnStatusFlagAdded.Broadcast(static_cast<EStatusFlags>(Added));
       if (Removed) OnStatusFlagRemoved.Broadcast(static_cast<EStatusFlags>(Removed));
   }
}

void UStatusComponent::ModifyStatusFlags(int32 FlagsToAdd, int32 FlagsToRemove)
{
   const uint8 OldFlags = StatusFlags;
   StatusFlags = (OldFlags | static_cast<uint8>(FlagsToAdd)) & ~static_cast<uint8>(FlagsToRemove);

   if (FlagsToAdd && ((OldFlags | static_cast<uint8>(FlagsToAdd)) != OldFlags)) 
   {
       OnStatusFlagAdded.Broadcast(static_cast<EStatusFlags>(FlagsToAdd));
   }
   
   if (FlagsToRemove && ((OldFlags & ~static_cast<uint8>(FlagsToRemove)) != OldFlags)) 
   {
       OnStatusFlagRemoved.Broadcast(static_cast<EStatusFlags>(FlagsToRemove));
   }
}

void UStatusComponent::ClearAllStatusFlags()
{
   if (StatusFlags == 0) return;
   
   const uint8 OldFlags = StatusFlags;
   StatusFlags = 0;
   OnStatusFlagRemoved.Broadcast(static_cast<EStatusFlags>(OldFlags));
}

// Flag checks
bool UStatusComponent::HasAllFlags(int32 FlagsToCheck) const
{
   if (FlagsToCheck == 0) return true;
   return (StatusFlags & static_cast<uint8>(FlagsToCheck)) == static_cast<uint8>(FlagsToCheck);
}

bool UStatusComponent::HasAnyFlags(int32 FlagsToCheck) const
{
   if (FlagsToCheck == 0) return false;
   return (StatusFlags & static_cast<uint8>(FlagsToCheck)) != 0;
}

bool UStatusComponent::HasStatusFlag(EStatusFlags Flag) const
{
   return (StatusFlags & static_cast<uint8>(Flag)) != 0;
}

bool UStatusComponent::HasAnyStatusFlags(EStatusFlags Flags) const
{
   return (StatusFlags & static_cast<uint8>(Flags)) != 0;
}

bool UStatusComponent::HasAllStatusFlags(EStatusFlags Flags) const
{
   return (StatusFlags & static_cast<uint8>(Flags)) == static_cast<uint8>(Flags);
}

// Action validation
bool UStatusComponent::CanPerformAction(int32 MustHaveFlags, int32 MustNotHaveFlags) const
{
   // Check if all required flags are present
   if ((StatusFlags & static_cast<uint8>(MustHaveFlags)) != static_cast<uint8>(MustHaveFlags))
   {
       return false;
   }
   
   // Check if none of the prohibited flags are present
   if ((StatusFlags & static_cast<uint8>(MustNotHaveFlags)) != 0)
   {
       return false;
   }
   
   return true;
}

// Utility functions
void UStatusComponent::AddTimedStatusFlag(EStatusFlags Flag, float Duration)
{
   if (!IsValidFlag(Flag) || Duration <= 0.0f) return;

   AddStatusFlag(Flag);
   FTimerHandle TimerHandle;
   GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Flag]()
   {
       ClearStatusFlag(Flag);
   }, Duration, false);
}

FString UStatusComponent::GetActiveFlagsAsString() const
{
   FString Result;
   for (int32 i = 0; i < 8; ++i)
   {
       const EStatusFlags Flag = static_cast<EStatusFlags>(1 << i);
       if (HasStatusFlag(Flag))
       {
           if (!Result.IsEmpty()) Result += TEXT(", ");
           Result += UEnum::GetDisplayValueAsText(Flag).ToString();
       }
   }
   return Result.IsEmpty() ? TEXT("None") : Result;
}

bool UStatusComponent::IsValidFlag(EStatusFlags Flag) const
{
   const uint8 FlagValue = static_cast<uint8>(Flag);
   // A valid single-bit flag has exactly one bit set to 1
   // We can check this using the formula (n & (n-1)) == 0
   return FlagValue != 0 && (FlagValue & (FlagValue - 1)) == 0;
}