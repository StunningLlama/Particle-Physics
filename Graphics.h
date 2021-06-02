#pragma once

#include <gl/glew.h>
#include <gl/glut.h>
#include <string>
#include "Font.h"
#include <string>

class Graphics {
public:
	Font font;

	std::string hudtext;

	int shadermode = 0;

	float mat[3][3] = { {0.01f, 0.0f, 0.0f}, {0.0f, 0.01f, 0.0f}, {0.0f, 0.0f, 1.0f} };

	void updatebuffer();

	int initializeGraphics();

	void display();

	void CreateVertexBuffer();

	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	void CompileShaders(GLuint &ShaderProgram, const char *vsName, const char *fsName);

	bool ReadFile(const char* pFileName, std::string& outFile);

private:

	GLuint VBOv;
	GLuint VBOuv;
	GLuint VBOd;
	GLuint VBOp;
	GLuint VBOc;
	GLuint gTransform;
	GLuint gTransform2;
	GLuint gTransform3;
	GLuint gFontColor;
	GLuint particleShaderNormal;
	GLuint particleShaderPressure;
	GLuint fontShader;
	const char* vsParticle = "shader.vs";
	const char* fsParticleNormal = "shader.fs";
	const char* vsParticlePressure = "shaderp.vs";
	const char* fVSFileName = "font.vs";
	const char* fFSFileName = "font.fs";
};