#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QHBoxLayout>
#include "PaintArea.h"
#include <QActionGroup>
#include "States.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QToolBar* toolBar;
    QActionGroup *toolsGroup;
    PaintArea* paintArea;

    std::map<QObject*, ToolType> mapButtonTool;

    QToolBar* createToolBar();
    void createToolOptions(QToolBar* toolB);
    QAction* createAction(const QString &actionName, QToolBar* toolB, const ToolType toolType);
    void spacer(QToolBar* toolB, int size = 0);
    PaintArea* createPaintArea();

private slots:
    void slotButtonPressed();
};
#endif // MAINWINDOW_H
