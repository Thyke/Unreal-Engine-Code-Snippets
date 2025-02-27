#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

/**
 * Bitflags representing the character's status
 * These flags are used to indicate the current state of the character
 */
UENUM(BlueprintType, meta = (Bitflags), meta = (Bitmask, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EStatusFlags : uint8
{
  None         = 0       UMETA(DisplayName = "None"),
  IsAction     = 1 << 0  UMETA(DisplayName = "Performing Action"),
  IsHiding     = 1 << 1  UMETA(DisplayName = "Hiding"), 
  IsInjured    = 1 << 2  UMETA(DisplayName = "Injured"),
  IsPanicking  = 1 << 3  UMETA(DisplayName = "Panicking"),
  IsInCinematic= 1 << 4  UMETA(DisplayName = "In Cinematic"),
  IsDead       = 1 << 5  UMETA(DisplayName = "Dead"),
  IsBreathing  = 1 << 6  UMETA(DisplayName = "Breathing"),
  IsSneaking   = 1 << 7  UMETA(DisplayName = "Sneaking")
// Since this Enum is an 8 bit uint8, maximum 8 flags can be defined
// If more flags are needed, the data type should be changed to uint16 or uint32
};
ENUM_CLASS_FLAGS(EStatusFlags);

// Delegate triggered when the component is initialized
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusComponentInitialized);

// Delegates triggered when status flags change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusFlagChanged, EStatusFlags, Flag);

/**
 * StatusComponent - Component that manages the character's current state using bitflags
 * This component is used to track various states of the character (performing action, hiding, injured, etc.)
 * and to perform operations for adding, removing, or querying these states.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAME_API UStatusComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UStatusComponent();

  /** Triggered when the component is successfully initialized */
  UPROPERTY(BlueprintAssignable)
  FOnStatusComponentInitialized OnStatusComponentInitialized;

  /** Triggered when a status flag is added */
  UPROPERTY(BlueprintAssignable, Category = "Status")
  FOnStatusFlagChanged OnStatusFlagAdded;
  
  /** Triggered when a status flag is removed */
  UPROPERTY(BlueprintAssignable, Category = "Status")  
  FOnStatusFlagChanged OnStatusFlagRemoved;

protected:
  virtual void BeginPlay() override;
  
  /** Initializes the component and establishes necessary connections */
  void InitializeStatusComponent();

public:
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  /** Current status flags of the character */
  UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (Bitmask, BitmaskEnum = "EStatusFlags"))
  uint8 StatusFlags;

  /**
   * Adds a single status flag
   * @param Flag - The status flag to add
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void AddStatusFlag(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) EStatusFlags Flag);

  /**
   * Clears a single status flag
   * @param Flag - The status flag to clear
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void ClearStatusFlag(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) EStatusFlags Flag);

  /**
   * Adds multiple status flags
   * @param FlagsToAdd - Bitmask of status flags to add
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void AddStatusFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToAdd);

  /**
   * Removes multiple status flags
   * @param FlagsToRemove - Bitmask of status flags to remove
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void RemoveStatusFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToRemove);

  /**
   * Toggles status flags (removes if present, adds if not)
   * @param FlagsToToggle - Bitmask of status flags to toggle
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void ToggleStatusFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToToggle);

  /**
   * Simultaneously adds some flags and removes others
   * @param FlagsToAdd - Bitmask of status flags to add
   * @param FlagsToRemove - Bitmask of status flags to remove
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void ModifyStatusFlags(
      UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToAdd,
      UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToRemove
  );

  /**
   * Clears all status flags
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void ClearAllStatusFlags();

  /**
   * Checks if the character has all specified status flags
   * @param FlagsToCheck - Bitmask of status flags to check
   * @return True: Has all flags, False: At least one flag is missing
   */
  UFUNCTION(BlueprintPure, Category = "Status")
  bool HasAllFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToCheck) const;

  /**
   * Checks if the character has any of the specified status flags
   * @param FlagsToCheck - Bitmask of status flags to check
   * @return True: Has at least one flag, False: Has none of the flags
   */
  UFUNCTION(BlueprintPure, Category = "Status")
  bool HasAnyFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 FlagsToCheck) const;

  /**
   * Checks if the character has a specific status flag
   * @param Flag - The status flag to check
   * @return True: Has the flag, False: Does not have the flag
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  bool HasStatusFlag(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) EStatusFlags Flag) const;

  /**
   * Checks if the character has any of the specified status flags
   * @param Flags - The status flags to check
   * @return True: Has at least one flag, False: Has none of the flags
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  bool HasAnyStatusFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) EStatusFlags Flags) const;

  /**
   * Checks if the character has all of the specified status flags
   * @param Flags - The status flags to check
   * @return True: Has all flags, False: At least one flag is missing
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  bool HasAllStatusFlags(UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) EStatusFlags Flags) const;

  /**
   * Checks status conditions required to perform an action
   * @param MustHaveFlags - Flags that must be present
   * @param MustNotHaveFlags - Flags that must not be present
   * @return True: Action can be performed, False: Action cannot be performed
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  bool CanPerformAction(
      UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 MustHaveFlags,
      UPARAM(meta = (Bitmask, BitmaskEnum = "EStatusFlags")) int32 MustNotHaveFlags
  ) const;

  /**
   * Adds a status flag for a specific duration and automatically removes it after the duration expires
   * @param Flag - The status flag to add
   * @param Duration - Duration (in seconds) for which the flag will remain active
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  void AddTimedStatusFlag(EStatusFlags Flag, float Duration);

  /**
   * Returns active status flags as a readable string
   * @return Comma-separated string of active flags
   */
  UFUNCTION(BlueprintCallable, Category = "Status")
  FString GetActiveFlagsAsString() const;

private:
  /**
   * Checks if the flag value is a valid single-bit flag
   * @param Flag - The flag to check
   * @return True: Valid flag, False: Invalid flag
   */
  bool IsValidFlag(EStatusFlags Flag) const;

private:
  /** Character that owns this component */
  UPROPERTY()
  TObjectPtr<class ACharacter> OwnerCharacter;
  
};