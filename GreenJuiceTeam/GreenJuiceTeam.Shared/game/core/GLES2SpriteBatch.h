#ifndef __GLES2SPRITEBATCH__
#define __GLES2SPRITEBATCH__

#include "SpriteBatch.h"
#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>

#include <OpenGLES/ES2/gl.h>
#else // __ANDROID__ or _WIN32
#if !defined(GLFW_INCLUDE_ES2)
#include <GLES2/gl2.h>
#endif
#endif

#define COSINE_TABLE_SIZE 1024
#define COSINE_TABLE_AND 1023

#define BATCH_SIZE 16 // NOTE, THIS CANNOT BE CHANGED UNLESS CORRESPONDING VALUES FROM THE SHADER'S ARE CHANGED ACCORDINGLY

class GLES2SpriteBatch : public SpriteBatch
{
public:
    GLES2SpriteBatch(int targetWidth, int targetHeight);
    virtual ~GLES2SpriteBatch();

    void draw ( SpriteDrawInfo *sdi, int spriteCount=1 );
    void begin( BlendMode blendMode, TransformMode dTransform = ePIXELSPACE, float *customProjectionMatrix = 0);
    void end();


    void flushSprites();

protected:
    float cosineTable[COSINE_TABLE_SIZE];
    float inputMatrixTemp[ BATCH_SIZE * 16 ];
	float colorTemp[ BATCH_SIZE * 4 ];

    SpriteDrawInfo batchCollection[ BATCH_SIZE ];
    int batchCounter;

        // Currently active texture
    GLuint currentTexture;
    int currentTextureWidth;
    int currentTextureHeight;

        // Objects for GLES2 rendering
    TransformMode currentDestinationTransform;
    int currentViewportWidth;
    int currentViewportHeight;

    void setActiveProgram( int programIndex );
    int currentProgram;

    GLfloat projectionMatrix[16];         // Sroted projectionmatrix.

        // index0 = texture*color, index1=texture
    GLint program[4];
    GLint fragmentShader[4];
    GLint vertexShader[4];

    // Some locations to the program
    GLuint inputMatrixLocation[4];
	GLuint colorLocation[4];
    GLuint projmLocation[4];
    GLuint samplerLocation[4];


    GLuint vbo;
};

#endif
