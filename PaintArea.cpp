#include "PaintArea.h"

PaintArea::PaintArea(QFrame* parent)
: QFrame(parent)
{
    setObjectName("paintArea");
    setFocusPolicy(Qt::ClickFocus);

    drawMap = {
        { ToolType::Rectangle, [&](const QRect &r) { shapes.emplace_back(std::make_unique<Rectangle>(r)); } },
        { ToolType::Ellipse,   [&](const QRect &r) { shapes.emplace_back(std::make_unique<Ellipse>(r)); } },
        { ToolType::Triangle,  [&](const QRect &r) { shapes.emplace_back(std::make_unique<Triangle>(r)); } }
    };
    ///мапы сделать
    // сделать мап для инструментов
}

void PaintArea::setTool(ToolType toolType)
{

    currentTool = toolType;
    connectionStartFromShape = nullptr;
    movingShape=nullptr;
}

BaseShape* PaintArea::pickShapeAt(const QPoint &pt)
{
    for(int i = (int)shapes.size()-1; i>=0; --i)
    {
        if(shapes[i]->contains(pt))
            return shapes[i].get();
    }
    return nullptr;
}

void PaintArea::paintEvent(QPaintEvent *evt)
{
    Q_UNUSED(evt);
    //if(!drawing) return;

    QPainter p(this);
    p.fillRect(rect(), Qt::white);

    ///////////draw shapes
    for(const auto &s : shapes) s->draw(&p);

    ///////// draw connections
    //for(const auto &c : connections) c->draw(p);
    //типо превью соединения
    if(connectingShapes)
    {
        p.setPen(QPen(Qt::black,1,Qt::DashLine));
        p.drawLine(connectionStartFromShape->center(), currentPoint);
        return;
    }

    QRect r(startPoint, currentPoint);
    r = r.normalized();
    p.setPen(QPen(Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    if(currentTool == ToolType::Rectangle) p.drawRect(r);
    else if(currentTool == ToolType::Ellipse) p.drawEllipse(r);
    else if(currentTool == ToolType::Triangle)
    {
        QPolygon poly;
        poly << QPoint(r.left()+r.width()/2,r.top()) << QPoint(r.right(),r.bottom()) << QPoint(r.left(),r.bottom());
        p.drawPolygon(poly);
    }

}

void PaintArea::mousePressEvent(QMouseEvent *evt)
{
    if(evt->button() == Qt::LeftButton)
    {
        startPoint = evt->pos();
        currentPoint = startPoint;

        if(currentTool == ToolType::Rectangle || currentTool == ToolType::Ellipse || currentTool == ToolType::Triangle)
        {
            drawing = true;
            this->update();
        } /*else if(currentTool==Tool::Connection){
            Shape *s = pickShapeAt(startPoint);
            if(s){ connectionStart = s; drawing = true; update(); }
        } else if(currentTool==Tool::Move){
            Shape *s = pickShapeAt(startPoint);
            if(s){ movingShape = s; lastMousePos = startPoint; setCursor(Qt::ClosedHandCursor); }
        } else if(currentTool==Tool::Delete){
            Shape *s = pickShapeAt(startPoint);
            if(s){ removeShape(s); update(); }
        }*/
        return;
    }
    if(evt->button() == Qt::RightButton)
    {
        // cancel operations
        drawing = false;
        connectionStartFromShape=nullptr;
        movingShape=nullptr;
        setCursor(Qt::ArrowCursor);
        this->update();
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *evt)
{
    currentPoint = evt->pos();
    if(movingShape)
    {
        QPoint delta = currentPoint - lastMousePos;
        //movingShape->translate(delta);
        // connections follow shapes automatically (they reference centers)
        lastMousePos = currentPoint;
        this->update();
    } else if(drawing){
        this->update();
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *evt)
{
    if(evt->button() != Qt::LeftButton) return;

    QPainter p(this);
    if(drawing)
    {
        QRect r(startPoint, currentPoint);
        r = r.normalized();
        if(r.width()>1 && r.height()>1)
        {
            drawMap[currentTool](r);
        }
        drawing = false;

        this->update();
    }
    /*else if(currentTool==Tool::Connection && connectionStart && drawing){
        Shape* end = pickShapeAt(evt->pos());
        if(end && end!=connectionStart){
            // add connection
            Connection *c = new Connection(connectionStart, end);
            connections.emplace_back(c);
            connectionStartFromShape->addConnection(c);
            end->addConnection(c);
        }
        connectionStart = nullptr; drawing=false; update();
    } else if(currentTool==Tool::Move && movingShape){
        movingShape = nullptr; setCursor(Qt::ArrowCursor); update();
    }*/
}

void PaintArea::keyPressEvent(QKeyEvent *evt)
{
    if(evt->key() == Qt::Key_Escape)
    {
        drawing = false;
        connectionStartFromShape=nullptr;
        movingShape = nullptr;
        setCursor(Qt::ArrowCursor);
        this->update();
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
