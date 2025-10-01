// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class COOPGAME_API Utils
{
public:
	Utils();
	~Utils();

	static TArray<int8> GenerateCode(int8);
	static void DebugShowCode(TArray<int8>&, FString);

public:
	template <typename T>
	static bool CheckEquals(const TArray<T>& input, const TArray<T>& solution)
	{
		if (input.Num() != solution.Num())
		{
			return false;
		}

		for (int i = 0; i < input.Num(); ++i)
		{
			if (input[i] != solution[i])
			{
				return false;
			}
		}
		return true;
	}
	template <typename T>
	static TArray<int8> ConvertArrayToInt8(const TArray <T>& Array)
	{
		TArray<int8> Temp;
		for (int8 i = 0; i < Array.Num(); ++i)
		{
			Temp.Add(static_cast<int8>(Array[i])); 

		}
		return Temp;
	}
};
