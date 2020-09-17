#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <time.h> 

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ActorsSpawns.h"

//using namespace std;

class ReadInformation
{
public:
	
	bool destry();
	//链表  获取的模式
	ActorsSpawns* pSpawnInfoHead;
	//获取随机的一个匹配信息
	ActorsSpawns* pRandom(string sFile, string sPart);

private:
	//读取
	bool LoadText(FString FileNameA, FString& SaveTextA);
	ActorsSpawns* ToReadInformation(string sFile, string sPart);//, string Type);
	//1:cube  2:triangle  3:triangleconnect
	//ActorsSpawns* ChunkKind(int type);
	ifstream* PassLine(ifstream* stream);
	int LineLength();
};



///////////////////////////////////////////////////////////////////////////////////////
