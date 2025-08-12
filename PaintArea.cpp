#include "PaintArea.h"
#include <QDebug>

PaintArea::PaintArea(QFrame* parent)
: QFrame(parent)
{
    setObjectName("paintArea");
    setFocusPolicy(Qt::ClickFocus);

    drawMap = {
        { ToolType::RectangleTool, [&](const QRect &r) { shapes.emplace_back(std::make_unique<Rectangle>(r)); } },
        { ToolType::EllipseTool,   [&](const QRect &r) { shapes.emplace_back(std::make_unique<Ellipse>(r)); } },
        { ToolType::TriangleTool,  [&](const QRect &r) { shapes.emplace_back(std::make_unique<Triangle>(r)); } }
    };
    ///мапы сделать
    // сделать мап для инструментов
    mousePressEventsMap = {
        { ToolType::RectangleTool, &PaintArea::drawShape},
        { ToolType::EllipseTool, &PaintArea::drawShape},
        { ToolType::TriangleTool, &PaintArea::drawShape},
        { ToolType::LinkTool, &PaintArea::drawLink}
    };
}

void PaintArea::setTool(ToolType toolType)
{
    currentTool = toolType;
    connectionStartFromShape = nullptr;
    movingShape = nullptr;
}

BaseShape* PaintArea::pickShapeAt(const QPoint &pt)
{
    for (int i = (int)shapes.size()-1; i>=0; --i)
    {
        if(shapes[i]->contains(pt)) return shapes[i].get();
    }
    return nullptr;
}

void PaintArea::paintEvent(QPaintEvent *evt)
{
    Q_UNUSED(evt);
    QPainter p(this);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.setRenderHint(QPainter::Antialiasing, true);
    p.fillRect(rect(), Qt::white);

    ///////////draw shapes
    for (const auto &s : shapes) s->draw(&p);
    for (const auto &c : connections) c->draw(p);

    if (!drawingAll) return;

    //превью соединения
    if (connectingShapes)
    {
        setMouseTracking(true);
        p.drawLine(connectionStartFromShape->center(), currentPoint);
        return;
    }

    // превью рисунка
    if (drawingShape)
    {
        QRect r(startPoint, currentPoint);
        r = r.normalized();
        currentShape = shapes.back().get();
        currentShape->draw(&p);
    }

}

void PaintArea::mousePressEvent(QMouseEvent *evt)
{
    if (!mousePressEventsMap.contains(currentTool)) return;
    if (evt->button() == Qt::LeftButton)
    {
        startPoint = evt->pos();
        currentPoint = startPoint;
        (this->*mousePressEventsMap[currentTool])();

         /*else if(currentTool==Tool::Move){
            Shape *s = pickShapeAt(startPoint);
            if(s){ movingShape = s; lastMousePos = startPoint; setCursor(Qt::ClosedHandCursor); }
        } else if(currentTool==Tool::Delete){
            Shape *s = pickShapeAt(startPoint);
            if(s){ removeShape(s); update(); }
        }
        return;*/
    }

    if (evt->button() == Qt::RightButton)
    {
        cancelOperations();
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *evt)
{
    currentPoint = evt->pos();

    if (connectingShapes)
    {
        this->update();
        return;
    }
    if (movingShape)
    {
        QPoint delta = currentPoint - lastMousePos;
        //movingShape->translate(delta);
        // connections follow shapes automatically (they reference centers)
        lastMousePos = currentPoint;
        this->update();
        return;
    }
    if (drawingAll)
    {
        QRect r(startPoint, currentPoint);
        currentShape->resize(r);
        this->update();
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *evt)
{
    if (drawingShape)
    {
        drawingShape = false;
        // drawingAll = false;
    }
    /*else if(currentTool==Tool::Move && movingShape){
        movingShape = nullptr; setCursor(Qt::ArrowCursor); update();
    }*/
}

void PaintArea::keyPressEvent(QKeyEvent *evt)
{
    if(evt->key() == Qt::Key_Escape)
    {
        cancelOperations();
    }
}

void PaintArea::removeShape(BaseShape* s)
{
    // // remove associated connections
    // for(auto it = connections.begin(); it != connections.end(); ){
    //     Connection* c = it->get();
    //     if(c->first()==s || c->second()==s){
    //         // notify the other shape
    //         if(c->first()!=s) c->first()->removeConnection(c);
    //         if(c->second()!=s) c->second()->removeConnection(c);
    //         it = connections.erase(it);
    //     } else ++it;
    // }

    // // remove shape
    // auto it = std::find_if(shapes.begin(), shapes.end(), [&](const std::unique_ptr<Shape> &up){ return up.get()==s; });
    // if(it!=shapes.end()) shapes.erase(it);
}

void PaintArea::drawShape()
{
    drawingShape = true;
    QRect r(startPoint, currentPoint);
    drawMap[currentTool](r);
    drawingAll = true;
    this->update();
}

void PaintArea::drawLink()
{
    if(!drawingLink)
    {
        drawingLink = true;
        connectingShapes = true;
        BaseShape *s = pickShapeAt(startPoint);
        if (s)
        {
            connectionStartFromShape = s;
            drawingAll = true;
            this ->update();
        }
        return;
    }

    BaseShape* endShape = pickShapeAt(currentPoint);
    if(endShape && endShape!=connectionStartFromShape)
    {
        Connection *c = new Connection(connectionStartFromShape, endShape);
        connections.emplace_back(c);
        connectionStartFromShape->addConnection(c);
        endShape->addConnection(c);

    }
    drawingLink = false;
    connectingShapes = false;
    connectionStartFromShape = nullptr;
    drawingAll = false;
    this->update();
    setMouseTracking(false);


}

void PaintArea::cancelOperations()
{
    //if (currentTool != ToolType::LinkTool) shapes.pop_back();
    if (drawingShape)
    {
        drawingShape = false;
        shapes.pop_back();
    }
    connectingShapes = false;
    currentShape = nullptr;
    drawingAll = false;
    connectionStartFromShape=nullptr;
    movingShape = nullptr;
    setCursor(Qt::ArrowCursor);
    this->update();
}
