#pragma once
#define WINDOWS
#define DEBUG true

#ifdef WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <windows.h>
#include <gl/glew.h>
#include <gl/glut.h>