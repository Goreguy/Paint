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

    fileSaveLoadMap = {
        { ToolType::SaveTool, &MainWindow::onActionSave},
        { ToolType::LoadTool, &MainWindow::onActionLoad}
    };

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
    QAction* rectangleButton = createAction(QString::fromUtf8("Прямоугольник"), toolB, ToolType::RectangleTool);
    spacer(toolB, 30);
    QAction* triangleButton = createAction(QString::fromUtf8("Треугольник"), toolB, ToolType::TriangleTool);
    spacer(toolB, 30);
    QAction* ellipseButton = createAction(QString::fromUtf8("Эллипс"), toolB, ToolType::EllipseTool);
    toolB->addSeparator();
    spacer(toolB);
    toolB->addSeparator();
    QAction* connectButton = createAction(QString::fromUtf8("Связь"), toolB, ToolType::LinkTool);
    toolB->addSeparator();
    spacer(toolB, 30);
    toolB->addSeparator();
    QAction* moveButton = createAction(QString::fromUtf8("Переместить"), toolB, ToolType::MoveTool);
    spacer(toolB, 30);
    QAction* eraseButton = createAction(QString::fromUtf8("\U0001F5D1"), toolB, ToolType::EraseTool);
    toolB->addSeparator();
    spacer(toolB);
    toolB->addSeparator();
    QAction* loadFileButton= createAction(QString::fromUtf8("Загрузить"), toolB, ToolType::LoadTool);
    QAction* saveFileButton = createAction(QString::fromUtf8("Сохранить"), toolB, ToolType::SaveTool);
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

void MainWindow::onActionSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save drawing"), QString(), tr("PaintProj (*.image)"));
    if(fileName.isEmpty()) return;
    if(!paintArea->saveFile(fileName)) QMessageBox::warning(this, tr("Save failed"), tr("Could not save file"));
}

void MainWindow::onActionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open drawing"), QString(), tr("PaintProj (*.image)"));
    if(fileName.isEmpty()) return;
    if(!paintArea->loadFile(fileName)) QMessageBox::warning(this, tr("Load failed"), tr("Could not load file"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
