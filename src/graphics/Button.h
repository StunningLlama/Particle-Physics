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
	float buttondefaultr = 0.2;
	float buttondefaultg = 0.2;
	float buttondefaultb = 0.2;
	float buttonactivatedr = 0.7;
	float buttonactivatedg = 0.7;
	float buttonactivatedb = 0.7;
	float buttonpressedr = 0.8;
	float buttonpressedg = 0.8;
	float buttonpressedb = 0.8;
	float textdefaultr = 0.9;
	float textdefaultg = 0.9;
	float textdefaultb = 0.9;
	float textactivatedr = 0.1;
	float textactivatedg = 0.1;
	float textactivatedb = 0.1;
	float r = 0.2;
	float g = 0.2;
	float b = 0.2;
	float textr = 0.9;
	float textg = 0.9;
	float textb = 0.9;
	bool hovering = false;
	bool activated = false;
	bool clicking = false;
	void setGraphicsData();
	int actionid = 0;
	int buttongroup = 0;
	std::string text;
	Button(float x, float y, float width, float height, std::string text);
	void renderText(Font f, GLuint shader);
	void updateColors();
};