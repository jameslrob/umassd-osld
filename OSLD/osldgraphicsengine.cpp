#include "osldgraphicsengine.h"

OSLDGraphicsEngine::OSLDGraphicsEngine(QWidget *parent)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    this->parent = parent;

    // get the status types for this diagram
    statuses["Valid"] = "#8BC34A";
    statuses["Invalid"] = "#EF5350";
    statuses["Unknown"] = "#888888";

    // get the sources for this diagram
    CommonSource sourceInfo;
    sourceInfo.name = "Combat System Database";
    sourceInfo.type = "SQLite Database";
    sources["source1"] = sourceInfo;

    for(int i = 0; i < 5; i++) {
        QPointF rootPoint(0, 0);

        // set the root for the subdiagram
        Block *block;
        if(i == 0) {
            block = getBlockInfoFromDescriptionFile(rootPoint);
        }
        else {
            DiagramItem *item;
            do {
                int random = qrand() % allSubdiagrams.at(i - 1)->getInputItems().count();
                item = allSubdiagrams.at(i - 1)->getInputItems().at(random);
            } while (item->isGate());
            block = qgraphicsitem_cast<Block *>(item);
        }
        allBlocks.append(block);
        allItems.append(block);

        // add items
        QPointF itemPoints;
        itemPoints.setX(rootPoint.x() - 200);
        itemPoints.setY(rootPoint.y());

        Gate *gate = getGateInfoFromDescriptionFile(itemPoints);
        allGates.append(gate);
        allItems.append(gate);

        itemPoints.setX(itemPoints.x() - 400);
        itemPoints.setY(itemPoints.y() - 100);
        Block *block1 = getBlockInfoFromDescriptionFile(itemPoints);
        allBlocks.append(block1);
        allItems.append(block1);

        itemPoints.setY(itemPoints.y() + 200);
        Block *block2 = getBlockInfoFromDescriptionFile(itemPoints);
        allBlocks.append(block2);
        allItems.append(block2);

        // set subdiagram nam and description
        QString name = QString("Subdiagram %1").arg(i);
        QString desc = QString("Subdiagram Description for %1").arg(i);

        Subdiagram *sub = new Subdiagram(block, name, desc);
        sub->addInputItem(gate);
        sub->connectItems(gate, block);

        sub->addInputItem(block1);
        sub->connectItems(block1, gate);

        sub->addInputItem(block2);
        sub->connectItems(block2, gate);

        allSubdiagrams.append(sub);
    }

    this->drawSubdiagramItems(allSubdiagrams.at(0));
}

Subdiagram *OSLDGraphicsEngine::getSubdiagramInfoFromDescriptionFile(Block *root, int index, int total) {
    Subdiagram *sub = new Subdiagram();

    // get name and description
    sub->setName(QString("Subdiagram %1").arg(index));
    sub->setDescription(QString("Subdiagram Description for %1").arg(index));

    // set the root block
    sub->setRoot(root);

    // get item ids, find the items, then add to the subdiagram

    // set all of the connectors

    return sub;
}


Gate *OSLDGraphicsEngine::getGateInfoFromDescriptionFile(QPointF pos) {

    int random = qrand() % 123456;

    QString id = "Gate ";
    id.append(QString::number(random));

    GateType type;

    if(random % 3 == 0) {
        type = AndGate;
    }
    else if((random % 3) - 1 == 0) {
        type = OrGate;
    }
    else {
        type = NotGate;
    }

    Gate *gate = new Gate(this->parent, id, pos, type);

    gate->setStatus("Valid", statuses);

    return gate;
}


// overrides to draw a grid for the background
void OSLDGraphicsEngine::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QBrush(QColor("#fafafa")));

    if(showGridBackground) {
        QPen pen;
        pen.setCosmetic(true);
        pen.setColor(QColor("#212121"));
        painter->setPen(pen);

        qreal topY = rect.top();
        qreal leftX = rect.left();
        qreal bottomY = rect.bottom();
        qreal rightX = rect.right();

        qreal startingX = int(leftX) - (int(leftX) % gridUnitSize);
        qreal startingY = int(topY) - (int(topY) % gridUnitSize);

        /*
        for(qreal x = startingX; x < rightX; x += gridUnitSize) {
            backgroundGrid.append(QLineF(x, topY, x, bottomY));
        }
        for(qreal y = startingY; y < bottomY; y += gridUnitSize) {
            backgroundGrid.append(QLineF(leftX, y, rightX, y));
        }
        painter->drawLines(backgroundGrid.data(), backgroundGrid.size());
        */

        for(qreal x = startingX; x < rightX; x += gridUnitSize) {
            for(qreal y = startingY; y < bottomY; y += gridUnitSize) {
                backgroundDots.append(QPointF(x, y));
            }
        }
        painter->drawPoints(backgroundDots.data(), backgroundDots.size());
    }
}

void OSLDGraphicsEngine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickedItem = itemAt(event->scenePos(), QTransform());
    clickPosition = event->scenePos();
    QGraphicsScene::mousePressEvent(event);
    update();
}

void OSLDGraphicsEngine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void OSLDGraphicsEngine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem *releaseItem = itemAt(event->scenePos(), QTransform());

    if(releaseItem == clickedItem && clickPosition == event->scenePos()) {
        DiagramItem *pressedItem = qgraphicsitem_cast<DiagramItem *>(releaseItem);
        if(pressedItem->isBlock()) {
            Block *pressedBlock = qgraphicsitem_cast<Block *>(pressedItem);
            if(pressedBlock->hasSubdiagram()) {
                this->hideSubdiagramItems(currentSubdiagram);
                this->drawSubdiagramItems(pressedBlock->getSubdiagram());
            }
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
    update();
}

void OSLDGraphicsEngine::drawSubdiagramItems(Subdiagram *sub)
{
    for(int i = 0; i < sub->getConnectors().count(); i++) {
        //qDebug() << "Drawing Connector" << i;
        this->addItem(sub->getConnectors().at(i));
    }
    for(int i = 0; i < sub->getInputItems().count(); i++) {
        //qDebug() << "Drawing Block" << i;
        this->addItem(sub->getInputItems().at(i));
    }
    this->addItem(sub->getRoot());
    currentSubdiagram = sub;
}

void OSLDGraphicsEngine::hideSubdiagramItems(Subdiagram *sub)
{
    for(int i = 0; i < sub->getConnectors().count(); i++) {
        //qDebug() << "Drawing Connector" << i;
        this->removeItem(sub->getConnectors().at(i));
    }
    for(int i = 0; i < sub->getInputItems().count(); i++) {
        //qDebug() << "Drawing Block" << i;
        this->removeItem(sub->getInputItems().at(i));
    }
    this->removeItem(sub->getRoot());
    currentSubdiagram = 0;
}

void OSLDGraphicsEngine::showGrid(bool show, QRectF area) {
    showGridBackground = show;
    this->invalidate(area, BackgroundLayer);
}


/*
 *  BLOCK FUNCTIONS
 */

// gets block information from a description file
// ******currently only generates random data
Block *OSLDGraphicsEngine::getBlockInfoFromDescriptionFile(QPointF pos)
{
    // create DescriptionFileReader object

    // obtain the id for this a block from
    int random = qrand() % 123456;

    QString id = "Block ";
    id.append(QString::number(random));

    QPointF position = pos;

    // create a BlockData structure to store the block data
    BlockData bd;
    bd.title = QString("Block %1 Title").arg(random);
    bd.description = QString("Block %1 Description").arg(random);
    bd.hovertext = QString("Block %1 Hovertext").arg(random);
    bd.status = (random % 2 == 0 ? "Valid" : "Invalid");

    if(random % 2 == 1) {
        bd.textColor = QColor(Qt::black);
    }
    else if(random % 3 == 0) {

    }

    // for testing large title strings
    // if(random % 3 == 0) bd.title.append("@@@@@ @@@@@@@@@ @@@@@@@ @@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@ @@@@@ @@@@ @@@@@@@@@@@@@@@@ @@@ @ @@@@@@@@@@@@ @@@@@@@@@ @@@ @@@@@@@@@@@@@@@@@@ @@@@ @");

    return buildBlock(id, position, bd);
}

// adds data to a block
Block *OSLDGraphicsEngine::buildBlock(QString id, QPointF position, BlockData data)
{
    // qDebug() << "Creating Block" << id;

    Block *block = new Block(this->parent, id, position);

    block->setTitle(data.title);
    block->setDescription(data.description);
    block->setToolTip(data.hovertext);
    block->setStatus(data.status, statuses);
    block->setTextColor(data.textColor);

    return block;
}

bool OSLDGraphicsEngine::blockExists(QString id) {
    for(int i = 0; i < allBlocks.count(); i++) {
        if(id.compare(allBlocks.at(i)->id())) {
            return true;
        }
    }
    return false;
}

Block *OSLDGraphicsEngine::retrieveBlock(QString id) {
    for(int i = 0; i < allBlocks.count(); i++) {
        if(id.compare(allBlocks.at(i)->id())) {
            return allBlocks.at(i);
        }
    }
    return 0;
}


QList<Subdiagram *> OSLDGraphicsEngine::getAllSubdiagrams() const
{
    return allSubdiagrams;
}

