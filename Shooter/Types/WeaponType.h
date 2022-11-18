#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SMG UMETA(DisplayName = "SMG"),
	EWT_AR UMETA(DisplayName = "AR"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),

	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};