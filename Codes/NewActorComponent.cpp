// Fill out your copyright notice in the Description page of Project Settings.
#include "NewActorComponent.h"
#include"Engine.h"
#include"Engine/World.h"
#include"GameFramework/PlayerController.h"
#include"Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Engine/Blueprint.h"

#include "Async.h"
#include "MyThread.h"
#include <stdio.h>
#include <stdlib.h>

///------------------------------
// Sets default values for this component's properties
UNewActorComponent::UNewActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UNewActorComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...*/

	ReadInfo = new ReadInformation();
	////初始化所有需要的
	//BeginPlay
	iChunkLineElements = 40;
	iVoxeSize = 100;
	iChunkSize = iChunkLineElements * iVoxeSize;//方块儿长
	iChunkSizeHalf = iChunkSize / 2;
	iRenderRange = 20;//更新方块的区块大小
	iChunkX = 0;//
	iChunkY = 0;
	///------------------------------------------------------------------------
	///InitList
	//MyChunk = new MyChunks;
	//UE_LOG(LogTemp, Warning, TEXT("2&MyChunk:(%d)"), MyChunk);
	MyChunkHead = nullptr;
	//AddChunk();
}


// Called every frame
void UNewActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (SetCharactorPossition() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Error,Can't read position"));
	}
	else
	{
		if (IsDiffrendChunk())
		{
			//FPrimeNumberWorker* pThread;
			//FPrimeNumberWorker* pThread =new FPrimeNumberWorker(PrimeNumbers, 50000, GetWorld()->GetFirstPlayerController(),this);
			//pThread->pTarget = this;
			//pThread->JoyInit(PrimeNumbers, 50000,GetWorld()->GetFirstPlayerController());
			//pThread->Init();
			//多线程进行
			//if(FPrimeNumberWorker::IsThreadFinished())
			//while (MyChunkHead->MyCritialSection.TryLock() == false);

			//MyChunkHead->MyCritialSection.Unlock();
			//FPrimeNumberWorker::Shutdown();
			FPrimeNumberWorker::JoyInit(PrimeNumbers, 50000, GetWorld()->GetFirstPlayerController(), this);
			//MyChunkHead->MyCritialSection.Lock();
				//AddChunk();

			//pThread->Run();
			//pThread->Shutdown();
			//FPrimeNumberWorker::pTarget = this;
			//FPrimeNumberWorker::Run();
			//FPrimeNumberWorker::
			//FPrimeNumberWorker::
			//FPrimeNumberWorker::Run();

			//std::thread threadObj(AddChunk);
			//thread task01(AddChunk());
			//threadObj.join();
			//task01.join();
		}
	}
}

//设置角色所在方块的位置 也就是获取角色位置后计算方块位置
bool UNewActorComponent::SetCharactorPossition()
{
	while (MyMianCritialSection.TryLock() == false);///===
	MyMianCritialSection.Lock();///===
	//AActor* myCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AActor* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);//获取角色位置
	if (myCharacter == nullptr)
	{
		//FPlatformProcess::Sleep(0.01);
		//if (FPrimeNumberWorker::IsThreadFinished() == true)
			//FPrimeNumberWorker::EnsureCompletion;
			//FPrimeNumberWorker::Shutdown(); 
		return false;//SetCharactorPossition();
	}
	FVector vector = myCharacter->GetActorLocation();
	vCharactorPosition = FVector(vector.X + iChunkSizeHalf, vector.Y + iChunkSizeHalf, 0);//计算的角色位置
	return true;
}

//in which chunk?
//不停的检查角色是否有移动，是的话返回真   没有移动返回否
bool UNewActorComponent::IsDiffrendChunk()
{
	//UE_LOG(LogTemp, Warning, TEXT("Begin iChunkX:%d,iChunkY:%d"), iChunkX, iChunkY);
	if (iChunkX != floorf(vCharactorPosition.X / iChunkSize) || iChunkY != floorf(vCharactorPosition.Y / iChunkSize))
	{
		iChunkX = floorf((vCharactorPosition.X ) / iChunkSize);
		iChunkY = floorf((vCharactorPosition.Y ) / iChunkSize);//计算所在方块
		//UE_LOG(LogTemp, Warning, TEXT("On Which Chunk???|-> X:(<%d>Y:<%d>)"), iChunkX, iChunkY);
		//UE_LOG(LogTemp, Warning, TEXT("Begin%d,%d"), iChunkX, iChunkY);
		MyMianCritialSection.Unlock();///===
		return true;
	}
	else 
	{ 
		MyMianCritialSection.Unlock();///===
		return false;
	}
}

//loop for create chunk
//添加方块   扫描范围为 +_iRenderRange
bool UNewActorComponent::AddChunk()
{
	for (int i = -iRenderRange; i <= iRenderRange; i++)
	{
		for (int j = -iRenderRange; j <= iRenderRange; j++)
		{
			//if(SpawnChunk(iChunkX + i, iChunkY + j));
			//else
			//{
			//	while (!SpawnChunk(iChunkX + i, iChunkY + j));
			//}
			SpawnChunk(iChunkX + i, iChunkY + j);
			CheckIsAddMoreWall(iChunkX + i, iChunkY + j);
		}
	}
	return true;
}


//check chunk is exsist,if yes , return *Chunk
//检查所在位置是否已经有方块有的话返回指针   没有返回null
MyChunks * UNewActorComponent::CheckExsist(MyChunks * ChunkHead, int i, int j)
{
	MyChunks* pNode = ChunkHead;
	while (pNode != nullptr)//遍历链表
	{
		while (pNode->MyCritialSection.TryLock() == false);///===
		pNode->MyCritialSection.Lock();///===
		if (int(pNode->vChunkVector.X) == i && int(pNode->vChunkVector.Y) == j)//查看所有方块是否有在此位置的
		{
			/*
			if (pNode->vChunkVector.Z > 70)
			{
				UE_LOG(LogTemp, Error, TEXT("Surrounding ->TackBackChunk,VZ:____%f"), pNode->vChunkVector.Z);
			}
			*/
			//pNode->MyCritialSection.Unlock();///===
			return pNode;//有的话返回指针
		}
		pNode->MyCritialSection.Unlock();
		pNode = pNode->next;
		///===
	}
	pNode = nullptr;
	///free(pNode);
	return nullptr;//没有返回空
}

//x=i  y=j  choise N,E,S,W And Chunk Type,return random ActorsSpawns*
//根据方块变动，返回方块所需的方向的信息的随机一个信息
ActorsSpawns * UNewActorComponent::ChoiseReadInfo(MyChunks* Chunk, int i, int j)
{
	if (Chunk == nullptr)
	{
		return nullptr;
	}
	string sPart[4] = { "ZERO" ,"ONE" ,"TWO" ,"THREE" },sChoisePart,sChoisePath; //设置四个方向
	MyChunks* tack = Chunk;
	//while (tack->MyCritialSection.TryLock() == false);///===
	//tack->MyCritialSection.Lock();///===
	int iYawTime = tack->rChunkRotator.Yaw/90;//旋转角度变成N，E，S，W
	
	//UE_LOG(LogTemp, Error, TEXT("Zero|One|Two|Three->was change: %d time____ChunkRotator.Yaw=%f___-iYawTime=%d"), iYawTime, Chunk->rChunkRotator.Yaw, iYawTime);

	for (; iYawTime > 0; iYawTime--)//将sPart重新排列为坐标方块匹配的N，E，S，W匹配的顺序
	{
		for (int num = 3; num > 0; num--)
		{//take end string to top
			string iChange;
			iChange = sPart[num];
			sPart[num] = sPart[num - 1];
			sPart[num -1] = iChange;
		}
	}
	/*
	std::string mlString0(sPart[0]);
	FString hlString0(FString(mlString0.c_str()));
	std::string mlString1(sPart[1]);
	FString hlString1(FString(mlString1.c_str()));
	std::string mlString2(sPart[2]);
	FString hlString2(FString(mlString2.c_str()));
	std::string mlString3(sPart[3]);
	FString hlString3(FString(mlString3.c_str()));
	//UE_LOG(LogTemp, Error, TEXT("0->%s | 1->%s | 2->%s | 3->%s"), *hlString0, *hlString1, *hlString2, *hlString3);
	////*/
	switch (tack->iLable)//选择对应模型文件名

	{
	case 1://Cube
		sChoisePath = "CubeInformation.txt";
		break;
	case 2://Triangle
		sChoisePath = "TriangleInformation.txt";
		break;
	case 3://Chunk->iLable == 3   TriangleConnect
		sChoisePath = "TriangleConnectInformation.txt";
		break;
	default:
		break;
	}

	//
	int iChoisePart;//与目标方块的坐标进行对比，找出相对位置
	if (tack->vChunkVector.X > i)
	{
		sChoisePart=sPart[0];
		iChoisePart = 0;
	}
	else if (tack->vChunkVector.Y > j)
	{
		sChoisePart=sPart[1];
		iChoisePart = 1;
	}
	else if (tack->vChunkVector.X < i)
	{
		sChoisePart=sPart[2];
		iChoisePart = 2;
	}
	else
	{//Chunk->vChunkVector.Y < j
		sChoisePart=sPart[3];
		iChoisePart = 3;
	}//ELogVerbosity::
	//UE_LOG(LogTemp,Error, TEXT("0|1|2|3->Choise:%d"), iChoisePart);
	tack->MyCritialSection.Unlock();//==
	return ReadInfo->pRandom(sChoisePath, sChoisePart);//返回取出的随机信息 
}

//得到随机的位置信息 并计算出位置 返回actorsSpawns   如果没有获取到随机信息，返回nullptr
ActorsSpawns * UNewActorComponent::SetAndRAndomChoiseACtorSpawn(MyChunks * ChoiseChunk, int i, int j)
{
	/*if (ChoiseChunk == nullptr)
	{
		i, j;
		return nullptr;
	}*/
	ActorsSpawns *pRandomInfo = ChoiseReadInfo(ChoiseChunk, i, j);//ReadInfo->pRandom(),
	//while (ChoiseChunk->MyCritialSection.TryLock() == false);///===
	//ChoiseChunk->MyCritialSection.Lock();///===
	//int m = ChoiseChunk->iLable;
	if (pRandomInfo == nullptr)//没获取到 返回空
	{
		//ChoiseChunk->MyCritialSection.Unlock();///===
		UE_LOG(LogTemp, Fatal, TEXT("Check -->nullptr"));
		return nullptr;
	}
	////////////////////////////////////////////-------------------
	//while (ChoiseChunk->MyCritialSection.TryLock() == false);///===
	//ChoiseChunk->MyCritialSection.Lock();///===
	ActorsSpawns *pNew = new ActorsSpawns;//获取到，设置信息
	pNew->MyCritialSection.Lock();//==
	pNew->iActor_Lable = pRandomInfo->iActor_Lable;
	pNew->iZ_Rotator_Diffrient = pRandomInfo->iZ_Rotator_Diffrient + ChoiseChunk->rChunkRotator.Yaw; //
	if (pNew->iZ_Rotator_Diffrient >= 360)//取余，避免值360以上
	{
		pNew->iZ_Rotator_Diffrient = pNew->iZ_Rotator_Diffrient % 360;
	}
	//while(pRandomInfo->MyCritialSection.TryLock() == false); ///===
	//pRandomInfo->MyCritialSection.Lock();///===
	pNew->iZ_High_Diffrient = ChoiseChunk->vChunkVector.Z + pRandomInfo->iZ_High_Diffrient;//设置信息高度
	//0UE_LOG(LogTemp, Error, TEXT("Check -->VectorZ:%d"), pNew->iZ_High_Diffrient);
	pNew->pNext = nullptr;
	///-----
	ChoiseChunk->MyCritialSection.Unlock();///===
	pRandomInfo->MyCritialSection.Unlock();///===
	ChoiseChunk = nullptr;
	pRandomInfo = nullptr;
	///free(TackBackChunk);
	return pNew;
}

//检查每个方块四周最低的位置并添加方块 也就是添加方块底部的方块
bool UNewActorComponent::CheckIsAddMoreWall(int i, int j)
{
	while (MyChunkHead->MyCritialSection.TryLock() == false);
	MyChunkHead->MyCritialSection.Lock();///===
	MyChunks* p = CheckExsist(MyChunkHead, i, j);//
	if (p != nullptr)
	{
		//while(p->MyCritialSection.TryLock()==false);///===
		//p->MyCritialSection.Lock();///===
		if (p->bAddMoreWall == true)//如果是完全位置，跳过，也就是完全匹配完
		{
			MyChunkHead->MyCritialSection.Unlock();///===
			p->MyCritialSection.Unlock();///===
			return false;
		}
		else//否则进行底部方块填充
		{
			int *pTackBackTime = new int;
			int *pTackBackLow = new int;
			*pTackBackTime = 0, *pTackBackLow = p->vChunkVector.Z;//Most High
			//这里计算周围最低的位置，并取出循环次数和最低
			MyChunks* pTempFree = CheckExsistSurrounding(MyChunkHead, i, j, pTackBackTime, pTackBackLow);
			if (pTempFree != nullptr)pTempFree->MyCritialSection.Unlock();///===
			MyChunkHead->MyCritialSection.Unlock();///===
			//UE_LOG(LogTemp, Error, TEXT("END->take back(Time = <%d>___low = <%d>)"), *pTackBackTime,*pTackBackLow);
			if (4 == *pTackBackTime)//四周都有方块时开始填充
			{
				if (*pTackBackLow < p->vChunkVector.Z)
				{
					for (int time = *pTackBackLow ;time< p->vChunkVector.Z;time++)
					{
						//FVector vec = FVector(i*iChunkSize, j*iChunkSize, time*iChunkSize);
						int chunksize = iChunkSize;
						///---------------------------------------------------
						AsyncTask(ENamedThreads::GameThread, [=]() {
							FRotator rot = FRotator(0);
							FVector vec = FVector(i*chunksize, j*chunksize, time*chunksize);
							GetWorld()->SpawnActor<AActor>(Cube, vec, rot);
							// code to execute on game thread here
							
						});
						UE_LOG(LogTemp, Warning, TEXT("(i=<%d>);(j=<%d>);(high=<%d>)"), i*chunksize, j*chunksize, time*chunksize);
					}//变为完全被匹配方块
					p->bAddMoreWall = true;
				}
			}
			p->MyCritialSection.Unlock();///===
			pTackBackTime = nullptr, pTackBackLow = nullptr;
		}
	}
	else return false;
	return false;
}

//check all diraction chunks
//检查四周的方块  如果有最后的两个参数，则周围的方块数量，和找出最低的位置
MyChunks* UNewActorComponent::CheckExsistSurrounding(MyChunks* ChunkHead, int i, int j, int* pBackCheckTime , int* pBackLowHigh )
{
	//MyChunks* chunk = CheckExsist(ChunkHead, i, j);
	///chunk->aChunkActor;
	int iLowest=0;
	//int* pchecktime = pBackCheckTime; int *LowHigh = pBackLowHigh;
	if (ChunkHead == nullptr)//如链表无方块儿创建返回
	{
		MyChunks* p = new MyChunks;
		p->iLable = 1;
		p->vChunkVector = FVector(0);
		p->rChunkRotator = FRotator(0);
		p->MyCritialSection.Lock();
		if (p == nullptr) UE_LOG(LogTemp, Fatal, TEXT("test error nullptr"));
		return p;
	}
	else//已存在链表则下一步
	{
		MyChunks* TackBackChunk=nullptr;
		int conparison=0;

		int iChecki;
		int iCheckj;

		//check N，E，S，W
		for (int time = 0; time < 4; time++)
		{
			iChecki = i;
			iCheckj = j;
			switch (time)
			{
			case 0:
				iChecki += -1;
				break;
			case 1:
				iCheckj += 1;
				break;
			case 2:
				iChecki += 1;
				break;
			case 3:
				iCheckj += -1;
				break;
			}
			if(TackBackChunk == nullptr)//
			{ //第一次的时候进行一次检测存在
				TackBackChunk = CheckExsist(ChunkHead, iChecki, iCheckj);
				if (TackBackChunk != nullptr)//找到目标位置四周存在的方块
				{
					//while (TackBackChunk->MyCritialSection.TryLock() == false);///===
					//TackBackChunk->MyCritialSection.Lock();///===
					if (pBackCheckTime != nullptr)//如有值，则找到四周最低的方块位置
					{//暂时不单独写个函数----------------------------------下面相同
						iLowest=TackBackChunk->vChunkVector.Z;//
						*pBackCheckTime = *pBackCheckTime + 1;//count times+1
						//UE_LOG(LogTemp, Warning, TEXT("(pBackCheckTime=<%d>)"), *pBackCheckTime);
						if (iLowest < *pBackLowHigh)
						{
							*pBackLowHigh = iLowest;
							//UE_LOG(LogTemp, Warning, TEXT("1:(pBackLowHigh=<%d>)"), *pBackLowHigh);
						}
						
					}
					//TackBackChunk->MyCritialSection.Unlock();///===
				}
				/*
				if (TackBackChunk != nullptr)
				{
					if (TackBackChunk->vChunkVector.Z > 70)
					{
						UE_LOG(LogTemp, Error, TEXT("Surrounding ->TackBackChunk,VZ:____%f"), TackBackChunk->vChunkVector.Z);
					}
					//UE_LOG(LogTemp, Error, TEXT("Surrounding chunk__X:%f__Y:%f__ iLable:%d,,Yaw:%f"), TackBackChunk->vChunkVector.X, TackBackChunk->vChunkVector.Y,TackBackChunk->iLable, TackBackChunk->rChunkRotator.Yaw);
				}
				*/
			}
			else//如果周围有方块
			{//TriangleConnect > Triangle > Cube 优先级
				MyChunks* pCompare= CheckExsist(ChunkHead, iChecki, iCheckj);//开始找该位置的对比方块
				//while (TackBackChunk->MyCritialSection.TryLock());///===
				//TackBackChunk->MyCritialSection.Lock();
				if (pCompare != nullptr)
				{
					//while (pCompare->MyCritialSection.TryLock() == false);///===
					//pCompare->MyCritialSection.Lock();///===
					if (pBackCheckTime != nullptr)
					{//暂时不单独写个函数----------------------------------上面相同
						*pBackCheckTime = *pBackCheckTime + 1;//count times
						//UE_LOG(LogTemp, Warning, TEXT("(<%d>)"), *pBackCheckTime);
						iLowest = pCompare->vChunkVector.Z;
						if (iLowest < *pBackLowHigh)
						{
							*pBackLowHigh = iLowest;
							//UE_LOG(LogTemp, Warning, TEXT("2:(pBackLowHigh=<%d>)"), *pBackLowHigh);
						}
					}
					if (pCompare->iLable < TackBackChunk->iLable)//对比优先度，优先替代后面的
					{
						MyChunks* pTempFreeLock = TackBackChunk;
						TackBackChunk = pCompare;
						//UE_LOG(LogTemp, Error, TEXT("Surrounding Compare chunk iLable:%d,,Yaw:%f_______under??"), pCompare->iLable, pCompare->rChunkRotator.Yaw);
						pTempFreeLock->MyCritialSection.Unlock();
						pCompare = nullptr;
					}
					else
					{
						pCompare->MyCritialSection.Unlock();///===
						pCompare = nullptr;
					}
				}
				///free(pCompare);
			}
		}
		//if (TackBackChunk == nullptr&&pBackCheckTime == nullptr) 
		//return CheckExsistSurrounding(ChunkHead, i, j);
		//UE_LOG(LogTemp, Error, TEXT("END"));
		//if (TackBackChunk == nullptr) UE_LOG(LogTemp, Fatal, TEXT("test error nullptr"));
		//TackBackChunk->MyCritialSection.Unlock();
		if (TackBackChunk == nullptr && pBackCheckTime == nullptr)//如果没找到方块并且不计算最低高度
		{//测试
			MyChunks* p = new MyChunks;
			p->iLable = 1;
			p->vChunkVector = FVector(float(i), float(j),0);
			p->rChunkRotator = FRotator(0);
			p->MyCritialSection.Lock();
			//if (p == nullptr) UE_LOG(LogTemp, Fatal, TEXT("test error nullptr"));
			return p;
		}
			//UE_LOG(LogTemp, Fatal, TEXT("Check -->nullptr"));
		return TackBackChunk;
		///TackBackChunk = nullptr;
		//ChoiseReadInfo(TackBackChunk,i,j);
	}
}

//create spawn
//添加方块
bool UNewActorComponent::SpawnChunk(int i, int j)
{///======================================
	if (MyChunkHead !=nullptr)//确保链表不空
	{
		while (MyChunkHead->MyCritialSection.TryLock() == false);//==
		MyChunkHead->MyCritialSection.Lock();//==
		MyChunks* pTempCheck = CheckExsist(MyChunkHead, i, j);//查找对应坐标是否存在方块
		if (pTempCheck != nullptr)//如存在返回真，否则继续
		{
			pTempCheck->MyCritialSection.Unlock();//==
			return true;
		}
		//MyChunkHead->MyCritialSection.Unlock();
	}
///===============================================
	//UE_LOG(LogTemp, Warning, TEXT("----------------------------"));
	//while (MyChunkHead->MyCritialSection.TryLock());
	/*
	if (MyChunkHead != nullptr)
	{
		while (MyChunkHead->MyCritialSection.TryLock() == false);
		MyChunkHead->MyCritialSection.Lock();
	}*/
	
	MyChunks *tast = CheckExsistSurrounding(MyChunkHead, i, j);
	//MyChunkHead->MyCritialSection.Unlock();
	//if (tast == nullptr)
	//{
		//tast=
	//}
	//UE_LOG(LogTemp, Fatal, TEXT("check ->nullptr"));
	//if (tast == nullptr) UE_LOG(LogTemp, Fatal, TEXT("test error nullptr"));
	//while (tast->MyCritialSection.TryLock() == false);///===
	//tast->MyCritialSection.Lock();///===
	ActorsSpawns *info = SetAndRAndomChoiseACtorSpawn(tast,i,j);//check i+_(1) j+_(1)输入对应找到四周的方块的一个
	tast->MyCritialSection.Unlock();
	//if (MyChunkHead != nullptr)MyChunkHead->MyCritialSection.Unlock();
	
	//UE_LOG(LogTemp, Warning, TEXT("Labe:%d  Rotator:%d   Vector:%d"),info->iActor_Lable, info->iZ_Rotator_Diffrient, info->iZ_High_Diffrient);
	MyChunks* pNew = new MyChunks();
	//设置获取到的信息位置
	FVector vec = FVector(float(i *iChunkSize), float(j *iChunkSize), float(info->iZ_High_Diffrient*iChunkSize));

	//UE_LOG(LogTemp, Warning, TEXT("vec.X:%f  vec.Y:%f   vec.Z:%f"), vec.X, vec.Y, vec.Z);
	//设置获取到的信息角度
	FRotator rot = FRotator(0, float(info->iZ_Rotator_Diffrient), 0);
	//UE_LOG(LogTemp, Log, TEXT("Pitch:%f  Yaw:%f   Roll:%f"), rot.Pitch,rot.Yaw,rot.Roll);
	
	///1=TriangleConnect  2=Triangle  3=Cube
	switch (info->iActor_Lable)//根据方块类型产生方块
	{
	case 3:
		AsyncTask(ENamedThreads::GameThread, [=]() {
			pNew->aChunkActor = GetWorld()->SpawnActor<AActor>(TriangleConnect, vec, rot);
			// code to execute on game thread here
		});
		break;
	case 2:
		AsyncTask(ENamedThreads::GameThread, [=]() {
			// code to execute on game thread here
			pNew->aChunkActor = GetWorld()->SpawnActor<AActor>(Triangle, vec, rot);
		});
		break;
	case 1:
		AsyncTask(ENamedThreads::GameThread, [=]() {
			// code to execute on game thread here
			pNew->aChunkActor = GetWorld()->SpawnActor<AActor>(Cube, vec, rot);
		});
		break;
	default:
		return false;
	}
	
	///将产生的方块加入到链表
	pNew->iLable = info->iActor_Lable;
	pNew->bAddMoreWall = false;
	pNew->rChunkRotator = rot;
	pNew->vChunkVector = FVector(float(int(vec.X) / iChunkSize), float(int(vec.Y) / iChunkSize), float(int(vec.Z) / iChunkSize));
	pNew->next = nullptr;
	pNew->MyCritialSection.Unlock();
	///------
	info->MyCritialSection.Unlock();
	delete(info);
	info = nullptr;
	//选择怎么添加到链表
	if (MyChunkHead == nullptr)
	{///first time spawn chunk
		//MyChunkHead->MyCritialSection.Lock();
		MyChunkHead = pNew;
		///------
		MyChunkHead->MyCritialSection.Unlock();///====
		pNew = nullptr;
		//MyChunkHead->MyCritialSection.Unlock();
		///free(pNew);
		return true;
	}
	else
	{
		//while (MyChunkHead->MyCritialSection.TryLock() == false)
		//MyChunkHead->MyCritialSection.Lock();
		pNew->next = MyChunkHead;
		MyChunkHead = pNew;
		MyChunkHead->MyCritialSection.Unlock();///====
		//UE_LOG(LogTemp, Warning, TEXT("Input ---> XY:(<%f><%f>) index(<%d>)"), pNew->vChunkVector.X, pNew->vChunkVector.Y, pNew->iIndex);
		//UE_LOG(LogTemp, Warning, TEXT("create seccess --X:<%f>Y:<%f> seconde etc"), pNew->vChunkVector.X, pNew->vChunkVector.Y);
		///------
		pNew = nullptr;
		//MyChunkHead->MyCritialSection.Unlock();
		return true;
	}
}
