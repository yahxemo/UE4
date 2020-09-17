// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateChunk.h"


#include"Engine.h"
#include"Engine/World.h"
#include"GameFramework/PlayerController.h"
#include"Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

// Sets default values for this component's properties
UCreateChunk::UCreateChunk()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCreateChunk::BeginPlay()
{
	Super::BeginPlay();

	// ...
	iChunkLineElements = 100;
	iVoxeSize = 100;
	iChunkSize = iChunkLineElements * iVoxeSize;
	iChunkSizeHalf = iChunkSize / 2;
	iRenderRange = 3;
}


// Called every frame
void UCreateChunk::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	SetCharactorPossition();
	//IsDiffrendChunk();
	AddChunk();
}

//to set position
void UCreateChunk::SetCharactorPossition()
{
	APawn *Controller = UGameplayStatics().GetPlayerPawn(GetWorld(), 0);
	//set charactorposition
	vCharactorPosition.X = Controller->GetActorLocation().X;
	vCharactorPosition.Y = Controller->GetActorLocation().Y;
	vCharactorPosition.Z = 0;
}

//in which chunk?
bool UCreateChunk::IsDiffrendChunk()
{
	if (iChunkX != floorf(vCharactorPosition.X / iChunkSizeHalf) || iChunkY != floorf(vCharactorPosition.Y / iChunkSizeHalf))
	{
		iChunkX = floorf(vCharactorPosition.X / iChunkSizeHalf);
		iChunkY = floorf(vCharactorPosition.Y / iChunkSizeHalf);
		return true;
	}
	else
		return false;
}

//loop for create chunk
void UCreateChunk::AddChunk()
{
	for (int i = -iRenderRange; i < iRenderRange; i++)
	{
		for (int j = -iRenderRange; j < iRenderRange; j++)
		{
			CheckIsChunkInChunks(i, j);
			//Chunks.Add(GetWorld()->SpawnActor<test>());
			//FVector vec=FVector((iChunkX + i)*iChunkSize + iChunkSizeHalf);
			//ChunkCords[1].Contains();
			//ChunkCords[1] = FVector2D();
		}
	}
}
//check radius
bool UCreateChunk::CheckRadius(float X, float Y, int iMultiplyRadius)
{
	//	vCharactorPosition.Size2D;
	return false;
}

bool UCreateChunk::CheckIsChunkInChunks(int i, int j)
{
	FVector2D vec = FVector2D(iChunkX + i, iChunkY + j);
	int32 iIndex;
	if (ChunkCords.Find(vec, iIndex))
	{
		return true;
	}
	else
	{
		ChunkCords.Add(vec);
		return false;
	}

}

