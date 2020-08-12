#include "GL/glut.h"
int mouse_x = 0, mouse_y = 0;
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, static_cast<double>(w) / h, 10.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
void display()
{
    constexpr float box_size = 2;
    const int h = glutGet(GLUT_WINDOW_HEIGHT);
    auto draw_nth_boxs_outline = [](int n) {
        const int x = (n - 1) % 8, y = (n - 1) / 8;
        glPushMatrix();
        glTranslated((x - 3.5) * box_size, (y - 3.5) * box_size, 0);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-box_size / 2, -box_size / 2, -box_size / 2);
        glVertex3f(-box_size / 2, +box_size / 2, -box_size / 2);
        glVertex3f(+box_size / 2, +box_size / 2, -box_size / 2);
        glVertex3f(+box_size / 2, -box_size / 2, -box_size / 2);
        glEnd();
        glPopMatrix();
    };
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(30, 30, -30, 0, 0, 0, 0, -1, 0);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            glStencilFunc(GL_ALWAYS, y * 8 + x + 1, 0xff);
            glPushMatrix();
            glTranslated((x - 3.5) * box_size, (y - 3.5) * box_size, 0);
            glColor3d(0.0, 1.0, 0.0);
            glutSolidCube(box_size);
            glPopMatrix();
        }
    }
    for (int i = 1; i <= 64; ++i) {
        glColor3d(0.0, 0.0, 0.0);
        draw_nth_boxs_outline(i);
    }
    unsigned int index;
    glReadPixels(mouse_x, h - mouse_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
    if (index != 0) {
        glColor3d(1.0, 0.0, 0.0);
        draw_nth_boxs_outline(index);
    }
    glFlush();
}
void mouse_moves(int x, int y)
{
    mouse_x = x;
    mouse_y = y;
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_STENCIL);
    glutInitWindowSize(640, 480);
    glutCreateWindow("window");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutPassiveMotionFunc(mouse_moves);
    glutMainLoop();
    return 0;
}
