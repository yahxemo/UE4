#pragma once

#include "Components/ActorComponent.h"

class MyChunks
{
	public:
	FVector vChunkVector;
	FRotator rChunkRotator;
	AActor* aChunkActor;
	int iLable;//1=Cube  2=Triangle  3=TriangleConnect
	MyChunks* next;
};