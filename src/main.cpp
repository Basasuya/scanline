#include <iostream>
#include <ctime>
#include <vector>
#include "Obj.h"
#include "Tool.h"
#include "Point.h"
#include "Scanline.h"
#include <GLUT/glut.h>

int width = 1000; int height = 800;
Obj obj;
std::vector<std::vector<Color> > buffer;
Scanline eliminate;

char objPath[200] = "./models/al.obj";

int ArgPos(char *str, int argc, char **argv) {
    int a;
    for (a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
        if (a == argc - 1) {
            printf("Argument missing for %s\n", str);
            exit(1);
        }
        return a;
    }
    return -1;
}

void setParams(int argc, char ** argv) {
    int i;
    if ((i = ArgPos((char *) "-read", argc, argv)) > 0) strcpy(objPath, argv[i + 1]);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);

	obj.scale(width, height);
	eliminate.setSize(width, height);
	eliminate.BuildTable(obj);
	eliminate.scan(obj, buffer);


	glBegin(GL_POINTS);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			glColor3ub(buffer[j][i], buffer[j][i] , buffer[j][i]);
			glVertex2i(i, j);
		}
	}
	glEnd();

	glFinish();
}
void reshape(int w,int h) {
    width = w;
	height = h;
	glViewport(0, 0, w, h);
}

int main(int argc,char** argv) {
    setParams(argc, argv);
    if(strlen(objPath) == 0) {
        printf("the obj Path is NULL\n");
        exit(0);
    }
	obj.load(objPath);
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("scan-line");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 1;
}