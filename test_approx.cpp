#include "test_approx.h"
#include "window.h"
#include <QMainWindow>

void TestApprox::testZero()
{
    QMainWindow main;
    Window window(&main);

    QTest::keySequence(&window, QKeySequence(Qt::CTRL + Qt::Key_C));
    QTest::keySequence(&window, QKeySequence(Qt::CTRL + Qt::Key_X));
}

QTEST_MAIN(TestApprox)
