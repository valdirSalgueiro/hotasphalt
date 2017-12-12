#ifndef GL2D_H_INCLUDED
#define GL2D_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include "SpriteBatch.h"


typedef enum GL2D_FLIP_MODE
{

    GL2D_NONE 	= 1 << 0,
    GL2D_FLIP_V 	= 1 << 1,
    GL2D_FLIP_H 	= 1 << 2,
    GL2D_CENTER 	= 1 << 3,
	GL2D_CENTER_HORIZONTAL 	= 1 << 4,
	GL2D_CENTER_VERTICAL 	= 1 << 5,
    GL2D_CENTER_2 	= 1 << 6,
    GL2D_NO_SCALE 	= 1 << 7,
	GL2D_ABSOLUTE 	= 1 << 8,
	GL2D_BOTTOM 	= 1 << 9,
	GL2D_RIGHT		= 1 << 10,
	GL2D_FONT		= 1 << 11,
	GL2D_COLOR_ADVANCED		= 1 << 12,
	GL2D_SCALE_TO_SCREEN = 1 << 13,
	GL2D_NO_SRC = 1 << 14,
} GL2D_FLIP_MODE;


typedef enum GL2D_BLEND_MODE
{

    GL2D_TRANS = 0,
	GL2D_SOLID,
	GL2D_BLENDED,
	GL2D_GLOW,
	GL2D_ALPHA,
	GL2D_BLACK


} GL2D_BLEND_MODE;

typedef struct glImage
{

    int		width;
    int 	height;
    float	u_off;
    float	v_off;
    float	u_width;
    float	v_height;

    GLuint	textureID;

} glImage;


void glScreenInit(const int width, const int height);

void glClearScreen();

void glSprite(int x, int y, int flipmode, const glImage *spr,bool rotate=false,const float angle=0.0f, const float scale1=1, const float scale2=1, float r=1, float g=1, float b=1, float a=1, float rb=0, float gb=0, float bb=0, float ab=0);

GLuint glLoadSprite(const char *filename, glImage *sprite);

GLuint glLoadSpriteset( const char *filename,
                        glImage *sprite,
                        int width,
                        int height,
                        const int numframes,
                        const unsigned int *texcoords,
                        const  GLuint filtermode, const int frame=0
                      );


int glLoadTileSet(  const char *filename,
				   glImage              *sprite,
				   int                  tile_wid,
				   int                  tile_hei,
                   int                  bmp_wid,
				   int                  bmp_hei		  
                 );

void Init2D( const int width, const int height );
void setSize(const int width, const int height );

void spriteBatchBegin();
void spriteBatchEnd();
void spriteBatchDestroy();

void startBatch( const int width, const int height );

void spriteBatchAddQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b, float a, int textureid);
void spriteBatchDraw(int x, int y, int flipmode, const glImage *spr,bool rotate=false,const float angle=0.0f, const float scale1=1, const float scale2=1, float r=1, float g=1, float b=1, float a=1, float rb=1, float gb=1, float bb=1, float ab=1);

void spriteDrawLine(int x1, int y1, int x2, int y2);

void glFree();


#endif // GL2D_H_INCLUDED
