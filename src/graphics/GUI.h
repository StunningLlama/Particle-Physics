#pragma once

#include <string>
#include "Font.h"
#include <string>
#include <vector>
#include "Button.h"

class Graphics;
class GUI {
public:
	Font font;
	float mat[3][3] = { {0.01f, 0.0f, 0.0f}, {0.0f, 0.01f, 0.0f}, {0.0f, 0.0f, 1.0f} };
	void updatebuffer();
	int initializeGraphics();
	void display();
	void CreateVertexBuffer();

	std::vector<Button*> buttons;

	GLuint gFontColor;
	GLuint VBO;
	GLuint gTransformButtonShader;
	GLuint gTransformFontShader;
	GLuint fontShader;
	GLuint uiShader;
	GLuint windowId;
	Graphics* gphx;

	bool mousedown = false;

	std::string vsButton = "resources/button.vs";
	std::string fsButton = "resources/button.fs";
	std::string fVSFileName = "resources/font.vs";
	std::string fFSFileName = "resources/font.fs";
};