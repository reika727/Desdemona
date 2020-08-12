#include "GL/glut.h"
#include "othello.hpp"
#include "othello_algorithm.hpp"
#include <algorithm>
#include <random>
#include <thread>

#include <iostream>

enum class game_turn {
    player,
    enemy,
};
game_turn turn = game_turn::player;
othello::board b;
int mouse_x = 0, mouse_y = 0;
void play_enemy_async()
{
    std::thread p(
        [] {
            turn = game_turn::enemy;
            try {
                play_best_hand(b, othello::stone::white(), 0);
            } catch (const othello::board::operation_error &) {
                // cant put anywhere
            }
            turn = game_turn::player;
        });
    p.detach();
};
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
    //gluLookAt(40, 0, -10, 0, 0, 0, 0, 0, -1);
    //gluLookAt(0, 0, -30, 0, 0, 0, 0, -1, 0);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            glStencilFunc(GL_ALWAYS, y * 8 + x + 1, 0xff);
            glPushMatrix();
            glTranslatef((x - 3.5) * box_size, (y - 3.5) * box_size, 0);
            glColor3f(0.0, 1.0, 0.0);
            glutSolidCube(box_size);
            glPopMatrix();
        }
    }
    for (int i = 1; i <= 64; ++i) {
        glColor3d(0.0, 0.0, 0.0);
        draw_nth_boxs_outline(i);
    }
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (auto s = b(x, y)) {
                glPushMatrix();
                if (s == othello::stone::black()) {
                    glColor3f(0, 0, 0);
                } else if (s == othello::stone::white()) {
                    glColor3f(1, 1, 1);
                }
                glTranslatef((x - 3.5) * box_size, (y - 3.5) * box_size, -box_size / 2);
                glRotatef(-90, 1, 0, 0);
                std::random_device seed_gen;
                std::default_random_engine engine(seed_gen());
                glRotatef(std::uniform_real_distribution<>(0, 360)(engine), 0, 1, 0);
                glutSolidTeapot(0.5);
                glPopMatrix();
            }
        }
    }
    unsigned int index;
    glReadPixels(mouse_x, glutGet(GLUT_WINDOW_HEIGHT) - mouse_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
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
void mouse_click(int button, int state, int x, int y)
{
    if (turn == game_turn::enemy) return;
    int index;
    glReadPixels(mouse_x, glutGet(GLUT_WINDOW_HEIGHT) - mouse_y, 1, 1, GL_STENCIL_INDEX, GL_INT, &index);
    if (index == 0) return;
    if (auto puttable = b.get_puttable_places(othello::stone::black()); !puttable.empty()) {
        othello::board::coordinate c{(index - 1) % 8, (index - 1) / 8};
        if (std::count(puttable.begin(), puttable.end(), c)) {
            b.put(c, othello::stone::black());
            play_enemy_async();
        } else {
            // cant put here
        }
    } else {
        // cant put anywhere
        play_enemy_async();
    }
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
    glutMouseFunc(mouse_click);
    glutMainLoop();
    return 0;
}
