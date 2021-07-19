#pragma once

//#define buttongroups 2
#define button_group_materials 0
#define button_group_brushes 1
#define button_group_load 3
#define button_group_save 4
#define button_group_pause 5

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
	//int selected[buttongroups] = { 0 };

	void setMaterial(int id);
	void setBrush(int id);
	void togglePause();

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


void mouseClick(int button, int state, int x, int y);

void mouseMotion(int x, int y);

void mousePassiveMotion(int x, int y);

void keyboard(unsigned char code, int x, int y);

void special(int code, int x, int y);