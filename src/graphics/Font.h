#pragma once

#include <gl/glew.h>
#include <gl/glut.h>
#include <map>
#include <glm/vec2.hpp>
#include <string>

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class Font {
public:
	int loadfont();

	void RenderText(GLuint shader, std::string text, float x, float y, float scale, bool rightAlign);

	std::map<char, Character> Characters;

	unsigned int VAO, VBO;
};