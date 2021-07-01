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

	const char* vsParticle = "resources/shader.vs";
	const char* fsParticleNormal = "resources/shader.fs";
	const char* vsParticlePressure = "resources/shaderp.vs";
	const char* fsBackground = "resources/background.fs";
	const char* vsBackground = "resources/background.vs";
	const char* fVSFileName = "resources/font.vs";
	const char* fFSFileName = "resources/font.fs";
	const char* bVSFileName = "resources/bond.vs";
	const char* bFSFileName = "resources/bond.fs";

	float bgPosData[8] = { -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0 };
	float bgUVdata[8] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
};