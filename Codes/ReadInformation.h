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
	//����  ��ȡ��ģʽ
	ActorsSpawns* pSpawnInfoHead;
	//��ȡ�����һ��ƥ����Ϣ
	ActorsSpawns* pRandom(string sFile, string sPart);

private:
	//��ȡ
	bool LoadText(FString FileNameA, FString& SaveTextA);
	ActorsSpawns* ToReadInformation(string sFile, string sPart);//, string Type);
	//1:cube  2:triangle  3:triangleconnect
	//ActorsSpawns* ChunkKind(int type);
	ifstream* PassLine(ifstream* stream);
	int LineLength();
};



///////////////////////////////////////////////////////////////////////////////////////
