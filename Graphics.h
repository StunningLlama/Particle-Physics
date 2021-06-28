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
	int shadermode = 1;
	int bgmode = 0;
	bool displaybonds = false;
	float pressurecontrast = 0.5;
	float pressureoffset = 0.0;
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
	GLuint VBObgPos;
	GLuint VBObgUV;
	GLuint gTransform;
	GLuint gTransform2;
	GLuint gTransform3;
	GLuint gTransform4;
	GLuint gFontColor;
	GLuint gTexture;
	GLuint particleShaderNormal;
	GLuint particleShaderPressure;
	GLuint backgroundShader;
	GLuint fontShader;
	GLuint bondShader;
	GLuint bgTexture;

	const char* vsParticle = "shader.vs";
	const char* fsParticleNormal = "shader.fs";
	const char* vsParticlePressure = "shaderp.vs";
	const char* fsBackground = "background.fs";
	const char* vsBackground = "background.vs";
	const char* fVSFileName = "font.vs";
	const char* fFSFileName = "font.fs";
	const char* bVSFileName = "bond.vs";
	const char* bFSFileName = "bond.fs";

	float bgPosData[8] = { -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0 };
	float bgUVdata[8] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
};