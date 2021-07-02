#pragma once
#include <string>
#include "Font.h"
class Button {
public:
	float x;
	float y;
	float width;
	float height;
	float vertexdata[5*4];
	float r = 0.2;
	float g = 0.2;
	float b = 0.2;
	bool hovering = false;
	bool activated = false;
	void setGraphicsData();
	int actionid = 0;
	std::string text;
	Button(float x, float y, float width, float height, std::string text);
	void renderText(Font f, GLuint shader);
	void mousemovement(int x, int y, bool mousepressed);
	void mouseclick(int x, int y, bool mousepressed);
};