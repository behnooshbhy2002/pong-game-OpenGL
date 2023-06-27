#include <string>
#include <math.h>
#include <gl\gl.h>
#include "GL/freeglut.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include<iostream>
#include <winuser.h>
#define drawOneLine(x1,y1,x2,y2)  glBegin(GL_LINES);  glVertex2f((x1),(y1));  glVertex2f((x2),(y2));  glEnd();

using namespace std;

//display width and height
int width = 500;
int height = 300;
int time = 15;

//score
int score_left = 0;
int score_right = 0;

//players shape
int player_w = 8;
int player_h = 70;
int player_speed = 3;

//left player vertex
float playerLeft_x = 10.0f;
float playerLeft_y = 50.0f;

//right player vertex
float playerRight_x = width - player_w - 10;
float playerRight_y = 50;

//ball
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 12;
int ball_speed = 2;
int ball_speed_increase = 0.8;

//initial functions
void update(int value);
void view(int width, int height);
void printScores(float x, float y, std::string text);
void display();
void showPlayers(float x, float y, float width, float height);
void playerMovment();
void ballAction();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pong Game");
    glutDisplayFunc(display);
    glutTimerFunc(time, update, 0);
    view(width, height);
    glutMainLoop();
    return 0;
}
void update(int value) {
    playerMovment();
    ballAction();
    glutTimerFunc(time, update, 0);
    glutPostRedisplay();
}

void view(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
}
void printScores(float x, float y, string text) {
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text.c_str());
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //show stipple line in center
    glColor3ub(255,255,255);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF);
    drawOneLine(250, 0, 250, 300);
    glDisable(GL_LINE_STIPPLE);

    // print score
    printScores(width / 2 - 22, height - 24, to_string(score_left)+ "   " + to_string(score_right));

    // show players
    glColor3ub(0,0,255);
    showPlayers(playerLeft_x, playerLeft_y, player_w, player_h);
    glColor3ub(255,0,0);
    showPlayers(playerRight_x, playerRight_y, player_w, player_h);

    //showBall
    glColor3ub(255,255,0);
    showPlayers(ball_pos_x, ball_pos_y, ball_size, ball_size);

    glFlush();
    glutSwapBuffers();
}
void showPlayers(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
void playerMovment() {

    // right player movements
    if (GetAsyncKeyState(VK_UP)){
        if(playerRight_y + player_h + player_speed < height)
            playerRight_y += player_speed;
    }
    if (GetAsyncKeyState(VK_DOWN)){
        if(playerRight_y - player_speed > 0)
            playerRight_y -= player_speed;
    }

    // left player movements
    if (GetAsyncKeyState(0x57)) { //key-S
        if(playerLeft_y + player_h + player_speed < height)
            playerLeft_y += player_speed;
    }
    if (GetAsyncKeyState(0x53)){ //key-X
        if(playerLeft_y - player_speed > 0)
            playerLeft_y -= player_speed;
    }
}
void ballAction() {
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;
    //cout<<ball_dir_x<<" "<<ball_dir_y<<endl;

    // ball hit left player -> changing ball directions
    if (ball_pos_x < playerLeft_x + player_w &&
        ball_pos_x > playerLeft_x &&
        ball_pos_y < playerLeft_y + player_h &&
        ball_pos_y > playerLeft_y)
    {
        float diff = ((ball_pos_y - playerLeft_y) / player_h) - 0.5f;
        ball_dir_x = fabs(ball_dir_x);
        ball_dir_y = diff;
    }

    // ball hit right player -> changing ball directions
    if (ball_pos_x + ball_size > playerRight_x &&
        ball_pos_x + ball_size < playerRight_x + player_w &&
        ball_pos_y < playerRight_y + player_h &&
        ball_pos_y > playerRight_y)
    {
        //cout<<ball_pos_x<<" "<<ball_pos_y<<" "<<playerRight_x<<" "<<playerRight_x+ player_w<<" "<<playerRight_y<<endl;
        float diff = ((ball_pos_y - playerRight_y) / player_h) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x);
        ball_dir_y = diff;
    }

    //ball hit top wall, changing direction y to negative
    if (ball_pos_y + ball_size > height) {
        ball_dir_y = -fabs(ball_dir_y);
    }

    //ball hit bottom wall, changing direction y to positive
    if (ball_pos_y < 0) {
        ball_dir_y = fabs(ball_dir_y);
    }

    //ball cross left wall , one score for player right
    if (ball_pos_x < 0) {
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x);
        ball_dir_y = 0;
        ball_speed+=ball_speed_increase;
        score_right++;
    }

    //ball cross right wall , one score for player left
    if (ball_pos_x > width) {
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = -fabs(ball_dir_x);
        ball_dir_y = 0;
        ball_speed+=ball_speed_increase;
        score_left++;
    }
}