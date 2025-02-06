#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Constants
#define MAXCOUNT 100
#define ZOOM_DURATION 550 // Milliseconds for zoom animation

// Initial parameters for Mandelbrot set
double left = -2.0;
double top = -1.0;
double xside = 3.0;
double yside = 2.0;

// Parameters for zoom animation
double targetLeft, targetTop, targetXSide, targetYSide;
int startTime;

// Current complex number being displayed
double currentComplexReal = 0.0;
double currentComplexImag = 0.0;

// Text position for displaying complex number
int textPosX = 10;
int textPosY = 580;

// Page indicator: 0 for guide, 1 for Mandelbrot set
int guidePage = 0;

// Function to convert HSL to RGB
void HSLtoRGB(double h, double s, double l, double* r, double* g, double* b);

// Helper function for hue to RGB conversion
double hue2rgb(double p, double q, double t);

// Function to display the guide page
void displayGuide();

// Function to generate the Mandelbrot set and display it
void fractal();

// Function to update the zoom
void updateZoom(int value);

// Function to handle mouse events
void handleMouse(int button, int state, int x, int y);

// Function to handle keyboard events
void keyboard(unsigned char key, int x, int y);

// Main display function
void display() {
    if (guidePage == 0) {
        displayGuide();
    } else {
        fractal();
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mandelbrot Fractals with OpenGL");
    gluOrtho2D(0, 800, 0, 600);

    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(handleMouse);
    glutKeyboardFunc(keyboard);

    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}

// Function to convert HSL to RGB
void HSLtoRGB(double h, double s, double l, double* r, double* g, double* b) {
    if (s == 0.0) {
        *r = *g = *b = l; // achromatic
    } else {
        double q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        double p = 2.0 * l - q;

        *r = hue2rgb(p, q, h + 1.0 / 3.0);
        *g = hue2rgb(p, q, h);
        *b = hue2rgb(p, q, h - 1.0 / 3.0);
    }
}

// Helper function for hue to RGB conversion
double hue2rgb(double p, double q, double t) {
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;
    if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
    if (t < 1.0 / 2.0) return q;
    if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    return p;
}

// Function to display the guide page
void displayGuide() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    // Guide text
    char guideText[4][100] = {
        "Welcome to Mandelbrot Fractals Viewer!",
        "Instructions:",
        " - Left-click: Zoom In",
        " - Right-click: Zoom Out"
    };

    int yOffset = 300;
    for (int i = 0; i < 4; i++) {
        glRasterPos2i(50, yOffset);
        for (int j = 0; j < strlen(guideText[i]); j++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, guideText[i][j]);
        }
        yOffset -= 40;
    }

    // Next page text
    glRasterPos2i(50, 100);
    char nextPageText[] = "Press 'Enter' to start exploring the Mandelbrot set!";
    for (int j = 0; j < strlen(nextPageText); j++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, nextPageText[j]);
    }

    glutSwapBuffers();
}

// Function to generate the Mandelbrot set and display it
void fractal() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    int maxx = glutGet(GLUT_WINDOW_WIDTH);
    int maxy = glutGet(GLUT_WINDOW_HEIGHT);

    double zoomFactorX = xside / maxx;
    double zoomFactorY = yside / maxy;

    double zoomLevel = log2(xside / 3.0); // Calculate zoom level

    // Pre-compute the fixed window size (maxx and maxy are constant)
    for (int y = 0; y < maxy; y++) {
        for (int x = 0; x < maxx; x++) {
            double cx = x * zoomFactorX + left;
            double cy = y * zoomFactorY + top;

            // Update the current complex number
            currentComplexReal = cx;
            currentComplexImag = cy;

            double zx = 0;
            double zy = 0;
            int count = 0;

            while (count < MAXCOUNT && zx * zx + zy * zy < 4.0) {
                double tempx = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = tempx;
                count++;
            }

            // Adjust hue based on zoom level
            double hue = (count % 255) / 255.0 + zoomLevel / 10.0;
            double saturation = 1.0;
            double lightness = 0.5 + (count % 255) / 510.0; // Improved contrast

            double red, green, blue;
            HSLtoRGB(hue, saturation, lightness, &red, &green, &blue);

            glColor3f(red, green, blue);
            glVertex2f(x, y);
        }
    }

    glEnd();

    // Print the current complex number
    char complexText[50];
    sprintf(complexText, "Current Complex Number: %.6f + %.6fi", currentComplexReal, currentComplexImag);
    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    glRasterPos2i(textPosX, textPosY);
    for (int j = 0; j < strlen(complexText); j++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, complexText[j]);
    }

    glutSwapBuffers();
}

// Function to update the zoom
void updateZoom(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - startTime;

    if (elapsedTime < ZOOM_DURATION) {
        double progress = (double)elapsedTime / ZOOM_DURATION;

        left = left + (targetLeft - left) * progress;
        top = top + (targetTop - top) * progress;
        xside = xside + (targetXSide - xside) * progress;
        yside = yside + (targetYSide - yside) * progress;

        glutPostRedisplay();
        glutTimerFunc(16, updateZoom, 0); // 60 FPS
    }
}

// Function to handle mouse events
void handleMouse(int button, int state, int x, int y) {
    if (guidePage == 0) return; // Ignore mouse input when on the guide page

    if (state != GLUT_DOWN) return; // Ignore mouse up events

    double mouseX = (double)x / glutGet(GLUT_WINDOW_WIDTH);
    double mouseY = (double)y / glutGet(GLUT_WINDOW_HEIGHT);

    if (button == GLUT_LEFT_BUTTON) {
        targetLeft = left + (mouseX - 0.5) * xside;
        targetTop = top - (mouseY - 0.5) * yside;
        targetXSide = xside / 2.0;
        targetYSide = yside / 2.0;
    } else if (button == GLUT_RIGHT_BUTTON) {
        targetLeft = left + (mouseX - 0.5) * xside - xside / 4.0;
        targetTop = top - (mouseY - 0.5) * yside + yside / 4.0;
        targetXSide = xside * 2.0;
        targetYSide = yside * 2.0;
    }

    startTime = glutGet(GLUT_ELAPSED_TIME);

    glutTimerFunc(16, updateZoom, 0); // 60 FPS
}


// Function to handle keyboard events
void keyboard(unsigned char key, int x, int y) {
    if (key == 13 && guidePage == 0) { // ASCII code for 'Enter'
        guidePage = 1; // Switch to Mandelbrot set display
        glutPostRedisplay();
    }
}




