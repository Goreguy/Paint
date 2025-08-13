#include "mainwindow.h"

void MainWindow::slotButtonPressed()
{

    ToolType buttonType = mapButtonTool[sender()];
    if (fileSaveLoadMap.contains(buttonType))
    {
        (this->*fileSaveLoadMap[buttonType])();
        return;
    }

    paintArea->setTool(buttonType);

}
