// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ReadInformation.h"
#include "ActorsSpawns.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "NewActorComponent.generated.h"

typedef struct MyChunks
{
	//int iIndex;
	//MyChunks();
	FVector vChunkVector;//位置
	FRotator rChunkRotator;//角度
	AActor* aChunkActor;//方块指针
	bool bAddMoreWall;//完全匹配
	int iLable;//1=Cube  2=Triangle  3=TriangleConnect
	struct MyChunks* next;
	FCriticalSection MyCritialSection;
	/*void InitList(MyChunks &c)
	{
		c = (MyChunks*)malloc(sizeof(MyChunks));
	}*/
};

/*
class MyChunks
{
public:
	MyChunks(FVector vector, FRotator rotator, AActor* actor,int index);
	int iIndex;
	//MyChunks();
	FVector vChunkVector;
	FRotator rChunkRotator;
	AActor* aChunkActor;
	MyChunks* next;

private:

};

MyChunks::MyChunks(FVector vector, FRotator rotator, AActor* actor, int index)
{
	vChunkVector = vector;
	rChunkRotator = rotator;
	aChunkActor = actor;
	next = NULL;
}



class MyChunks
{
public:
	MyChunks(FVector vector, FRotator rotator, AActor* actor);
	int iIndex;
	//MyChunks();
	FVector vChunkVector;
	FRotator rChunkRotator;
	AActor* aChunkActor;
	MyChunks* next;
	void InitList(MyChunks &c)
	{
		c = (MyChunks*)malloc(sizeof(MyChunks));
	}
	
};

MyChunks::MyChunks(FVector vector, FRotator rotator, AActor* actor)
{
	vChunkVector = vector;
	rChunkRotator = rotator;
	aChunkActor = actor;
	next = NULL;
}

class MyChunks
{
public:
	
};
*/



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFINANTROUND_API UNewActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNewActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//AActor* playercontroller;


	//loop for create chunk
	bool AddChunk();

	//lock
	FCriticalSection MyMianCritialSection;


	//take back actor from 所选方块
	UPROPERTY(EditAnywhere, Category="ChoiseActor")
		TSubclassOf<AActor> Triangle;
	UPROPERTY(EditAnywhere, Category = "ChoiseActor")
		TSubclassOf<AActor> TriangleConnect;
	UPROPERTY(EditAnywhere, Category = "ChoiseActor")
		TSubclassOf<AActor> Cube;


	MyChunks* MyChunkHead;//方块链表头

private:
	///---
	//FPrimeNumberWorker* pThread;
	///----

	int iRenderRange;//扫描半斤
	int iVoxeSize;//
	int iChunkLineElements;//加上面就是iChunkSize
	int iChunkSize;//方块边
	int iChunkSizeHalf;
	int iChunkX;//角色xy
	int iChunkY;

	ReadInformation* ReadInfo;//信息模块
	FVector vCharactorPosition;//角色位置
	//TArray<AActor*> Chunks;
	//TArray<FVector> ChunkCords;
	//FVector2D<FVector<int> >ChunkCords;

	//thread multiple
	TArray < uint32 >  PrimeNumbers;

	//to set position
	bool SetCharactorPossition();

	//in which chunk?
	bool IsDiffrendChunk();


	bool CheckIsAddMoreWall(int i, int j);
	///check radius
	///bool CheckRadius(float X,float Y,int iMultiplyRadius);

	///Look for MyChunks class
	///bool CheckChunksClass(FVector CheckVector);

	//check is exsist?
	MyChunks* CheckExsist(MyChunks* ChunkHead,int i, int j);

	//ChoiseReadInfo   which chunk?_Cube?_Triangle?_TriangleConnect?	which part?_ZERO?ONE?TWO?THREE? 
	ActorsSpawns* ChoiseReadInfo(MyChunks* Chunk, int i, int j);

	ActorsSpawns* SetAndRAndomChoiseACtorSpawn(MyChunks* ChoiseChunk, int x, int y);

	//check i+-1  j+-1 surrounding and return
	MyChunks* CheckExsistSurrounding(MyChunks* ChunkHead, int i,int j,int* pBackCheckTime = nullptr, int* pBackLowHigh = nullptr);

	//add chunk where
	UFUNCTION()
		bool SpawnChunk(int i,int j);

	//MyChunks* FindEnd(MyChunks* Chunk);

	//void SetVisible();
};

