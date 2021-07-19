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

void Button::updateColors() {
	if (hovering) {
		if (clicking) {
			r = buttonpressedr;
			g = buttonpressedg;
			b = buttonpressedb;

			textr = textactivatedr;
			textg = textactivatedg;
			textb = textactivatedg;
		}
		else {

			if (activated) {
				r = buttonactivatedr*1.5f;
				g = buttonactivatedg*1.5f;
				b = buttonactivatedb*1.5f;

				textr = textactivatedr;
				textg = textactivatedg;
				textb = textactivatedb;
			}
			else {
				r = buttondefaultr * 1.5f;
				g = buttondefaultg * 1.5f;
				b = buttondefaultb * 1.5f;

				textr = textdefaultr;
				textg = textdefaultg;
				textb = textdefaultb;
			}
		}
		hovering = true;
	}
	else {
		if (activated) {
			r = buttonactivatedr;
			g = buttonactivatedg;
			b = buttonactivatedb;

			textr = textactivatedr;
			textg = textactivatedg;
			textb = textactivatedb;
		}
		else {
			r = buttondefaultr;
			g = buttondefaultg;
			b = buttondefaultb;

			textr = textdefaultr;
			textg = textdefaultg;
			textb = textdefaultb;
		}
		hovering = false;
	}
}