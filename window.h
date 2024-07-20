
#ifndef WINDOW_H
#define WINDOW_H

#include "interpolate.h"
#include <QWidget>

struct PlotScale {
    double x;
    double y;
    double cx;
    double cy;
};

class Window : public QWidget
{
    Q_OBJECT

  private:
    int func_id;
    const char *f_name;
    double A;
    double B;
    int n;
    int N;
    double (*f)(double);
    double (*df)(double);
    double (*d2f)(double);
    // double *derivatives2;
    double f2a;
    double f2b;
    int fileflag;
    bool mf1;
    bool mf2;
    bool mf0;

    std::string inputFilePath;

    void resetDefaults();

  public:
    Window(QWidget *parent);

    //  QSize minimumSizeHint () const;
    //  QSize sizeHint () const;

    int parseCommanLineArgs(int argc, char *argv[]);

    void drawPlots();
    void drawAxesShifted(QPainter &painter, const PlotScale &scale);
  public slots:
    void change_func();
    void change_np();
    void change_nm();
    void change_Ap();
    void change_Am();
    void change_Bp();
    void change_Bm();
    void change_fileflag();
    void change_mf1();
    void change_mf2();
    void change_mf0();

    int mkarrays();
    void cleanup();

  protected:
    void paintEvent(QPaintEvent *event);
    void drawAxes(QPainter &painter, const PlotScale &scale);
};

void drawDataPoints(const DataPoints &vals, QPainter &painter, QPen pen,
                    const PlotScale &scale, bool drawCircles = false,
                    int skipPoints = 0);
double calculateError(const DataPoints &dp1, const DataPoints &dp2,
                      DataPoints &res);
void fillFunctionValues(DataPoints &values, double a, double b,
                        double (*f)(double), int extend = 0);
void definePlotRegion(const DataPoints &vals, QRectF &rect);
void drawHorizontalTick(QPainter &painter, int pxi, int pyi, double yi,
                        int tickHalfLength);

void drawHorizontalTick(QPainter &painter, int pyi, double yi,
                        int tickHalfLength);
void drawVerticalTick(QPainter &painter, int pxi, int pyi, double xi,
                      int tickHalfLength);
void drawVerticalTick(QPainter &painter, int pxi, double xi,
                      int tickHalfLength);
void definePlotScale(QRectF region, int width, int height, PlotScale &scale);
void showErrorMessage(const char *msg);

#endif
