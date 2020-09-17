#include "ReadInformation.h"
#include"Engine/World.h"
#include <stdlib.h>
//#define WIN32_LEAN_AND_MEAN
//#include "Windows.h"
//#include <random>
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"

#include "Math/UnrealMathUtility.h"

bool ReadInformation::LoadText(FString FileNameA, FString& SaveTextA) {
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::GameDir() + FileNameA));
}

ActorsSpawns * ReadInformation::ToReadInformation(string sFile, string sPart)//, string Type)
{
	destry();//销毁链表先
	//"D:\\unreal engine\\InfinantRound\\InfinantRoundPack\\WindowsNoEditor\\" +
	ifstream test("D:\\unreal engine\\InfinantRound\\InfinantRoundPack\\WindowsNoEditor\\" + sFile, ios::in);
	string str;
	//ActorsSpawns* pTriangleConnectHead;
	//ActorsSpawns* pHead = nullptr ;
	
	/*
	//#define WIN32_LEAN_AND_MEAN
	//#include "Windows.h"
	TCHAR *path = new TCHAR[MAX_PATH];
	ZeroMemory(path, MAX_PATH);
	// path == "d:\Project\Test\MFC\MFC\debug"
	GetCurrentDirectory(MAX_PATH, path);
	*/

	if (test.good())//如果能读取文件，则按格式读取
	{
		//cout << "成功" << endl;
	
		while (getline(test, str))
		{
			if (str != "")
			{
				ActorsSpawns* pNew = new ActorsSpawns;
				if (str == sPart)
				{
					getline(test, str);
					int iChoise=0;
					//if (Type == str)
					//{
					if (str == "Cube")
					{
						pNew->iActor_Lable = 1;
						iChoise = 1;
					}
					else if (str == "Triangle")
					{
						pNew->iActor_Lable = 2;
						iChoise = 2;
					}
					else if (str == "TriangleConnect")
					{
						pNew->iActor_Lable = 3;
						iChoise = 3;
					}
					else
					{
						UE_LOG(LogTemp,Fatal, TEXT("Read Error"));
						continue;
					}
					pNew->iActor_Lable = iChoise;
					pNew->MyCritialSection.Unlock();///===
					//UE_LOG(LogTemp, Warning, TEXT("Choise_-_-_-_%d"), pNew->iActor_Lable);
					//}
					//else
					//{
					//	continue;
					//}
					//pNew->sActor_Name = str;
					/*if (str=="Cube")
					{
						pNew->iActor_Lable = 1;
					}
					else if (s
					pNew->pNext = nullptr;
					UE_LOG(LogTemp, Warning, TEXT("Find->Labe:%d  Rotator:%d   Vector:%d"), pNew->iActor_Lable, pNew->iZ_Rotator_Diffrient, pNew->iZ_High_Diffrient);
				}
				else
				{
					PassLine(&test);tr == "Triangle")
					{
						pNew->iActor_Lable = 2;
					}
					else
					{//TriangleConnect=3
						pNew->iActor_Lable = 3;
					}*/
					//cout <<  str << endl;

					getline(test, str);//设置信息
					pNew->iZ_Rotator_Diffrient = stoi(str, 0);
					//cout <<  str << endl;
					getline(test, str);
					pNew->iZ_High_Diffrient = stoi(str, 0);
					//cout <<   str << endl;
					pNew->pNext=nullptr;
					pNew->MyCritialSection.Unlock();
					//UE_LOG(LogTemp, Warning, TEXT("Find->Labe:%d  Rotator:%d   Vector:%d"), pNew->iActor_Lable, pNew->iZ_Rotator_Diffrient, pNew->iZ_High_Diffrient);
				}
				else
				{
					continue;
				}

				if (pSpawnInfoHead == nullptr)//看看信息链表是否存在不存在新建否则加罪后面
				{
					pSpawnInfoHead = pNew;
					while(pSpawnInfoHead->MyCritialSection.TryLock()==false);///===
					pSpawnInfoHead->MyCritialSection.Lock();///===
				}
				else
				{
					pSpawnInfoHead->MyCritialSection.Unlock();
					pNew->pNext = pSpawnInfoHead;
					pSpawnInfoHead = pNew;
					while (pSpawnInfoHead->MyCritialSection.TryLock() == false);///===
					pSpawnInfoHead->MyCritialSection.Lock();///===
				}
				pNew = nullptr;
			}
			else
			{
				continue;
			}
		}
			/*
			while (getline(test, str))//按行读文件
			{
		
					cout << " " << str << endl;
			}
			*/
				//这里可以不要，只是用来看你自己txt文档中原来的内容
				/*
				if (str[0] == '>')
					continue;
				else
					temp.insert(temp.length(), str, 0, str.size());//最终都存入temp中
				*/
	}
	else
		UE_LOG(LogTemp, Fatal, TEXT("Read Error"));
	/*
	ActorsSpawns* p=pHead;
	while (p != NULL)
	{
		cout << p->sActor_Name << endl;
		cout << p->iZ_Rotator_Diffrient << endl;
		cout << p->iZ_High_Diffrient << endl;
		p = p->pNext;
	}
	*/

	//std::cout << "Hello World!\n"; 
	//UE_LOG(LogTemp, Error, TEXT("Find->Over"));
	return pSpawnInfoHead;
}
//销毁所有链表
bool ReadInformation::destry()
{
	//ActorsSpawns* node = pSpawnInfoHead;
	ActorsSpawns* next = nullptr;
	if (pSpawnInfoHead !=nullptr)
	{
		while (pSpawnInfoHead->MyCritialSection.TryLock() == false);///===
		pSpawnInfoHead->MyCritialSection.Lock();///===
		next = pSpawnInfoHead->pNext;
		delete(pSpawnInfoHead);
		pSpawnInfoHead = next;
	}
	pSpawnInfoHead = nullptr; next = nullptr;
	return false;
}


/*
ActorsSpawns * ReadInformation::ChunkKind(int type)
{
	if (true)
	{

	}
	return nullptr;
}
*/
//跳过不符合的信息格式
ifstream * ReadInformation::PassLine(ifstream* stream)
{
	string temp;
	for (int i = 0; i < 3; i++)
	{
		getline(*stream, temp);
	}
	return nullptr;
}

//取链表里的随机一个信息
ActorsSpawns * ReadInformation::pRandom(string sFile, string sPart)
{
	  // Random generator seed
	// Use the time function to get a "seed” value for srand
	//seed = time(NULL);
	//srand(seed);
	//int RandomNum = rand() % 3;
	/*std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 3);
	int RandomNum = distribution(generator);
	*/
	//FString pFile();
	//FString pPart = &sPart;

	/*
	std::string mlString1(sFile);
	FString hlString1(FString(mlString1.c_str()));

	std::string mlString2(sPart);
	FString hlString2(FString(mlString2.c_str()));
	*/
	//UE_LOG(LogTemp, Warning, TEXT("sFile=__%s__sPart=__%s"), *hlString1, *hlString2);
	//int RandomNum = FMath().RandRange(0,2);
	//UE_LOG(LogTemp, Warning, TEXT("Set random num Choise type is(0:CB,1:T,2:TC) = %d"),RandomNum);
	//string RandomChunks[3] = { "Cube","Triangle","TriangleConnect" };
	ToReadInformation(sFile, sPart);//, RandomChunks[RandomNum]);
	if (pSpawnInfoHead==nullptr)//如果没信息返回空
	{
		return nullptr;
	}
	//seed = time(NULL);
	//srand(seed);
	//int limit = LineLength();
	/*
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, limit);
	RandomNum = distribution(generator);
	*/
	int RandomNum = FMath().RandRange(0, LineLength()-1);//看链表长度，并取一个随机值
	//UE_LOG(LogTemp, Warning, TEXT("Set random Num position=%d"), RandomNum+1);
	ActorsSpawns* p= pSpawnInfoHead;
	
	while (p != nullptr && RandomNum > 0)//找到定下来的随机位置的信息
	{
		//p->MyCritialSection.Lock();
		ActorsSpawns* pTempFree = p;
		p = p->pNext;
		p->MyCritialSection.Lock();
		pTempFree->MyCritialSection.Unlock();
		RandomNum--;
		/*
		if (p->iActor_Lable != 1 && 0 == RandomNum)
		{
			p = pSpawnInfoHead;
			seed = time(0);
			srand(seed);
			RandomNum = rand() % limit;
		}
		*/
	}
	pSpawnInfoHead->MyCritialSection.Unlock();
	//UE_LOG(LogTemp, Warning, TEXT("pRandomHigh=%d"), p->iZ_High_Diffrient);
	return p;
}

//获取链表长度
int ReadInformation::LineLength()
{
	ActorsSpawns*p = pSpawnInfoHead;
	int time=0;
	while (p != nullptr)
	{
		p = p->pNext;
		time++;
	}
	p = nullptr;
	return time;
}

//////////////////////////////////////////////////////////
