#include "diagramitem.h"

/*
 *  CONSTRUCTOR
 */

DiagramItem::DiagramItem(QWidget *parent, QString id, QPointF loc)
{
    this->parent = parent;  // save parent item for resizing purposes
    this->itemId = id;      // save the item's id
    this->setPos(loc);      // position the item
}


/*
 *  OUTPUT ITEM MANAGEMENT
 */

QList<DiagramItem *> DiagramItem::outputItems() const
{
    return outputItem;
}

void DiagramItem::addOutputItem(DiagramItem *item)
{
    this->outputItem.append(item);
}

void DiagramItem::removeOutputItem(DiagramItem *item)
{
    for(int i = 0; i < this->outputItem.count(); i++) {
        if(item->id() == outputItem.at(i)->id()) {
            outputItem.removeAt(i);
        }
    }
}

/*
 *  ITEM POSITIONING AND LOCATING
 */

/*
QPointF DiagramItem::convertPointToRelative(QPointF loc, DiagramItem *anchor)
{
    QPointF temp;
    QPointF anchorPoint = anchor->location();

    temp.setX(loc.x() - anchorPoint.x());
    temp.setY(loc.y() - anchorPoint.y());

    return temp;
}

QPointF DiagramItem::convertPointToAbsolute(QPointF loc, DiagramItem *anchor)
{
    QPointF temp;
    QPointF anchorPoint = anchor->location();

    temp.setX(loc.x() + anchorPoint.x());
    temp.setY(loc.y() + anchorPoint.y());

    return temp;
}
*/

// get the point where connectors can enter this item
QPointF DiagramItem::inputPoint() const
{
    return this->pos() + QPoint(this->boundingRect().left(), this->boundingRect().center().y());
}

// get the point where connectors can leave this item
QPointF DiagramItem::outputPoint() const
{
    return this->pos() + QPoint(this->boundingRect().right(), this->boundingRect().center().y());
}


/*
 *  ATTRIBUTE GETTERS AND SETTERS
 */

// returns this item's unique id
QString DiagramItem::id() const { return itemId; }

// returns the width of this item
int DiagramItem::width() const { return itemWidth; }

// sets the width for this item
// QGraphicsItem::update() may have to be called afterwards
void DiagramItem::setWidth(int value) { itemWidth = value; }

// returns the height of this item
int DiagramItem::height() const { return itemHeight; }

// sets the height for this item
// QGraphicsItem::update() may have to be called afterwards
void DiagramItem::setHeight(int value) { itemHeight = value; }
















