#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "core/gl2d.h"

#pragma once
#ifndef UTIL_
#define UTIL_
#define PI 3.14159265358979323846
#define DEG2RAD(DEG) ((DEG)*((PI)/(180.0)))

enum DIRECTION{
	RIGHT = 1<<0,
	LEFT  = 1<<1,
	UP = 1<<2,
	DOWN = 1<<3
};

class Util {
public:



	enum Sprites{
		IMG_GAME_BACKGROUND1,

		COP_CAR1_1,COP_CAR1_2,COP_CAR1_3,

		IMG_TERRAIN1_1,
		IMG_TERRAIN1_2,
		IMG_TERRAIN1_3,
		IMG_PARALLAX_1,

		LARGADA1,
		LARGADA2,
		LARGADA3,
		GO,
		BANDEIRA,	

		SHADE1,
		SHADE2,
		RANK,
		RANK1,


		LAPS,
		SHADOW,

		JOYPAD,

		RADIALBLUR,

		LEO,

		SIZE
	};

	enum Fonts{
		AGENCY_36,
		AGENCY_16,
		AGENCY_48,
		AGENCY_24,
		AGENCY_2K12_18,
		ARIAL_15,
	};

	static std::vector<glImage*> textures;


	static std::vector<int> generateRandomArray(int size);

	static int random(int min, int max);

	static void quickSort(std::vector<int>* elements);

	static void quickSort(std::vector<int>* elements, int lowIndex, int highIndex);

	static const int CAR_LENGTH;

	static bool accel;
	static bool music;
	static bool sound;

};


static Util::Sprites CAR_SPRITES[1][1][3]=
{
	{
		{Util::COP_CAR1_1,Util::COP_CAR1_2,Util::COP_CAR1_3}
	}
};

template <typename T>
void FreeAll( T & t ) {
	T tmp;
	t.swap( tmp );
}




#endif