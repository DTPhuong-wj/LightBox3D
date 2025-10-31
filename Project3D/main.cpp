#include <windows.h>
#include <GL/glut.h>
#include <cmath>        // Cho sinf, cosf
#include <cstdio>       // Cho printf
#include <cstdlib>      // Cho exit
#include "light.h"      // Include file header của light.cpp

// Biến điều khiển camera
float cameraAngleX = 30.0f;
float cameraAngleY = 45.0f;
float cameraDistance = 15.0f;

// ================================
// HÀM VẼ MỘT SÀN ĐƠN GIẢN
// ================================
void drawFloor() {
    setupWoodMaterial();  // Dùng hàm từ light.cpp

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(10.0f, 0.1f, 10.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// ================================
// HÀM VẼ MỘT CUBE ĐƠN GIẢN
// ================================
void drawTestCube() {
    setupMetalMaterial();  // Dùng hàm từ light.cpp

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidCube(1.5f);
    glPopMatrix();
}

// ================================
// HÀM VẼ MỘT SPHERE ĐƠN GIẢN
// ================================
void drawTestSphere() {
    setupPlasticMaterial();  // Dùng hàm từ light.cpp

    glPushMatrix();
    glTranslatef(2.5f, 1.5f, 0.0f);
    glutSolidSphere(0.8f, 32, 32);
    glPopMatrix();
}

// ================================
// HÀM DISPLAY
// ================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Setup camera
    float camX = cameraDistance * sinf(cameraAngleY * 3.14159f / 180.0f);
    float camY = cameraDistance * sinf(cameraAngleX * 3.14159f / 180.0f);
    float camZ = cameraDistance * cosf(cameraAngleY * 3.14159f / 180.0f);

    gluLookAt(camX, camY, camZ,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    // Vẽ các object đơn giản để test ánh sáng
    drawFloor();
    drawTestCube();
    drawTestSphere();

    glutSwapBuffers();
}

// ================================
// HÀM RESHAPE
// ================================
void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// ================================
// HÀM XỬ LÝ PHÍM
// ================================
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        toggleMainLight();  // Từ light.cpp
        break;
    case '2':
        toggleLampLight();  // Từ light.cpp
        break;
    case '3':
        toggleSpotLight();  // Từ light.cpp
        break;
    case '4':
        toggleAmbientLight();  // Từ light.cpp
        break;
    case '+':
    case '=':
        adjustLampIntensity(0.1f);  // Từ light.cpp
        break;
    case '-':
    case '_':
        adjustLampIntensity(-0.1f);  // Từ light.cpp
        break;
    case 't':
    case 'T':
        setTimeOfDay(12.0f);  // Ban ngày - từ light.cpp
        break;
    case 'n':
    case 'N':
        setTimeOfDay(0.0f);  // Ban đêm - từ light.cpp
        break;
    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// ================================
// HÀM XỬ LÝ PHÍM MŨI TÊN
// ================================
void specialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraAngleX += 5.0f;
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraAngleX -= 5.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        break;
    case GLUT_KEY_LEFT:
        cameraAngleY -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraAngleY += 5.0f;
        break;
    case GLUT_KEY_PAGE_UP:
        cameraDistance -= 0.5f;
        if (cameraDistance < 2.0f) cameraDistance = 2.0f;
        break;
    case GLUT_KEY_PAGE_DOWN:
        cameraDistance += 0.5f;
        if (cameraDistance > 50.0f) cameraDistance = 50.0f;
        break;
    }
    glutPostRedisplay();
}

// ================================
// HÀM IDLE - ANIMATION
// ================================
float lastTime = 0.0f;

void idle() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Cập nhật ánh sáng từ light.cpp
    updateLighting(deltaTime);

    glutPostRedisplay();
}

// ================================
// HÀM MAIN
// ================================
int main(int argc, char** argv) {
    // Khởi tạo GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Test Light.cpp - Lighting System");

    // Màu nền
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // QUAN TRỌNG: Khởi tạo hệ thống ánh sáng từ light.cpp
    initLighting();

    // Đăng ký callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutIdleFunc(idle);

    // In hướng dẫn
    printf("=== TEST LIGHT.CPP ===\n");
    printf("Controls:\n");
    printf("  1: Toggle main light\n");
    printf("  2: Toggle lamp light\n");
    printf("  3: Toggle spotlight\n");
    printf("  4: Toggle ambient light\n");
    printf("  +/-: Adjust lamp intensity\n");
    printf("  T: Daytime\n");
    printf("  N: Nighttime\n");
    printf("  Arrow Keys: Rotate camera\n");
    printf("  Page Up/Down: Zoom\n");
    printf("  ESC: Exit\n");
    printf("======================\n");

    // Bắt đầu main loop
    glutMainLoop();

    return 0;
}