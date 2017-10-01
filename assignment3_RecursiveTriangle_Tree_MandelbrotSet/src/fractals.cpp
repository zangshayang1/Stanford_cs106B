/*
 * File: fractals.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains fractal problems for CS106B.
 */


#include "fractals.h"
#include <cmath>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    if (x < 0 || y < 0 || size <= 0 || order <= 0) {
        return ;
    }
    if (order == 1) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + 0.5 * size, y + 0.5 * sqrt(3) * size);
        gw.drawLine(x + size, y, x + 0.5 * size, y + 0.5 * sqrt(3) * size);
        return ;
    }
    else {
        // left
        drawSierpinskiTriangle(gw, x, y, 0.5 * size, order - 1);
        // right
        drawSierpinskiTriangle(gw, x + 0.5 * size, y, 0.5 * size, order - 1);
        //bottom
        drawSierpinskiTriangle(gw, x + 0.25 * size, y + 0.25 * sqrt(3) * size, 0.5 * size, order - 1);
    }

}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */

void drawBranches(GWindow & gw, double x, double y, double length, int order);

void drawTree(GWindow& gw, double x, double y, double size, int order) {
    if (x < 0 || y < 0 || size <= 0 || order <= 0) {
        return ;
    }
    double root_x = x + 0.5 * size;
    double root_y = y + size;
    if (order == 1) {
        gw.drawPolarLine(root_x, root_y, 0.5 * size, 90);
        return ;
    }
    else {
        gw.drawPolarLine(root_x, root_y, 0.5 * size, 90);
        drawBranches(gw, root_x, root_y - 0.5 * size, 0.25 * size, order - 1);
    }
}

void drawBranches(GWindow & gw, double x, double y, double length, int order) {
    gw.drawPolarLine(x, y, length, 135);
    gw.drawPolarLine(x, y, length, 120);
    gw.drawPolarLine(x, y, length, 105);
    gw.drawPolarLine(x, y, length, 90);
    gw.drawPolarLine(x, y, length, 75);
    gw.drawPolarLine(x, y, length, 60);
    gw.drawPolarLine(x, y, length, 45);
    if (order == 1) {
        return ;
    }
    else {
        // mid
        drawBranches(gw, x, y - length, 0.5 * length, order - 1);
        // left 45
        drawBranches(gw, x - 0.5 * sqrt(2) * length, y - 0.5 * sqrt(2) * length, 0.5 * length, order - 1);
        // left 30
        drawBranches(gw, x - 0.5 * length, y - 0.5 * sqrt(3) * length, 0.5 * length, order - 1);
        // left 15
        drawBranches(gw, x - 0.25 * (sqrt(6) - sqrt(2)) * length, y - 0.25 * (sqrt(2) + sqrt(6)) * length, 0.5 * length, order - 1);
        // right 15
        drawBranches(gw, x + 0.25 * (sqrt(6) - sqrt(2)) * length, y - 0.25 * (sqrt(2) + sqrt(6)) * length, 0.5 * length, order - 1);
        // right 30
        drawBranches(gw, x + 0.5 * length, y - 0.5 * sqrt(3) * length, 0.5 * length, order - 1);
        // right 45
        drawBranches(gw, x + 0.5 * sqrt(2) * length, y - 0.5 * sqrt(2) * length, 0.5 * length, order - 1);
    }
}



/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth(); // 575
    int height = gw.getCanvasHeight(); // 466
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    cout << pixels.numRows() << endl;
    cout << pixels.numCols() << endl;

    for (int i = 0; i < pixels.numRows(); i++) {
        for (int j = 0; j < pixels.numCols(); j++) {
            Complex cpx = Complex(minX + i * incX, minY + j * incY);
            int iterNum = mandelbrotSetIterations(cpx, maxIterations);
            if (iterNum >= maxIterations) {
                pixels[i][j] = palette[iterNum % palette.size()];
            }
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex c, int maxIterations) {
    return maxIterations - mandelbrotSetIterations(Complex(0, 0), c, maxIterations);
}
/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    z = z * z + c;
    if (remainingIterations <= 0 || z.abs() > 4) {
        // diverging
        return remainingIterations;
    }
    else {
        return mandelbrotSetIterations(z, c, remainingIterations - 1);
    }
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
