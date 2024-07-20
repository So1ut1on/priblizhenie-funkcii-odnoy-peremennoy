
#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>

#include "window.h"
//#include "calcapprox.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    printf("Ok\n");
    QMainWindow *window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar(window);
    Window *graph_area = new Window(window);
    printf("Ok\n");
    QAction *action;
    printf("Ok\n");

    if (graph_area->parseCommanLineArgs(argc, argv)) {
        printf("Ok\n");
        QMessageBox::warning(0, "Wrong input arguments!",
                             "Wrong input arguments!");
        return -1;
    }

    printf("Ok\n");
    action = tool_bar->addAction("&Change function", graph_area,
                                 SLOT(change_func()));
    action->setShortcut(QString("0"));

    action = tool_bar->addAction("&Increase N", graph_area, SLOT(change_np()));
    action->setShortcut(QString("4"));

    action = tool_bar->addAction("&Decrease N", graph_area, SLOT(change_nm()));
    action->setShortcut(QString("3"));

    action = tool_bar->addAction("&Increase A", graph_area, SLOT(change_Ap()));
    action->setShortcut(QString("6"));

    action = tool_bar->addAction("&Decrease A", graph_area, SLOT(change_Am()));
    action->setShortcut(QString("5"));

    action = tool_bar->addAction("&Increase B", graph_area, SLOT(change_Bp()));
    action->setShortcut(QString("8"));

    printf("Ok\n");
    action = tool_bar->addAction("&Decrease B", graph_area, SLOT(change_Bm()));
    action->setShortcut(QString("7"));

    action = tool_bar->addAction("&Read from file", graph_area,
                                 SLOT(change_fileflag()));
    action->setShortcut(QString("Ctrl+F"));

    action = tool_bar->addAction("&mf1", graph_area, SLOT(change_mf1()));
    action->setShortcut(QString("T"));

    action = tool_bar->addAction("&mf2", graph_area, SLOT(change_mf2()));
    action->setShortcut(QString("Y"));

    action = tool_bar->addAction("&mf0", graph_area, SLOT(change_mf0()));
    action->setShortcut(QString("R"));

    action = tool_bar->addAction("E&xit", window, SLOT(close()));
    action->setShortcut(QString("Ctrl+X"));

    printf("Ok\n");
    tool_bar->setMaximumHeight(30);

    window->setMenuBar(tool_bar);
    window->setCentralWidget(graph_area);
    window->setWindowTitle("Graph");

    window->show();
    return app.exec();
}
