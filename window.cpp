#include <QPainter>
#include <math.h>
#include <qmessagebox.h>
#include <stdio.h>
#include <stdlib.h>

#include "window.h"
//#include "calcapprox.h"
#include "interpolate.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 300
#define DEFAULT_NN 5
#define MINIMUM_POINTS_COUNT 5

static double f_0(double x)
{
    return x;
}

static double df_0(double x)
{
    x += 1;
    return 1.0;
}

static double d2f_0(double x)
{
    x += 1;
    return 0;
}

static double f_1(double x)
{
    return x * x * x;
}

static double df_1(double x)
{
    return 3 * x * x;
}

static double d2f_1(double x)
{
    return 6 * x;
}

static double f_2(double x)
{
    x++;
    return 1;
}

static double df_2(double x)
{
    x++;
    return 0;
}

static double d2f_2(double x)
{
    x += 1;
    return 0;
}

static double f_3(double x)
{
    return sin(x);
}

static double df_3(double x)
{
    return cos(x);
}

static double d2f_3(double x)
{

    return (-1) * sin(x);
}

void fillFunctionValues(DataPoints &values, double a, double b,
                        double (*f)(double), int extend)
{
    int n = values.N;
    double dx = 0;

    if (values.N > 1) {

        if (extend) {
            if (values.N <= 5) {
                dx = 0;
            } else {
                dx = (b - a) / (values.N - 5);
                a -= extend * dx;
                b += extend * dx;
            }
        } else {
            dx = (b - a) / (values.N - 1);
        }
    }

    for (int i = 0; i < n; i++) {
        double xi = a + i * dx;
        values.x[i] = xi;
        if (f != NULL)
            values.y[i] = f(xi);
    }
}

void Window::resetDefaults()
{
    n = DEFAULT_N;
    A = DEFAULT_A;
    B = DEFAULT_B;
    N = DEFAULT_NN;
    mf0 = true;
    mf1 = true;
    mf2 = true;
    func_id = 0;
    fileflag = 0;
    change_func();
}

Window::Window(QWidget *parent) : QWidget(parent)
{
    resetDefaults();
}

// QSize Window::minimumSizeHint () const
//{
//    return QSize (100, 100);
//}

// QSize Window::sizeHint () const
//{
//  return QSize (1000, 1000);
//}

int Window::parseCommanLineArgs(int argc, char *argv[])
{
    if (argc == 1)
        return 0;

    if (argc == 2) {

        inputFilePath = argv[1];
        fileflag = 1;
    }

    return 0;
}

/// change current function for drawing
void Window::change_func()
{
    func_id = (func_id + 1) % 4;

    switch (func_id) {
    case 0:
        f_name = "f (x) = x";
        f = f_0;
        df = df_0;
        d2f = d2f_0;
        break;
    case 2:
        f_name = "f (x) = x * x * x";
        f = f_1;
        df = df_1;
        d2f = d2f_1;
        break;
    case 1:
        f_name = "f (x) = 1";
        f = f_2;
        df = df_2;
        d2f = d2f_2;
        break;
    case 3:
        f_name = "f (x) = sin (x)";
        f = f_3;
        df = df_3;
        d2f = d2f_3;
        break;
    }
    update();
}

void Window::change_np()
{
    N *= 2;
    printf("N = %d\n", N);
    update();
}

void Window::change_nm()
{
    if (N >= 10)
        N /= 2;
    printf("N = %d\n", N);
    update();
}

void Window::change_Ap()
{
    int step = 2;
    if (A + step + EPS < B) {
        A += step;
    }
    // char r2[30];
    // strcpy(A_value, "A = ");
    // sprintf(r2, "3.f", A);
    // strcat(A_value,r2);

    printf("A = %lf44\n", A);
    update();
}

void Window::change_Am()
{
    A -= 2;
    printf("A = %lf\n", A);
    update();
}

void Window::change_Bp()
{
    B += 2;
    printf("B = %lf\n", B);
    update();
}

void Window::change_Bm()
{
    int step = 2;
    if (A < B - step - EPS) {
        B -= step;
    }
    printf("B = %lf\n", B);
    update();
}
void Window::change_fileflag()
{
    fileflag ^= 1;
    printf("Now%s reading input.txt", (fileflag == 0 ? " not" : ""));
    update();
}
void Window::change_mf1()
{
    mf1 = !mf1;
    update();
}

void Window::change_mf2()
{
    mf2 = !mf2;
    update();
}

void Window::change_mf0()
{
    mf0 = !mf0;
    update();
}

int Window::mkarrays()
{

    return 1;
}
void Window::cleanup() {}
/// render graph

void definePlotRegion(const DataPoints &vals, QRectF &rect)
{
    const double minSize = 0.01;
    QRectF region(vals.x[0], vals.y[0], vals.x[0], vals.y[0]);
    if (vals.N <= 0) {
        rect = QRectF(0.0, 0., 0., minSize);
        return;
    }
    double minX = vals.x[0];
    double maxX = vals.x[0];
    double minY = vals.y[0];
    double maxY = vals.y[0];
    for (int i = 0; i < vals.N; i++) {
        double xi = vals.x[i];
        double yi = vals.y[i];
        if (xi < minX) {
            minX = xi;
        } else if (xi > maxX) {
            maxX = xi;
        }
        if (yi < minY) {
            minY = yi;
        } else if (yi > maxY) {
            maxY = yi;
        }
    }
    if (maxY - minY < minSize) {
        double middle = (minY + maxY) / 2;
        minY = middle - minSize;
        maxY = middle + minSize;
    }
    rect = QRectF(minX, minY, maxX - minX, maxY - minY);
}

void drawHorizontalTick(QPainter &painter, int pxi, int pyi, double yi,
                        int tickHalfLength)
{
    char buffer[16];
    painter.drawLine(pxi - tickHalfLength, pyi, pxi + tickHalfLength, pyi);
    sprintf(buffer, "%.3g", yi);
    int xOffset = -60;
    int yOffset = 8;
    painter.drawText(pxi + xOffset, pyi + yOffset, buffer);
}

void drawHorizontalTick(QPainter &painter, int pyi, double yi,
                        int tickHalfLength)
{
    char buffer[16];
    painter.drawLine(-tickHalfLength, pyi, tickHalfLength, pyi);
    sprintf(buffer, "%0.1f", yi);
    painter.drawText(2 * tickHalfLength, pyi + 2 * tickHalfLength, buffer);
}

void drawVerticalTick(QPainter &painter, int pxi, int pyi, double xi,
                      int tickHalfLength)
{
    char buffer[16];
    painter.drawLine(-pxi, pyi - tickHalfLength, -pxi, pyi + tickHalfLength);
    sprintf(buffer, "%0.1f", xi);
    painter.drawText(pxi, pyi + 4 * tickHalfLength, buffer);
}

void drawVerticalTick(QPainter &painter, int pxi, double xi, int tickHalfLength)
{
    char buffer[16];
    painter.drawLine(-pxi, -tickHalfLength, -pxi, tickHalfLength);
    sprintf(buffer, "%0.1f", xi);
    painter.drawText(pxi, 4 * tickHalfLength, buffer);
}

void Window::drawAxesShifted(QPainter &painter, const PlotScale &scale)
{
    int w = width();
    int h = height();
    painter.translate(0, 0);
    int axX = w / 15;
    int axY = h - h / 20;

    painter.drawLine(0, axY, w, axY);
    painter.drawLine(axX, 0, axX, h);

    int nTicks = 5;
    int cx = w / 2;
    int cy = h / 2;

    double x0 = scale.cx - (cx - axX) / scale.x;
    double y0 = scale.cy + (cy - axY) / scale.y;
    double dx = (w - 2 * axX) / scale.x / nTicks;
    double dy = (2 * axY - h) / scale.y / nTicks;
    int tickHalfLength = 5;
    for (int i = 1; i <= nTicks; i++) {
        double yi = i * dy + y0;
        double xi = i * dx + x0;
        int pyi = (y0 - yi) * scale.y + axY;
        int pxi = (xi - x0) * scale.x + axX;
        drawHorizontalTick(painter, axX, pyi, yi, tickHalfLength);
        drawVerticalTick(painter, pxi, axY, xi, tickHalfLength);
    }
    painter.translate(w / 2, h / 2);
}

void Window::drawAxes(QPainter &painter, const PlotScale &scale)
{
    int w = width();
    int h = height();
    painter.translate(0.5 * w, 0.5 * h);
    painter.setPen("black");
    int left = w / 2;
    int right = -w / 2;
    int top = -h / 2;
    int bot = h / 2;
    int axX = (int)(0.05 * w);
    int axY = (int)(0.95 * h);
    painter.drawLine(left, axY, right, axY);
    painter.drawLine(axX, top, 0, bot);
    // draw ticks
    double c = 0.9;
    int nTicks = 3;
    int tickHalfLength = 5;
    double maxY = h / 2 * c;
    double maxX = w / 2 * c;
    double dy = std::floor(maxY / nTicks / scale.y * 10) / 10;
    double dx = std::floor(maxX / nTicks / scale.x * 10) / 10;

    for (int i = 1; i < nTicks + 1; i++) {
        double yi = i * dy;
        double xi = i * dx;
        int pyi = yi * scale.y;
        int pxi = xi * scale.x;
        drawHorizontalTick(painter, -pyi, yi, tickHalfLength);
        drawHorizontalTick(painter, pyi, -yi, tickHalfLength);
        drawVerticalTick(painter, pxi, xi, tickHalfLength);
        drawVerticalTick(painter, -pxi, -xi, tickHalfLength);
    }
}

void drawDataPoints(const DataPoints &vals, QPainter &painter, QPen pen,
                    const PlotScale &scale, bool addCircles, int skipPoints)
{
    double scaleX = scale.x;
    double scaleY = scale.y;
    double centerX = scale.cx;
    double centerY = scale.cy;
    int prevX = -1;
    int prevY = -1;

    painter.setPen(pen);
    const int circleRadius = 6;

    for (int i = skipPoints; i < vals.N - skipPoints; i++) {
        int px = (int)(scaleX * (vals.x[i] - centerX));
        int py = (int)(scaleY * (centerY - vals.y[i]));

        if (addCircles) {
            painter.drawEllipse(px - circleRadius / 2, py - circleRadius / 2,
                                circleRadius, circleRadius);
        } else {
            if (i > 0) {
                painter.drawLine(prevX, prevY, px, py);
            }
        }
        prevX = px;
        prevY = py;
    }
}

void definePlotScale(QRectF region, int width, int height, PlotScale &scale)
{
    double c = 0.8;

    double maxY = std::max(std::abs(region.top()), std::abs(region.bottom()));
    double maxX = std::max(std::abs(region.left()), std::abs(region.right()));

    double minVal = 0.05; // minumum value to constrain maximum scale
    if (maxX < minVal) {
        maxX = minVal;
    }
    if (maxY < minVal) {
        maxY = minVal;
    }

    double scaleX = width / region.width() * c;
    double scaleY = height / 2 / maxY * c;
    double centerX = region.left() + region.width() / 2;
    double centerY = 0.;

    scale = PlotScale{scaleX, scaleY, centerX, centerY};
}

void showErrorMessage(const char *msg)
{
    if (msg == NULL)
        return;
    QMessageBox messageBox;
    messageBox.critical(0, "Error", msg);
}

double calculateError(const DataPoints &dp1, const DataPoints &dp2,
                      DataPoints &res)
{
    assert(dp1.N == dp2.N);
    double maxErr = 0.;
    for (int i = 0; i < dp1.N; i++) {
        res.x[i] = dp1.x[i];
        double err = std::abs(dp1.y[i] - dp2.y[i]);
        res.y[i] = err;
        if (err > maxErr) {
            maxErr = err;
        }
    }
    return maxErr;
}

void Window::drawPlots()
{
    QPainter painter(this);

    std::ifstream inputFile;
    int pointsCount = N + 4;
    if (fileflag != 0) {
        inputFile.open(inputFilePath);
        if (inputFile.bad()) {
            QMessageBox messageBox;
            messageBox.critical(0, "Error", "Input file not found!");
            fileflag = 0;
        } else {
            inputFile >> pointsCount;
            if (pointsCount < MINIMUM_POINTS_COUNT) {
                showErrorMessage(
                    "File data is not enought to perform interpolation");
                return;
            }
        }
    }

    DataPoints functionValues(pointsCount);
    double leftBorder = A;
    double rightBorder = B;

    if (fileflag != 0) {
        if (!functionValues.read(inputFile)) {
            fileflag = 0;
            showErrorMessage("Failed to read file");
        } else {
            leftBorder = functionValues.x[0];
            rightBorder = functionValues.x[pointsCount - 1];
        }
    }
    if (fileflag == 0) {
        fillFunctionValues(functionValues, leftBorder, rightBorder, f, 2);
    }

    DataPoints interpAkima(n);
    DataPoints interpSp(n);

    double akimaLeft = functionValues.x[2];
    double akimaRight = functionValues.x[functionValues.N - 3];
    fillFunctionValues(interpAkima, akimaLeft, akimaRight, NULL);
    double splinesLeftBorder = akimaLeft;
    double splinesRightBorder = akimaRight;
    if (fileflag) {
        splinesLeftBorder = functionValues.x[1];
        splinesRightBorder = functionValues.x[functionValues.N - 2];
    }
    fillFunctionValues(interpSp, splinesLeftBorder, splinesRightBorder, NULL);
    interpolateSplines(functionValues, interpSp);
    interpolateAkima(functionValues, interpAkima);
    DataPoints errAkima(n);
    DataPoints errSplines(n);

    DataPoints targetValues(n);
    QRectF plotRect;
    definePlotRegion(interpSp, plotRect);
    //    painter.setPen("green");

    int skipPoints = 1;
    double maxErr = 0.;
    double maxErrAkima = 0.;
    double maxErrSplines = 0.;
    if (!fileflag) {
        fillFunctionValues(targetValues, leftBorder, rightBorder, f, 0);
        maxErrAkima = calculateError(targetValues, interpAkima, errAkima);
        maxErrSplines = calculateError(targetValues, interpSp, errSplines);

        maxErr = maxErrAkima;
        if (maxErrAkima > maxErrSplines) {
            maxErr = maxErrAkima;
        }
        skipPoints = 2;
    }

    if (maxErr > plotRect.top()) {
        plotRect.setTop(maxErr);
    }
    PlotScale scale;
    definePlotScale(plotRect, width(), height(), scale);
    drawAxesShifted(painter, scale);
    if (!fileflag) {
        if (mf0)
            drawDataPoints(targetValues, painter, QPen(Qt::darkGray, 3), scale,
                           false);
        if (mf1)
            drawDataPoints(errAkima, painter, QPen(Qt::red, 3), scale, false);
        if (mf2)
            drawDataPoints(errSplines, painter, QPen(Qt::green, 3), scale,
                           false);
    }

    drawDataPoints(functionValues, painter, QPen(Qt::darkBlue, 3), scale, true,
                   skipPoints);
    if (mf1) {
        drawDataPoints(interpAkima, painter, QPen(Qt::darkRed, 3), scale);
    }
    if (mf2)
        drawDataPoints(interpSp, painter, QPen(Qt::darkGreen, 3), scale);
    // add Function name
    painter.setPen("blue");
    int offsetY = 20;
    painter.drawText(-width() / 2 + 4 * offsetY, -height() / 2 + offsetY,
                     f_name);
    char textBuffer[100];
    sprintf(textBuffer, "N=%d", N);
    painter.drawText(-width() / 2 + 14 * offsetY, -height() / 2 + offsetY,
                     textBuffer);
    sprintf(textBuffer, "A=%0.2f", leftBorder);
    painter.drawText(-width() / 2 + 20 * offsetY, -height() / 2 + offsetY,
                     textBuffer);
    sprintf(textBuffer, "B=%0.2f", rightBorder);
    painter.drawText(-width() / 2 + 26 * offsetY, -height() / 2 + offsetY,
                     textBuffer);
    if (!fileflag) {
        sprintf(textBuffer, "E1=%0.4f", maxErrAkima);
        painter.drawText(-width() / 2 + 32 * offsetY, -height() / 2 + offsetY,
                         textBuffer);

        sprintf(textBuffer, "E2=%0.4f", maxErrSplines);
        painter.drawText(-width() / 2 + 38 * offsetY, -height() / 2 + offsetY,
                         textBuffer);
    }
}

void Window::paintEvent(QPaintEvent * /* event */)
{
    try {
        drawPlots();
    } catch (std::bad_alloc &e) {
        QMessageBox messageBox;
        messageBox.critical(NULL, "Error",
                            "Not enough memory! Resetting preset");
        resetDefaults();
        drawPlots();
    }
}
