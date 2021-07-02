#include "../Includes.h"
#include "GUI.h"
#include "Button.h"
#include "../Life.h"

#include <iostream>
#include <fstream>

void GUI::updatebuffer() {
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setGraphicsData();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, i * 20 * sizeof(float), 20 * sizeof(float), &(buttons[i]->vertexdata[0]));
	}
}


void GUI::display() {
	updatebuffer();
	glClear(GL_COLOR_BUFFER_BIT);

	mat[0][0] = 2.0f / glutGet(GLUT_WINDOW_WIDTH);
	mat[1][1] = 2.0f / glutGet(GLUT_WINDOW_HEIGHT);
	mat[2][0] = -1.0f;
	mat[2][1] = -1.0f;

	glUseProgram(uiShader);
	glUniformMatrix3fv(gTransformButtonShader, 1, GL_FALSE, &mat[0][0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(2 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, buttons.size() * 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUseProgram(0);


	glUseProgram(fontShader);

	glUniform3f(gFontColor, 0.9f, 0.9f, 0.9f);

	glUniformMatrix3fv(gTransformFontShader, 1, GL_FALSE, &mat[0][0]);

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->renderText(font, fontShader);
	}

	glUseProgram(0);

	glutSwapBuffers();
}

void GUI::CreateVertexBuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, buttons.size() * sizeof(float) * 5 * 4, NULL, GL_DYNAMIC_DRAW);
}


void guiMouseClick(int button, int state, int x, int y) {
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	GUI* gui = &instance->gphx->gui;
	gui->mousedown = button == GLUT_LEFT_BUTTON && state == GLUT_DOWN;
	for (Button* b : gui->buttons) {
		b->mouseclick(x, windowHeight - y, gui->mousedown);
	}
}

void guiMouseMotion(int x, int y) {
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	GUI* gui = &instance->gphx->gui;
	for (Button* b : gui->buttons) {
		b->mousemovement(x, windowHeight - y, gui->mousedown);
	}
}

void guiMousePassiveMotion(int x, int y) {
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	GUI* gui = &instance->gphx->gui;
	for (Button* b : gui->buttons) {
		b->mousemovement(x, windowHeight - y, gui->mousedown);
	}
}

void guiloop() {
	instance->gphx->gui.display();
}

int GUI::initializeGraphics() {
	for (int i = 0; i < sim_materials; i++) {
		Button* b = new Button(20, i * 50 + 20, 260, 40, instance->sim->materialnames[i]);
		b->actionid = i;
		buttons.push_back(b);
	}
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(300, 600);
	glutInitWindowPosition(50, 50);
	windowId = glutCreateWindow("Tools");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMouseFunc(guiMouseClick);
	glutMotionFunc(guiMouseMotion);
	glutPassiveMotionFunc(guiMousePassiveMotion);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	gphx->CompileShaders(uiShader, vsButton, fsButton);
	gTransformButtonShader = glGetUniformLocation(uiShader, "transform");

	gphx->CompileShaders(fontShader, fVSFileName, fFSFileName);
	gTransformFontShader = glGetUniformLocation(fontShader, "transform");
	gFontColor = glGetUniformLocation(fontShader, "textColor");


	int fontstate = font.loadfont();
	if (fontstate != 0) {
		std::cout << "Error loading font!";
		system("PAUSE");
	}
	//glUseProgram(ShaderProgram);

	glutDisplayFunc(guiloop);
	//glutIdleFunc(guiloop);
	return 0;
}