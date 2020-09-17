#pragma once

using namespace std;

class ActorsSpawns
{
public:
	int iActor_Lable;//Cube=1  Triangle=2  TriangleConnect=3
	int iZ_Rotator_Diffrient;
	int iZ_High_Diffrient;
	ActorsSpawns* pNext;
	FCriticalSection MyCritialSection;
};

