#include "../Includes.h"
#include "Button.h"
#include "Font.h"
#include "../Life.h"

void Button::setGraphicsData() {
	vertexdata[0] = x;
	vertexdata[1] = y;
	vertexdata[2] = r;
	vertexdata[3] = g;
	vertexdata[4] = b;
	vertexdata[5] = x+width;
	vertexdata[6] = y;
	vertexdata[7] = r;
	vertexdata[8] = g;
	vertexdata[9] = b;
	vertexdata[10] = x+width;
	vertexdata[11] = y+height;
	vertexdata[12] = r;
	vertexdata[13] = g;
	vertexdata[14] = b;
	vertexdata[15] = x;
	vertexdata[16] = y+height;
	vertexdata[17] = r;
	vertexdata[18] = g;
	vertexdata[19] = b;
}

Button::Button(float x, float y, float width, float height, std::string text) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->text = text;
}

void Button::renderText(Font f, GLuint shader) {
	float fontpt = 36.0f;
	float fontheight = 12.0f;
	
	f.RenderText(shader, this->text, x+width/2.0f, y + (height / 2.0f - fontheight / 2.0f), fontheight / fontpt, font_align_center);
}

void Button::mouseclick(int mx, int my, bool mousepressed) {
	if (this->hovering && !mousepressed) {
		instance->input->keyboard('1' + actionid, 0, 0);
		this->activated = true;
	} else if (!this->hovering && !mousepressed) {
		this->activated = false;
	}
	else if (this->hovering && mousepressed) {
		r = 0.7;
		g = 0.7;
		b = 0.6;
	}
}
void Button::mousemovement(int mx, int my, bool mousepressed) {
	if (mx > x && mx < x + width && my > y && my < y + height) {
		if (mousepressed) {
			r = 0.7;
			g = 0.7;
			b = 0.6;
		}
		else {
			r = 0.5;
			g = 0.5;
			b = 0.5;
		}
		hovering = true;
	}
	else {
		if (activated) {
			r = (float)instance->sim->colortable[actionid][0] / 255.0f;
			g = (float)instance->sim->colortable[actionid][1] / 255.0f;
			b = (float)instance->sim->colortable[actionid][2] / 255.0f;
		}
		else {
			r = 0.2;
			g = 0.2;
			b = 0.2;
		}
		hovering = false;
	}
}