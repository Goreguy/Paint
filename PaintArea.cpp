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

    // сделать мап для инструментов
    mousePressEventsMap = {
        { ToolType::RectangleTool, &PaintArea::drawShape},
        { ToolType::EllipseTool, &PaintArea::drawShape},
        { ToolType::TriangleTool, &PaintArea::drawShape},
        { ToolType::LinkTool, &PaintArea::drawLink},
        { ToolType::MoveTool, &PaintArea::moveShape},
        { ToolType::EraseTool, &PaintArea::removeShape}
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
    for (int i = (int)shapes.size()-1; i >= 0; --i)
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

    //рисовка фигур и связей
    for (const auto &s : shapes) s->draw(&p);
    for (const auto &c : connections) c->draw(p);

    if (!drawingAll) return;

    //превью связи
    if (drawingLink)
    {
        setMouseTracking(true);
        if (!connectionStartFromShape)
        {
            cancelOperations();
            return;
        }
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
        return;
    }

    if (evt->button() == Qt::RightButton)
    {
        cancelOperations();
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *evt)
{
    if (!mousePressEventsMap.contains(currentTool)) return;

    currentPoint = evt->pos();

    if (drawingLink)
    {
        this->update();
        return;
    }

    if (movingShape)
    {
        QPoint delta = currentPoint - lastMousePos;
        movingShape->moveShape(delta);
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
    if (!mousePressEventsMap.contains(currentTool)) return;

    if (drawingShape)
    {
        drawingShape = false;
        drawingAll = false;
        return;
    }

    if (movingShape)
    {
        movingShape = nullptr;
        setCursor(Qt::ArrowCursor);
        update();
    }


}

void PaintArea::keyPressEvent(QKeyEvent *evt)
{
    if (evt->key() == Qt::Key_Escape)
    {
        cancelOperations();
    }
}

void PaintArea::removeShape()
{
    BaseShape *sToRemove = pickShapeAt(startPoint);
    if (sToRemove)
    {
        for(auto it = connections.begin(); it != connections.end();)
        {
            Connection* c = it->get();
            if (c->getFirstShape() == sToRemove || c->getSecondShape() == sToRemove)//ну хз хз
            {
                if(c->getFirstShape() != sToRemove) c->getFirstShape()->removeConnection(c);
                if(c->getSecondShape() != sToRemove) c->getSecondShape()->removeConnection(c);
                it = connections.erase(it);
            }
            else ++it;
        }

        auto it = std::find_if(shapes.begin(), shapes.end(),
                               [sToRemove](const std::unique_ptr<BaseShape>& s) {
                                   return s.get() == sToRemove;
                               });

        if (it != shapes.end())
        {
            shapes.erase(it);
        }
        this->update();
    }
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
    if (!drawingLink)
    {
        drawingLink = true;
        BaseShape *s = pickShapeAt(startPoint);

        if (!s)
        {
            cancelOperations();
            return;
        }

        connectionStartFromShape = s;
        drawingAll = true;
        this ->update();
        return;
    }

    BaseShape* endShape = pickShapeAt(currentPoint);
    if (connectionExists(connectionStartFromShape, endShape))
    {
        cancelOperations();
        return;
    }

    if (endShape && endShape != connectionStartFromShape)//продумать безопасное усл!
    {
        Connection *c = new Connection(connectionStartFromShape, endShape);
        connections.emplace_back(c);
        connectionStartFromShape->addConnection(c);
        endShape->addConnection(c);
    }

    cancelOperations();

}

void PaintArea::cancelOperations()
{
    if (drawingShape)
    {
        drawingShape = false;
        shapes.pop_back();
    }

    setMouseTracking(false);
    drawingLink = false;
    currentShape = nullptr;
    drawingAll = false;
    connectionStartFromShape = nullptr;

    if (movingShape) movingShape->moveShape(startPoint - movingShape->center());
    movingShape = nullptr;
    setCursor(Qt::ArrowCursor);
    this->update();
}

bool PaintArea::connectionExists(const BaseShape* a, const BaseShape* b)
{
    for (const auto &c : connections)
    {
        if (c->getFirstShape() == a && c->getSecondShape() == b) return true;
        if (c->getFirstShape() == b && c->getSecondShape() == a) return true;
    }
    return false;
}

void PaintArea::moveShape()
{
    BaseShape* s = pickShapeAt(startPoint);

    if (s)
    {
        movingShape = s;
        lastMousePos = startPoint;
        setCursor(Qt::ClosedHandCursor);
        startPoint = s->center();
    }

}

bool PaintArea::saveFile(const QString &filePath)
{

    QFile f(filePath);
    if(!f.open(QIODevice::WriteOnly)) return false;
    QDataStream out(&f);
    out.setVersion(QDataStream::Qt_5_15);

    out << (quint32) shapes.size();
    for(const auto &s : shapes) s->serialize(out);

    out << (quint32)connections.size();
    for(const auto &c : connections) c->serialize(out);

    return true;
}

bool PaintArea::loadFile(const QString &filePath)
{

    QFile f(filePath);
    if(!f.open(QIODevice::ReadOnly)) return false;
    QDataStream in(&f);
    in.setVersion(QDataStream::Qt_5_15);

    connections.clear();
    shapes.clear();
    cancelOperations();//проверить правильный возврат!

    std::unordered_map<quint32, BaseShape*> shapeMap;
    quint32 nShapes;
    in >> nShapes;
    for(quint32 i = 0; i < nShapes; ++i)
    {
        auto s = BaseShape::deserialize(in);
        shapeMap[s->getId()] = s.get();
        shapes.emplace_back(std::move(s));
    }

    quint32 nCon;
    in >> nCon;

    for (quint32 i = 0; i < nCon; ++i)
    {
        auto [fromId, toId] = Connection::deserialize(in);

        auto itFrom = shapeMap.find(fromId);
        auto itTo   = shapeMap.find(toId);

        if (itFrom != shapeMap.end() && itTo != shapeMap.end())
        {
            auto c = std::make_unique<Connection>(itFrom->second, itTo->second);
            itFrom->second->addConnection(c.get());
            itTo->second->addConnection(c.get());
            connections.push_back(std::move(c));
        }
    }

    this->update();
    return true;
}
