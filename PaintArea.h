#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QFrame>
#include "States.h"
#include <vector>
#include <memory>
#include <QMouseEvent>
#include "Shapes/BaseShape.h"
#include "Shapes/rectangle.h"
#include <QPainter>
#include <QFile>


class PaintArea: public QFrame
{
    Q_OBJECT
public:
    PaintArea(QFrame* parent = nullptr);
    void setTool(ToolType toolType);

protected:
    void paintEvent(QPaintEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void keyPressEvent(QKeyEvent *evt) override;

private:
    ToolType currentTool = ToolType::None;
    QPoint startPoint;
    QPoint currentPoint;
    bool drawing = false;

    std::vector<std::unique_ptr<BaseShape>> shapes;
    // std::vector<std::unique_ptr<Connection>> connections;

    BaseShape* pickShapeAt(const QPoint &pt);
    void removeShape(BaseShape* s);

    BaseShape* connectionStart = nullptr;

    BaseShape* movingShape = nullptr;
    QPoint lastMousePos;
};

#endif // PAINTAREA_H
