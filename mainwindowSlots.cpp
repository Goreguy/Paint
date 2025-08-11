#include "mainwindow.h"

void MainWindow::slotButtonPressed()
{
    ToolType buttonType = mapButtonTool[sender()];
    paintArea->setTool(buttonType);


}
