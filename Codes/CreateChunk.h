// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreateChunk.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFINANTROUND_API UCreateChunk : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreateChunk();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int iRenderRange;
	int iVoxeSize;
	int iChunkLineElements;
	int iChunkSize;
	int iChunkSizeHalf;
	int iChunkX;
	int iChunkY;
	FVector vCharactorPosition;
	//TArray<AActor> Chunks;
	TArray<FVector2D> ChunkCords;
	//FVector2D< FVector<int> >ChunkCords;

	//to set position
	void SetCharactorPossition();

	//in which chunk?
	bool IsDiffrendChunk();

	//loop for create chunk
	void AddChunk();

	//check radius
	bool CheckRadius(float X, float Y, int iMultiplyRadius);

	//check the piont chunk dos in Chunks
	bool CheckIsChunkInChunks(int i, int j);
		
};
