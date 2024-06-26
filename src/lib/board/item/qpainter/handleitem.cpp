#include "handleitem.h"
#include "resizablehandlerect.h"

#include <QGraphicsSceneMouseEvent>

BOARD_PAINTER_NAMESPACE_USE
HandleItem::HandleItem(Position position) :
    handlePosition(position)
{
    setFlag(QGraphicsItem::ItemIsMovable);

}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch (handlePosition) {

    case TopLeft:
    {

        ResizableHandleRect * rectItem = dynamic_cast<ResizableHandleRect *>( parentItem());
        if(rectItem){

            QRectF boundingFrameRect = rectItem->rectBounds();

            boundingFrameRect.setTop(event->pos().y() + 6);
            boundingFrameRect.setLeft(event->pos().x() +6);

            rectItem->setRectBounds(boundingFrameRect.normalized());
        }
    }
        break;

    case TopRight:
    {
        ResizableHandleRect * rectItem = dynamic_cast<ResizableHandleRect *>( parentItem());
        if(rectItem){

            QRectF boundingFrameRect = rectItem->rectBounds();

            boundingFrameRect.setTop(event->pos().y() + 6);
            boundingFrameRect.setRight(event->pos().x() -6);

            rectItem->setRectBounds(boundingFrameRect.normalized());

        }

    }
        break;

    case BottomRight:
    {
        ResizableHandleRect * rectItem = dynamic_cast<ResizableHandleRect *>( parentItem());
        if(rectItem){

            QRectF boundingFrameRect = rectItem->rectBounds();

            boundingFrameRect.setRight(event->pos().x() - 6);
            boundingFrameRect.setBottom(event->pos().y() -6);

            rectItem->setRectBounds(boundingFrameRect.normalized());

        }

    }
        break;

    case BottomLeft:
    {
        ResizableHandleRect * rectItem = dynamic_cast<ResizableHandleRect *>( parentItem());
        if(rectItem){

            QRectF boundingFrameRect = rectItem->rectBounds();

            boundingFrameRect.setBottom(event->pos().y() -6);
            boundingFrameRect.setLeft(event->pos().x() + 6);

            rectItem->setRectBounds(boundingFrameRect.normalized());
        }

    }
        break;

    }

}
