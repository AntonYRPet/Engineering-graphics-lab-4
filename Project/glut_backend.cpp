#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glut_backend.h"

static ICallbacks* s_pCallbacks = NULL;

// ������� ����� ��� ������������� � �������� ��������� ������ glut
static void SpecialKeyboardCB(int Key, int x, int y) {
    s_pCallbacks->SpecialKeyboardCB(Key, x, y);
}

static void KeyboardCB(unsigned char Key, int x, int y) {
    s_pCallbacks->KeyboardCB(Key, x, y);
}

static void PassiveMouseCB(int x, int y) {
    s_pCallbacks->PassiveMouseCB(x, y);
}

static void RenderSceneCB() {
    s_pCallbacks->RenderSceneCB();
}

static void IdleCB() {
    s_pCallbacks->IdleCB();
}

// ������������� ������� ��������� ������ glut
static void InitCallbacks() {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
}

// ������������� glut
void GLUTBackendInit(int argc, char** argv) {
    glutInit(&argc, argv); // ������������� glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ��������� �����������
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); // ��������� ����� glut
}

// �������� ������
bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, unsigned int bpp, bool isFullScreen, const char* pTitle) {
    if (isFullScreen) { // ���� ���� ������� ������, �� ������ �����
        char ModeString[64] = { 0 };
        snprintf(ModeString, sizeof(ModeString), "%dx%d@%d", Width, Height, bpp);
        glutGameModeString(ModeString);
        glutEnterGameMode();
    }
    else { // ����� �� ��������
        glutInitWindowSize(Width, Height);
        glutCreateWindow(pTitle);
    }

    // �������� � �������� glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
    }

    return true;
}

// ������ ������� glut � ��������� ������ � ��������
void GLUTBackendRun(ICallbacks* pCallbacks) {
    if (!pCallbacks) {
        fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
        return;
    }

    // �������
    glEnable(GL_DEPTH_TEST);

    // ���� � ��������� ��������
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // ������������� ������� ��������� ������
    s_pCallbacks = pCallbacks;
    InitCallbacks();
    // �������� ���� glut
    glutMainLoop();
}