#define GLUT_DISABLE_ATEXIT_HACK   // اصلاح مشكلة exit مع Visual Studio

#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

// إعدادات نافذة
int winWidth = 600, winHeight = 600;

// حالة الوجه: 0 = smile, 1 = sad
int faceState = 0;

// رسم دائرة (مملوءة) باستخدام triangle fan
void DrawCircle(float cx, float cy, float r, int num_segments = 100)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// رسم قوس (جزء من دائرة) — من angleStart إلى angleEnd بالدرجات
void DrawArc(float cx, float cy, float r, float angleStartDeg, float angleEndDeg, int segments = 60)
{
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; ++i)
    {
        float t = (float)i / (float)segments;
        float angleDeg = angleStartDeg + t * (angleEndDeg - angleStartDeg);
        float angleRad = angleDeg * 3.14159265f / 180.0f;
        float x = cx + r * cosf(angleRad);
        float y = cy + r * sinf(angleRad);
        glVertex2f(x, y);
    }
    glEnd();
}

void OnDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // نرسم في نظام إحداثيات مريح: مركز الشاشة (0,0) بمقاس [-1,1]
    // الوجه الخارجي
    glColor3f(1.0f, 0.9f, 0.3f); // أصفر فاتح
    DrawCircle(0.0f, 0.0f, 0.8f, 200);

    // حواف الوجه (حد أصفر داكن)
    glColor3f(0.9f, 0.6f, 0.1f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 100; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(100);
        float x = 0.8f * cosf(theta);
        float y = 0.8f * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();

    // العيون
    glColor3f(0.0f, 0.0f, 0.0f); // أسود
    DrawCircle(-0.28f, 0.28f, 0.08f, 50);
    DrawCircle(0.28f, 0.28f, 0.08f, 50);

    // بريق في العينين (صغير أبيض)
    glColor3f(1.0f, 1.0f, 1.0f);
    DrawCircle(-0.25f, 0.32f, 0.02f, 10);
    DrawCircle(0.31f, 0.32f, 0.02f, 10);

    // الفم (يعتمد على الحالة)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(6.0f);

    if (faceState == 0)
    {
        // مبتسم: قوس لأعلى (من 200deg إلى 340deg)
        DrawArc(0.0f, -0.1f, 0.45f, 200.0f, 340.0f, 80);
    }
    else
    {
        // حزين: قوس لأسفل (من 20deg إلى 160deg)
        DrawArc(0.0f, -0.5f, 0.45f, 20.0f, 160.0f, 80);
    }

    glutSwapBuffers();
}

void OnReshape(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // نستخدم شاشة مربعة نسبياً: نحافظ على نسبة العرض إلى الارتفاع
    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f)
    {
        // عريض
        glOrtho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    }
    else
    {
        // طويل
        glOrtho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
}

void OnKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;

    case '1':
        faceState = 0; // smile
        glutPostRedisplay();
        break;

    case '2':
        faceState = 1; // sad
        glutPostRedisplay();
        break;

    case ' ':
        faceState = 1 - faceState; // toggle
        glutPostRedisplay();
        break;

    default:
        break;
    }
}

int main(int argc, char** argv)
{
    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Smiley / Sad Face - OpenGL");

    glutDisplayFunc(OnDisplay);
    glutReshapeFunc(OnReshape);
    glutKeyboardFunc(OnKeyPress);

    // تهيئة خلفية بيضاء
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutMainLoop();
    return 0;
}
