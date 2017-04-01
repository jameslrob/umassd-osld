#ifndef DESCRIPTIONFILEREADER_H
#define DESCRIPTIONFILEREADER_H

#include <QtCore>
#include <QFileDialog>
#include <QDebug>
#include "subdiagram.h"

class DescriptionFileReader: public QXmlStreamReader
{
private:
    TokenType currentToken;
    QString diagramName;
    QString diagramDescription;

    QMap <QString, CommonSource> sources;
    QMap <QString, StatusTypes> statuses;
    QMap <DiagramItemData,int> blockStatuses; // First QString should be id/"for_status" of block..LINE 744!!!

    QList<Subdiagram *> allSubdiagrams;  // a list of all of the subdiagrams
    QList<Block *> allBlocks;       // a list of all of the blocks in the diagram
    QList<Gate *> allGates;         // a list of all of the gates in the diagram
    QList<DiagramItem *> allItems;  // a list of both blocks and gates

    QMap<QString, int> getDimensions();
    QPointF getLocationPoint(QString tagName);
    QMap<QString, DiagramItemData> getStatusInfo();
    QMap<QString, QString> makeConnectMap();

    // holds error status for specific subelement functions
    Error currentError = NoError;

public:
    DescriptionFileReader(QString filePath = "", QWidget *parent = 0);
    ~DescriptionFileReader();

    //Methods
    Error readFile(QString filepath);

    QString getDiagramName() const;

    QString getDescription() const;

    QString cleanString(QString s);

    bool stringToBool (QString boolString);

    DiagramItem *findDiagramItemById(QString itemid);

    Error readMetaData();
    Error readBlocks();
    Error readGates();
    Error readSubdiagrams();

    QList<Subdiagram *> getAllSubdiagrams() const;

    QList<Block *> getAllBlocks() const;

    QList<Gate *> getAllGates() const;

    QList<DiagramItem *> getAllItems() const;
    QMap<QString, CommonSource> getSources() const;
    QMap<QString, StatusTypes> getStatuses() const;
};

#endif // DESCRIPTIONFILEREADER_H