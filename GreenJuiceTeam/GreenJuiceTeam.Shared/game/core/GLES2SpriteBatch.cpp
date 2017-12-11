#include <math.h>
#include <memory.h>
#include <stdio.h>
#include "GLES2SpriteBatch.h"

#include <stdio.h>
#include <string.h>

#define STRINGIFY(A) #A

/*
   Shaders for the spritebatch rendering with a color multiply
*/
const char* strSpriteBatchFragmentShaderColorMultiplty =
STRINGIFY(
	uniform sampler2D sampler2d;
varying lowp vec4 vertexColor;
varying lowp vec2 texCoord;

void main(void)
{
	gl_FragColor = texture2D(sampler2d, texCoord)*vertexColor;
}
);

const char* strSpriteBatchVertexShaderColorMultiply =
STRINGIFY(attribute highp vec3 vertex;
uniform mediump mat4 projMatrix;
uniform mediump mat4 im[16];         // input
varying mediump vec2 texCoord;
varying mediump vec4 vertexColor;

void main(void)
{
	mediump mat4 thisim = im[int(vertex.z)]; //input
	highp vec2 transVertex = vec2(thisim[2][0], thisim[2][1]) + mat2(thisim[0][0], thisim[0][1], thisim[1][0], thisim[1][1]) * (vertex.xy - vec2(thisim[2][2] - 0.5, thisim[2][3] - 0.5));
	gl_Position = vec4(transVertex, 1, 1) * projMatrix;
	vertexColor = vec4(thisim[3][0], thisim[3][1], thisim[3][2], thisim[3][3]);
	texCoord = (vertex.xy + vec2(0.5, 0.5)) * vec2(thisim[1][2], thisim[1][3]) + vec2(thisim[0][2], thisim[0][3]);
});

/*
   Shaders for the spritebatch rendering with a color multiply for alpha
*/
const char* strSpriteBatchFragmentShaderAlpha =
STRINGIFY(uniform sampler2D sampler2d;
varying lowp vec4 vertexColor;
varying lowp vec2 texCoord;

void main(void)
{
	gl_FragColor = texture2D(sampler2d, texCoord).w*vertexColor;
});

const char* strSpriteBatchVertexShaderAlpha =
STRINGIFY(attribute highp vec3 vertex;
uniform mediump mat4 projMatrix;
uniform mediump mat4 im[16];
varying mediump vec2 texCoord;
varying mediump vec4 vertexColor;

void main(void)
{
	mediump mat4 thisim = im[int(vertex.z)];
	highp vec2 transVertex = vec2(thisim[2][0], thisim[2][1]) + mat2(thisim[0][0], thisim[0][1], thisim[1][0], thisim[1][1]) * (vertex.xy - vec2(thisim[2][2] - 0.5, thisim[2][3] - 0.5));
	gl_Position = vec4(transVertex, 1, 1) * projMatrix;
	vertexColor = vec4(thisim[3][0], thisim[3][1], thisim[3][2], thisim[3][3]);
	texCoord = (vertex.xy + vec2(0.5, 0.5)) * vec2(thisim[1][2], thisim[1][3]) + vec2(thisim[0][2], thisim[0][3]);
});

/*
   Shaders for the spritebatch rendering with a color multiply
*/
const char* strSpriteBatchFragmentShaderColorAdvanced =
STRINGIFY(uniform sampler2D sampler2d;
uniform mediump vec4 colorAdvanced;

varying lowp vec4 vertexColor;
varying lowp vec2 texCoord;

void main(void)
{
	lowp vec4 colorFinal;
	lowp float r_original = texture2D(sampler2d, texCoord).r;
	lowp float g_original = texture2D(sampler2d, texCoord).g;
	lowp float b_original = texture2D(sampler2d, texCoord).b;
	lowp float a_original = texture2D(sampler2d, texCoord).a;

	colorFinal.r = r_original * (vertexColor.r / 100.0) + colorAdvanced.r;
	colorFinal.g = g_original * (vertexColor.g / 100.0) + colorAdvanced.g;
	colorFinal.b = b_original * (vertexColor.b / 100.0) + colorAdvanced.b;
	colorFinal.a = a_original * (vertexColor.a / 100.0) + colorAdvanced.a;


	gl_FragColor = colorFinal;
});

const char* strSpriteBatchVertexShaderColorAdvanced =
STRINGIFY(attribute highp vec3 vertex;
uniform mediump mat4 projMatrix;
uniform mediump mat4 im[16];
varying mediump vec2 texCoord;
varying mediump vec4 vertexColor;

uniform mediump vec4 colorAdvanced;

void main(void)
{

	mediump mat4 thisim = im[int(vertex.z)];
	highp vec2 transVertex = vec2(thisim[2][0], thisim[2][1]) + mat2(thisim[0][0], thisim[0][1], thisim[1][0], thisim[1][1]) * (vertex.xy - vec2(thisim[2][2] - 0.5, thisim[2][3] - 0.5));
	gl_Position = vec4(transVertex, 1, 1) * projMatrix;
	vertexColor = vec4(thisim[3][0], thisim[3][1], thisim[3][2], thisim[3][3]);
	texCoord = (vertex.xy + vec2(0.5, 0.5)) * vec2(thisim[1][2], thisim[1][3]) + vec2(thisim[0][2], thisim[0][3]);
});

GLES2SpriteBatch::GLES2SpriteBatch(int w, int h) : SpriteBatch(w, h)
{
	for (int f = 0; f < COSINE_TABLE_SIZE; f++)
		cosineTable[f] = cosf((float)f / (float)COSINE_TABLE_SIZE * 3.14159f * 2.0f);

	batchCounter = 0;

	// Initialize shaders
	GLint retval;

	for (int f = 0; f < 4; f++) {
		if (f == 1)
			continue;

		fragmentShader[f] = glCreateShader(GL_FRAGMENT_SHADER);

		switch (f) {
		case 0:
			glShaderSource(fragmentShader[f], 1, (const char**)&strSpriteBatchFragmentShaderColorMultiplty, NULL);
			break;
		case 2:
			glShaderSource(fragmentShader[f], 1, (const char**)&strSpriteBatchFragmentShaderAlpha, NULL);
			break;
		case 3:
			glShaderSource(fragmentShader[f], 1, (const char**)&strSpriteBatchFragmentShaderColorAdvanced, NULL);
			break;
		}

		glCompileShader(fragmentShader[f]);
		glGetShaderiv(fragmentShader[f], GL_COMPILE_STATUS, &retval);
		if (!retval)
		{
			// failed
			printf("failed to compile fshader: %d", f);
		}
		else {}

		vertexShader[f] = glCreateShader(GL_VERTEX_SHADER);

		switch (f) {
		case 0:
			glShaderSource(vertexShader[f], 1, (const char**)&strSpriteBatchVertexShaderColorMultiply, NULL);
			break;
		case 2:
			glShaderSource(vertexShader[f], 1, (const char**)&strSpriteBatchVertexShaderAlpha, NULL);
			break;
		case 3:
			glShaderSource(vertexShader[f], 1, (const char**)&strSpriteBatchVertexShaderColorAdvanced, NULL);
			break;
		}

		glCompileShader(vertexShader[f]);
		glGetShaderiv(vertexShader[f], GL_COMPILE_STATUS, &retval);
		if (!retval) {
			// failed
			printf("failed to compile vshader: %d", f);
		}
		else {}

		program[f] = glCreateProgram();
		glAttachShader(program[f], fragmentShader[f]);
		glAttachShader(program[f], vertexShader[f]);
		// Bind the custom vertex attributes
		glBindAttribLocation(program[f], 0, "vertex");

		glLinkProgram(program[f]);

		// Check if linking succeeded in the same way we checked for compilation success
		glGetProgramiv(program[f], GL_LINK_STATUS, &retval);
		if (!retval) {
			// failed
			printf("failed to link program");
			GLint size = 0;
			glGetProgramiv(program[f], GL_INFO_LOG_LENGTH, &size);
			GLsizei realSize = size; // including the null terminator
			char * infoprintf = new char[size];
			GLsizei obtainedprintfSize = 0; // not including the null terminator
			glGetProgramInfoLog(program[f], realSize, &obtainedprintfSize, infoprintf);
			if (obtainedprintfSize + 1 != realSize) {
				printf("mismatch in sizes %d vs %d", realSize, obtainedprintfSize);
			}
			printf(infoprintf);
			delete[] infoprintf;
		}
		else {
			printf("Finished building program %d", f);
		}


		samplerLocation[f] = glGetUniformLocation(program[f], "sampler2d");
		inputMatrixLocation[f] = glGetUniformLocation(program[f], "im");
		projmLocation[f] = glGetUniformLocation(program[f], "projMatrix");
		colorLocation[f] = glGetUniformLocation(program[f], "colorAdvanced");
	}


	glUseProgram(program[0]);
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = { -0.5f,-0.5f,0.0f, 0.5f,-0.5f,0.0f, 0.5f,0.5f,0.0f,    // triangle 1
						  -0.5f,-0.5f,0.0f, 0.5f,0.5f,0.0f, -0.5f,0.5f,0.0f };   // triangle 2

	GLfloat *tempVertices = new GLfloat[3 * 6 * BATCH_SIZE];
	for (int f = 0; f < BATCH_SIZE; f++) {
		memcpy(tempVertices + f * 3 * 6, vertices, 3 * 6 * sizeof(GLfloat));
		for (int g = 0; g < 6; g++) tempVertices[f * 3 * 6 + 2 + g * 3] = f;            // mark the index for each triangle
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, BATCH_SIZE * 6 * sizeof(GLfloat) * 3, tempVertices, GL_STATIC_DRAW);

	delete[] tempVertices;
}


GLES2SpriteBatch::~GLES2SpriteBatch()
{
	for (int f = 0; f < 4; f++) {
		if (program[f]) glDeleteProgram(program[f]);
		if (fragmentShader[f]) glDeleteShader(fragmentShader[f]);
		if (vertexShader[f]) glDeleteShader(vertexShader[f]);
	}

	if (vbo) glDeleteBuffers(1, &vbo);
	vbo = 0;
	for (int f = 0; f < 4; f++) {
		program[f] = 0;
		fragmentShader[f] = 0;
		vertexShader[f] = 0;
	}
}


void GLES2SpriteBatch::flushSprites()
{
	if (batchCounter < 1) return;         // nothing to fill

	// Fill the inputTempMatrices
	float *m = inputMatrixTemp;
	float *c = colorTemp;
	SpriteDrawInfo *sdi = batchCollection;

	float angletointmul = (float)COSINE_TABLE_SIZE / 3.14159f / 2.0f;

	for (int f = 0; f < batchCounter; f++)
	{
		// targetposition
		m[8 + 0] = sdi->posx;
		m[8 + 1] = sdi->posy;
		// origin
		m[8 + 2] = sdi->originX;
		m[8 + 3] = sdi->originY;

		// Destination orientation
		int iangle = ((int)(sdi->angle * angletointmul) & COSINE_TABLE_AND);
		m[0] = cosineTable[iangle] * sdi->scaleX;
		m[1] = cosineTable[(iangle + COSINE_TABLE_SIZE / 4)&COSINE_TABLE_AND] * sdi->scaleX;
		m[4 + 0] = cosineTable[(iangle + COSINE_TABLE_SIZE / 4)&COSINE_TABLE_AND] * sdi->scaleY;
		m[4 + 1] = -cosineTable[iangle] * sdi->scaleY;

		// Source rectangle
		m[2] = sdi->sourcex;
		m[3] = sdi->sourcey;
		m[4 + 2] = sdi->sourceWidth;
		m[4 + 3] = sdi->sourceHeight;

		m[12 + 0] = sdi->r;
		m[12 + 1] = sdi->g;
		m[12 + 2] = sdi->b;
		m[12 + 3] = sdi->a;

		c[0] = sdi->rb;
		c[1] = sdi->gb;
		c[2] = sdi->bb;
		c[3] = sdi->ab;

		m += 16;
		c += 4;
		sdi++;
	}

	glUniformMatrix4fv(inputMatrixLocation[currentProgram], batchCounter, GL_FALSE, inputMatrixTemp);
	glUniform4fv(colorLocation[currentProgram], batchCounter, colorTemp);

	glDrawArrays(GL_TRIANGLES, 0, 6 * batchCounter);

	// reset batch
	batchCounter = 0;
}


void GLES2SpriteBatch::draw(SpriteDrawInfo *sdi, int spriteCount)
{
	SpriteDrawInfo *i = sdi;
	while (spriteCount > 0) {

		int selectedProgram = 0;

		if (sdi->effect == FONT) {
			selectedProgram = 2;
		}
		else if (sdi->effect == COLOR_ADVANCED) {
			selectedProgram = 3;

		}

		setActiveProgram(selectedProgram);

		if (i->textureHandle != currentTexture) {
			// Texture have been changed, flush currently collected renderingdata
			flushSprites();
			// Change our current texture and activate it.
			currentTexture = i->textureHandle;
			glBindTexture(GL_TEXTURE_2D, currentTexture);
		}


		memcpy(batchCollection + batchCounter, i, sizeof(SpriteDrawInfo));
		batchCounter++;
		if (batchCounter >= BATCH_SIZE)
			flushSprites();

		if (sdi->effect == COLOR_ADVANCED)
			flushSprites();

		i++;
		spriteCount--;
	}
}

void GLES2SpriteBatch::setActiveProgram(int programIndex)
{
	if (currentProgram != programIndex) {
		flushSprites();
		glUseProgram(program[programIndex]);
		glUniform1i(samplerLocation[programIndex], 0);
		glUniformMatrix4fv(projmLocation[programIndex], 1, GL_FALSE, projectionMatrix);
		currentProgram = programIndex;
	}
}

void GLES2SpriteBatch::begin(BlendMode bmode, TransformMode dTransform, float *customProjectionMatrix)
{
	if (batchCounter != 0)
	{
		//printf("Warning: SpriteBatch begin() called before end()");
	}

	currentDestinationTransform = dTransform;

	// Build a new projection matrix accoring screen's width and height
	GLfloat mproj[16] = {
		2.0f / (float)targetWidth, 0, 0, -1.0f,
		0, 2.0f / (float)targetHeight, 0, -1.0f,
		0, 0, 1, 0,
		0,0, 0, 1,
	};

	memcpy(projectionMatrix, mproj, 16 * sizeof(GLfloat));


	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	if (bmode == eALPHA)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisable(GL_CULL_FACE);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);

	currentTexture = 0;
	currentProgram = -1;
}

void GLES2SpriteBatch::end()
{
	// Draw everything that is collected.
	flushSprites();
}
