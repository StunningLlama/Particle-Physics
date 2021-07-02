#pragma once

#define font_align_left 0
#define font_align_center 1
#define font_align_right 2

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

	void RenderText(GLuint shader, std::string text, float x, float y, float scale, int align);

	std::map<char, Character> Characters;

	unsigned int VAO, VBO;
};