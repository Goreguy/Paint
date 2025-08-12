#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QFrame>
#include <vector>
#include <memory>
#include <QMouseEvent>
#include <QPainter>
#include <QFile>

#include "States.h"
#include "Shapes/BaseShape.h"
#include "Shapes/rectangle.h"
#include "Shapes/ellipse.h"
#include "Shapes/triangle.h"
#include "connection.h"

class PaintArea: public QFrame
{
    Q_OBJECT
public:
    PaintArea(QFrame* parent = nullptr);
    void setTool(ToolType toolType);
    QPainter* painter;

protected:
    void paintEvent(QPaintEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void keyPressEvent(QKeyEvent *evt) override;

private:
    ToolType currentTool = ToolType::NoneTool;
    QPoint startPoint;
    QPoint currentPoint;
    bool drawingAll = false;
    bool drawingShape = false;
    bool drawingLink = false;
    bool connectingShapes = false;
    BaseShape* currentShape = nullptr;


    std::vector<std::unique_ptr<BaseShape>> shapes;
    std::vector<std::unique_ptr<Connection>> connections;

    BaseShape* pickShapeAt(const QPoint &pt);
    void removeShape(BaseShape* s);

    BaseShape* connectionStartFromShape = nullptr;
    BaseShape* movingShape = nullptr;
    QPoint lastMousePos;

    std::unordered_map<ToolType, std::function<void(const QRect&)>> drawMap;

    void cancelOperations();
    void drawShape();
    void drawLink();

    typedef void (PaintArea::*ToolMethod)();
    std::unordered_map<ToolType, ToolMethod> mousePressEventsMap;

};

#endif // PAINTAREA_H
