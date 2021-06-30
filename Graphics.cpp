#include "stdafx.h"
#include "Graphics.h"
#include "Life.h"

#include <iostream>
#include <fstream>

void Graphics::updatebuffer() {

	for (Particle *p : instance->sim->particles) {
		instance->sim->g_coords[p->id * 8] = p->x - instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 1] = p->y - instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 2] = p->x - instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 3] = p->y + instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 4] = p->x + instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 5] = p->y + instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 6] = p->x + instance->sim->drawradius[p->material]*p->size;
		instance->sim->g_coords[p->id * 8 + 7] = p->y - instance->sim->drawradius[p->material]*p->size;

		float dp = (p->avgpressure - instance->sim->avgoverallpressure) * pressurecontrast + 0.5f + pressureoffset;
		instance->sim->g_pressures[p->id * 4] = dp;
		instance->sim->g_pressures[p->id * 4 + 1] = dp;
		instance->sim->g_pressures[p->id * 4 + 2] = dp;
		instance->sim->g_pressures[p->id * 4 + 3] = dp;
	}

	for (Bond* b : instance->sim->bonds) {
		instance->sim->g_bonds[b->id * 4] = b->particleA->x;
		instance->sim->g_bonds[b->id * 4 + 1] = b->particleA->y;
		instance->sim->g_bonds[b->id * 4 + 2] = b->particleB->x;
		instance->sim->g_bonds[b->id * 4 + 3] = b->particleB->y;
	}

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 

	glBindBuffer(GL_ARRAY_BUFFER, VBOv);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(a),&a, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, instance->sim->g_coords.size()*sizeof(float), &instance->sim->g_coords[0], GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
	glBufferData(GL_ARRAY_BUFFER, instance->sim->g_uvs.size()*sizeof(float), &instance->sim->g_uvs[0], GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, VBOd);
	glBufferData(GL_ARRAY_BUFFER, instance->sim->g_bonds.size()*sizeof(float), &instance->sim->g_bonds[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOp);
	glBufferData(GL_ARRAY_BUFFER, instance->sim->g_pressures.size()*sizeof(float), &instance->sim->g_pressures[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOc);
	glBufferData(GL_ARRAY_BUFFER, instance->sim->g_colors.size()*sizeof(unsigned char), &instance->sim->g_colors[0], GL_DYNAMIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pressureXpoints, pressureYpoints, 0, GL_RED, GL_FLOAT, &instance->sim->localpressureavg);
}


void Graphics::display() {
	instance->graphics_1.start();
	glClear(GL_COLOR_BUFFER_BIT);

	mat[0][0] = 2.0f / instance->sim->xbound;
	mat[1][1] = 2.0f / instance->sim->ybound;
	mat[2][0] = -1.0f;
	mat[2][1] = -1.0f;


	if (bgmode == 1) {
		glUseProgram(backgroundShader);
		glUniform1i(gTexture, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bgTexture);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, VBObgPos);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBObgUV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUseProgram(0);
	}


	if (displaybonds) {
		glUseProgram(bondShader);
		glUniformMatrix3fv(gTransform4, 1, GL_FALSE, &mat[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBOd);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_LINES, 0, 2 * instance->sim->bonds.size());
		glDisableVertexAttribArray(0);
	}


	if (shadermode == 1) {
		glUseProgram(particleShaderNormal);
		glUniformMatrix3fv(gTransform, 1, GL_FALSE, &mat[0][0]);
	}
	if (shadermode == 2) {
		glUseProgram(particleShaderPressure);
		glUniformMatrix3fv(gTransform3, 1, GL_FALSE, &mat[0][0]);
	}

	if (shadermode == 1 || shadermode == 2) {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, VBOv);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBOp);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBOc);
		glVertexAttribPointer(3, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

		glDrawArrays(GL_QUADS, 0, instance->sim->particleid * 4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		glUseProgram(0);
	}


	glUseProgram(fontShader);

	glUniform3f(gFontColor, 0.5f, 0.0f, 0.0f);

	glUniformMatrix3fv(gTransform2, 1, GL_FALSE, &mat[0][0]);

	font.RenderText(fontShader, instance->input->brushnames[instance->input->modebrush], instance->sim->xbound - 1.0f, instance->sim->ybound - 5.0f, 0.1f, true);
	font.RenderText(fontShader, instance->sim->materialnames[instance->input->modematerial], instance->sim->xbound - 1.0f, instance->sim->ybound - 10.0f, 0.1f, true);
	font.RenderText(fontShader, instance->input->sizenames[instance->input->brushsize-1], instance->sim->xbound - 1.0f, instance->sim->ybound - 15.0f, 0.1f, true);
	font.RenderText(fontShader, instance->input->densitynames[instance->input->density - 1], instance->sim->xbound - 1.0f, instance->sim->ybound - 20.0f, 0.1f, true);
	font.RenderText(fontShader, "Particles: " + std::to_string(instance->sim->particleid), 1.0f, instance->sim->ybound - 5.0f, 0.1f, false);
	font.RenderText(fontShader, "Time: " + std::to_string(instance->sim->stime), 1.0f, instance->sim->ybound - 10.0f, 0.1f, false);
	if (instance->input->paused)
		font.RenderText(fontShader, "Paused", 1.0f, instance->sim->ybound - 15.0f, 0.1f, false);
	glUseProgram(0);

	glutSwapBuffers();
	instance->graphics_1.end();
}

void Graphics::CreateVertexBuffer()
{
	glGenBuffers(1, &VBOv);
	glGenBuffers(1, &VBOuv);
	glGenBuffers(1, &VBOd);
	glGenBuffers(1, &VBOp);
	glGenBuffers(1, &VBOc);
	glGenBuffers(1, &VBObgPos);
	glGenBuffers(1, &VBObgUV);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


void Graphics::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (int)strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		system("PAUSE");
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
	//glBindAttribLocation(ShaderProgram, 0, "Position");
}

void Graphics::CompileShaders(GLuint &ShaderProgram, const char *vsName, const char *fsName)
{
	ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	std::string vs, fs;

	if (!ReadFile(vsName, vs)) {
		exit(1);
	};

	if (!ReadFile(fsName, fs)) {
		exit(1);
	};

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	//glBindAttribLocation(ShaderProgram, 0, "position")
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
}


void mouseClick(int button, int state, int x, int y) {
	instance->input->mouseClick(button, state, x, y);
}

void mouseMotion(int x, int y) {
	instance->input->mouseMotion(x, y);
}

void mousePassiveMotion(int x, int y) {
	instance->input->mousePassiveMotion(x, y);
}

void keyboard(unsigned char code, int x, int y) {
	instance->input->keyboard(code, x, y);
}

void special(int code, int x, int y) {
	instance->input->special(code, x, y);
}

int Graphics::initializeGraphics() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	if (sim_width / sim_height > ((float)glutGet(GLUT_SCREEN_WIDTH) / (float)glutGet(GLUT_SCREEN_HEIGHT))) {
		glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH)*0.9, glutGet(GLUT_SCREEN_WIDTH) * sim_height / sim_width * 0.9);
	}
	else {
		glutInitWindowSize(glutGet(GLUT_SCREEN_HEIGHT) * sim_width / sim_height * 0.9, glutGet(GLUT_SCREEN_HEIGHT) * 0.9);
	}
	glutInitWindowPosition(50, 50);
	glutCreateWindow("N�ot n�ot");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassiveMotion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, instance->sim->xbound, 0, instance->sim->ybound, 1, -1);
	glMatrixMode(GL_MODELVIEW);*/
	
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(0x8861);
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		system("PAUSE");
		return 1;
	}

	glPointSize(10.f);
	glLineWidth(2.0f);
	CreateVertexBuffer();

	glGenTextures(1, &bgTexture);
	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindBuffer(GL_ARRAY_BUFFER, VBObgPos);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &bgPosData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBObgUV);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &bgUVdata, GL_STATIC_DRAW);


	CompileShaders(particleShaderNormal, vsParticle, fsParticleNormal);
	CompileShaders(particleShaderPressure, vsParticlePressure, fsParticleNormal);
	gTransform = glGetUniformLocation(particleShaderNormal, "transform");
	gTransform3 = glGetUniformLocation(particleShaderPressure, "transform");
	CompileShaders(fontShader, fVSFileName, fFSFileName);
	gTransform2 = glGetUniformLocation(fontShader, "transform");
	gFontColor = glGetUniformLocation(fontShader, "textColor");
	CompileShaders(backgroundShader, vsBackground, fsBackground);
	gTexture = glGetUniformLocation(backgroundShader, "BGtexture");
	CompileShaders(bondShader, bVSFileName, bFSFileName);
	gTransform4 = glGetUniformLocation(bondShader, "transform");
	//std::cout << gTransform2 << std::endl;
	std::cout << gFontColor << std::endl;
	//std::cout << glGetUniformLocation(fontShader, "text") << std::endl;
	if (true) {
		GLint i;
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(fontShader, GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);

		for (i = 0; i < count; i++)
		{
			glGetActiveUniform(fontShader, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		}
	}


	int fontstate = font.loadfont();
	if (fontstate != 0) {
		std::cout << "Error loading font!";
		system("PAUSE");
	}
	//glUseProgram(ShaderProgram);

	return 0;
}


bool Graphics::ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);

	bool ret = false;

	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		std::cout << "�rror reading file!";
		system("PAUSE");
		
		exit(1);
	}

	return ret;
}