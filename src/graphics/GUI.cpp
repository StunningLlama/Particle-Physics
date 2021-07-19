#include "../Includes.h"
#include "GUI.h"
#include "Button.h"
#include "../Life.h"
#include "../Input.h"

#include <iostream>
#include <fstream>

void GUI::updatebuffer() {
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->updateColors();
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

	glUniformMatrix3fv(gTransformFontShader, 1, GL_FALSE, &mat[0][0]);

	for (int i = 0; i < buttons.size(); i++) {
		glUniform3f(gFontColor, buttons[i]->textr, buttons[i]->textg, buttons[i]->textb);
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


void guiMouseClick(int button, int state, int mx, int my) {
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	GUI* gui = &instance->gphx->gui;
	for (Button* b : gui->buttons) {
		int x = mx;
		int y = windowHeight - my;
		if (b->hovering && (button == GLUT_LEFT_BUTTON && state == GLUT_UP)) {
			if (b->buttongroup == button_group_materials) {
				gui->setMaterial(b->actionid);
			}
			else if (b->buttongroup == button_group_brushes) {
				gui->setBrush(b->actionid);
			}
			else if (b->buttongroup == button_group_save) {
				instance->input->save = true;
			}
			else if (b->buttongroup == button_group_load) {
				instance->input->load = true;
			}
			else if (b->buttongroup == button_group_pause) {
				gui->togglePause();
			}
		}
		else if (b->hovering && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			b->clicking = true;
		}
	}
	gui->mousedown = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);


	if (button == 3 && state == GLUT_DOWN)
		instance->input->brushsize = instance->input->clamp(instance->input->brushsize + 1, 1, 5);
	if (button == 4 && state == GLUT_DOWN)
		instance->input->brushsize = instance->input->clamp(instance->input->brushsize - 1, 1, 5);
}


void GUI::setMaterial(int id) {
	instance->input->modematerial = id;
	for (Button* bp : buttons) {
		if (bp->buttongroup == button_group_materials) {
			if (bp->actionid == id) {
				bp->activated = true;
			}
			else {
				bp->activated = false;
			}
		}
	}
}

void GUI::setBrush(int id) {
	instance->input->modebrush = id;
	for (Button* bp : buttons) {
		if (bp->buttongroup == button_group_brushes) {
			if (bp->actionid == id) {
				bp->activated = true;
			}
			else {
				bp->activated = false;
			}
		}
	}
}

void GUI::togglePause() {
	for (Button* bp : buttons) {
		if (bp->buttongroup == button_group_pause) {
			bp->activated = !bp->activated;
			instance->input->paused = bp->activated;
			if (bp->activated) {
				bp->text = "Unpause";
			} else {
				bp->text = "Pause";
			}
		}
	}
}

void guiMouseMotion(int mx, int my) {
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	GUI* gui = &instance->gphx->gui;
	for (Button* b : gui->buttons) {
		int x = mx;
		int y = windowHeight - my;
		if (x > b->x && x < b->x + b->width && y > b->y && y < b->y + b->height) {
			b->hovering = true;
			if (gui->mousedown) {
				b->clicking = true;
			}
			else {
				b->clicking = false;
			}
		}
		else {
			b->hovering = false;
			b->clicking = false;
		}
	}
}

void guiMousePassiveMotion(int mx, int my) {
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	GUI* gui = &instance->gphx->gui;
	for (Button* b : gui->buttons) {
		float x = mx;
		float y = windowHeight - my;
		if (x > b->x && x < b->x + b->width && y > b->y && y < b->y + b->height) {
			b->hovering = true;
			if (gui->mousedown) {
				b->clicking = true;
			}
			else {
				b->clicking = false;
			}
		}
		else {
			b->hovering = false;
			b->clicking = false;
		}
	}
}

void guiloop() {
	instance->gphx->gui.display();
}

int GUI::initializeGraphics() {

	float height = 30;
	float spacing = 40;
	float groupspacing = 20;

	for (int i = 0; i < sim_materials; i++) {
		int materialid = sim_materials - i - 1;
		Button* b = new Button(20, i * spacing + 20, 260, height, instance->sim->materialnames[materialid]);
		b->actionid = materialid;
		b->buttonactivatedr = (float)instance->sim->colortable[b->actionid][0] / 255.0f;
		b->buttonactivatedg = (float)instance->sim->colortable[b->actionid][1] / 255.0f;
		b->buttonactivatedb = (float)instance->sim->colortable[b->actionid][2] / 255.0f;
		b->textdefaultr = (float)instance->sim->colortable[b->actionid][0] / 255.0f;
		b->textdefaultg = (float)instance->sim->colortable[b->actionid][1] / 255.0f;
		b->textdefaultb = (float)instance->sim->colortable[b->actionid][2] / 255.0f;
		b->buttongroup = button_group_materials;
		buttons.push_back(b);
	}

	int offset = sim_materials;

	for (int i = 0; i < brushes; i++) {
		int brushid = brushes - i - 1;
		Button* b = new Button(20, (i+ offset) * spacing + 20 + groupspacing, 260, height, instance->input->brushnames[brushid]);
		b->actionid = brushid;
		b->buttonactivatedr = 240 / 255.0f;
		b->buttonactivatedg = 240 / 255.0f;
		b->buttonactivatedb = 240 / 255.0f;
		b->textdefaultr = 240 / 255.0f;
		b->textdefaultg = 240 / 255.0f;
		b->textdefaultb = 240 / 255.0f;
		b->buttondefaultr = 100 / 255.0f;
		b->buttondefaultg = 100 / 255.0f;
		b->buttondefaultb = 100 / 255.0f;
		b->buttongroup = button_group_brushes;
		buttons.push_back(b);
	}

	offset = sim_materials + brushes;

	{
		Button* b = new Button(20, (0 + offset) * spacing + 20 + 2 * groupspacing, 260, height, "Load");
		b->actionid = 0;
		b->buttonactivatedr = 240 / 255.0f;
		b->buttonactivatedg = 240 / 255.0f;
		b->buttonactivatedb = 240 / 255.0f;
		b->textdefaultr = 240 / 255.0f;
		b->textdefaultg = 240 / 255.0f;
		b->textdefaultb = 240 / 255.0f;
		b->buttondefaultr = 100 / 255.0f;
		b->buttondefaultg = 100 / 255.0f;
		b->buttondefaultb = 100 / 255.0f;
		b->buttongroup = button_group_load;
		buttons.push_back(b);
	}

	{
		Button* b = new Button(20, (1 + offset) * spacing + 20 + 2 * groupspacing, 260, height, "Save");
		b->actionid = 0;
		b->buttonactivatedr = 240 / 255.0f;
		b->buttonactivatedg = 240 / 255.0f;
		b->buttonactivatedb = 240 / 255.0f;
		b->textdefaultr = 240 / 255.0f;
		b->textdefaultg = 240 / 255.0f;
		b->textdefaultb = 240 / 255.0f;
		b->buttondefaultr = 100 / 255.0f;
		b->buttondefaultg = 100 / 255.0f;
		b->buttondefaultb = 100 / 255.0f;
		b->buttongroup = button_group_save;
		buttons.push_back(b);
	}

	{
		Button* b = new Button(20, (2 + offset) * spacing + 20 + 2 * groupspacing, 260, height, "Pause");
		b->actionid = 0;
		b->buttonactivatedr = 240 / 255.0f;
		b->buttonactivatedg = 240 / 255.0f;
		b->buttonactivatedb = 240 / 255.0f;
		b->textdefaultr = 240 / 255.0f;
		b->textdefaultg = 240 / 255.0f;
		b->textdefaultb = 240 / 255.0f;
		b->buttondefaultr = 100 / 255.0f;
		b->buttondefaultg = 100 / 255.0f;
		b->buttondefaultb = 100 / 255.0f;
		b->buttongroup = button_group_pause;
		buttons.push_back(b);
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(300, (offset+3)*spacing + 4*groupspacing);
	glutInitWindowPosition(50, 50);
	windowId = glutCreateWindow("Tools");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMouseFunc(guiMouseClick);
	glutMotionFunc(guiMouseMotion);
	glutPassiveMotionFunc(guiMousePassiveMotion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

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