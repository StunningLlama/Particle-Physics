#pragma once

#include <string>
#include "Font.h"
#include <string>
#include "GUI.h"

class Graphics {
public:
	GUI gui;
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
	void AddShader(GLuint ShaderProgram, std::string pShaderText, GLenum ShaderType);
	void CompileShaders(GLuint &ShaderProgram, std::string vsName, std::string fsName);
	bool ReadFile(std::string pFileName, std::string& outFile);

	int windowWidth = 0;
	int windowHeight = 0;

	GLuint VBOparticleVertex;
	GLuint VBOparticleUV;
	GLuint VBObondVertex;
	GLuint VBOparticlePressure;
	GLuint VBOparticleColor;
	GLuint VBObackgroundVertex;
	GLuint VBObackgroundUV;
	GLuint gTransformParticleShaderNormal;
	GLuint gTransformFont;
	GLuint gTransformParticleShaderPressure;
	GLuint gTransformBondShader;
	GLuint gFontColor;
	GLuint gTexture;
	GLuint particleShaderNormal;
	GLuint particleShaderPressure;
	GLuint backgroundShader;
	GLuint fontShader;
	GLuint bondShader;
	GLuint backgroundTexture;
	GLuint windowId;

	std::string vsParticle = "resources/shader.vs";
	std::string fsParticleNormal = "resources/shader.fs";
	std::string vsParticlePressure = "resources/shaderp.vs";
	std::string fsBackground = "resources/background.fs";
	std::string vsBackground = "resources/background.vs";
	std::string fVSFileName = "resources/font.vs";
	std::string fFSFileName = "resources/font.fs";
	std::string bVSFileName = "resources/bond.vs";
	std::string bFSFileName = "resources/bond.fs";

	float bgPosData[8] = { -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0 };
	float bgUVdata[8] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
};