#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    toolsGroup = new QActionGroup(this);
    toolsGroup->setExclusive(true);
    toolBar = createToolBar();
    addToolBar(toolBar);


    paintArea = createPaintArea();
    this->setCentralWidget(paintArea);

}

QToolBar* MainWindow::createToolBar()
{
    QToolBar* toolB = new QToolBar(this);

    toolB->setMovable(false);
    createToolOptions(toolB);
    toolB->setStyleSheet( "QToolBar QToolButton { color: black; }" );
    toolB->setFixedHeight(50);

    return toolB;
}

void MainWindow::createToolOptions(QToolBar* toolB)
{
    spacer(toolB, 60);
    toolB->addSeparator();
    QAction* rectangleButton = createAction(QString::fromUtf8("Прямоугольник"), toolB, ToolType::Rectangle);
    spacer(toolB, 30);
    QAction* triangleButton = createAction(QString::fromUtf8("Треугольник"), toolB, ToolType::Triangle);
    spacer(toolB, 30);
    QAction* ellipseButton = createAction(QString::fromUtf8("Эллипс"), toolB, ToolType::Ellipse);
    toolB->addSeparator();
    spacer(toolB);
    toolB->addSeparator();
    QAction* connectButton = createAction(QString::fromUtf8("Связь"), toolB, ToolType::Connect);
    toolB->addSeparator();
    spacer(toolB, 30);
    toolB->addSeparator();
    QAction* moveButton = createAction(QString::fromUtf8("Переместить"), toolB, ToolType::Move);
    spacer(toolB, 30);
    QAction* eraseButton = createAction(QString::fromUtf8("\U0001F5D1"), toolB, ToolType::Erase);
    toolB->addSeparator();
    spacer(toolB);
    toolB->addSeparator();
    QAction* loadFileButton= createAction(QString::fromUtf8("Загрузить"), toolB, ToolType::Load);
    QAction* saveFileButton = createAction(QString::fromUtf8("Сохранить"), toolB, ToolType::Save);
    toolB->addSeparator();
    spacer(toolB, 60);

}

QAction* MainWindow::createAction(const QString &actionName, QToolBar* toolB, const ToolType toolType)
{
    QAction * button = new QAction (toolB);
    button->setObjectName(actionName);
    button->setText(actionName);
    button->setCheckable(true);
    mapButtonTool[button] = toolType;
    toolsGroup->addAction(button);

    connect(button, &QAction::triggered, this, &MainWindow::slotButtonPressed);
    toolB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolB->addAction(button);

    return button;
}

void MainWindow::spacer(QToolBar* toolB, int size)
{
    QWidget* spacer = new QWidget(toolB);
    if (size != 0 ) spacer->setFixedWidth(size);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolB->addWidget(spacer);
}

PaintArea* MainWindow::createPaintArea()
{
    PaintArea* paintAr = new PaintArea();
    return paintAr;
}

MainWindow::~MainWindow()
{
    delete ui;
}
