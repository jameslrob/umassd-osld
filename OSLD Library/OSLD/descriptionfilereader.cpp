#include "descriptionfilereader.h"

/*
 * CONSTRUCTORS
 */

DescriptionFileReader::DescriptionFileReader(QString filePath)
{
    // If a path was provided
    if(!filePath.isEmpty()) {
        // read the file with the path
        this->readFile(filePath);
    }
}

DescriptionFileReader::~DescriptionFileReader()
{

}


/*
 * INIT FUNCTION
 */

QXmlStreamReader::Error DescriptionFileReader::readFile(QString filepath)
{
    //Parser handler;
    //bool hope; //determines if file is open and able to be parsed
    //this->setContentHandler(&handler);
    //this->setErrorHandler(&handler);

    // qDebug() << ">> QXmlStreamReader";
    // qDebug() << "Reading file:" << filepath;
    QFile xmlFile(filepath);

    if(xmlFile.open(QIODevice::ReadOnly)) {
        // qDebug() << "File Opened";

        // QXmlInputSource source (&xmlFile);
        // QXmlStreamReader xmlReader(xmlFile);

        // std::string inBuff;

        // qDebug() << "Setting device to XML file";
        this->setDevice(&xmlFile);

        // qDebug() << "Begin reading";
        while(!this->atEnd()) {

            // get the type of xml element currently being read
            this->readNext();


            /*
            // qDebug string to print current token type
            // prints every line read in the xml file
            QString tString = (this->tokenString().replace("Characters", "String") +
                               (this->tokenString().contains("Element") ? " " : "") + this->name().toString());
            // qDebug() << ">> Found Token (not in function):" << tString;
            */

            // start of element
            if(this->isStartElement()) {

                // get the name of the current tag
                QString currentTag = this->name().toString();
                // qDebug() << "Reading in element:" << currentTag;

                // diagram tag
                if(currentTag == "diagram") {           // diagram tag
                    // qDebug() << "Setting diagram name";
                    diagramName = this->attributes().value("name").toString();  // get the value of the name attribute

                    // qDebug() << "Diagram Name:" << diagramName;
                }
                else if(currentTag == "meta") {         // meta tag
                    // qDebug() << "calling readMetaData function"<<endl;
                    this->currentError = this->readMetaData();
                }
                else if(currentTag == "blocks") {       // blocks tag
                    // qDebug() << "do blocks stuff";
                    this->currentError = this->readBlocks();
                }
                else if(currentTag == "gates") {        // gates tag
                    // qDebug() << "do gates stuff";
                    this->currentError = this->readGates();
                }
                else if(currentTag == "subdiagrams") {  // subdiagrams tag
                    // qDebug() << "do subdiagram stuff";
                    this->currentError = this->readSubdiagrams();
                }
            }
        }
        // qDebug() << "End reading";
    }
    else {
        this->raiseError("File not found '" + filepath + "'");
    }

    if(this->hasError()) {
        qDebug() << this->errorString() << "at line" << this->lineNumber();
        this->currentError = this->error();
    }

    return this->error();
}


/*
 * UTILITIES
 */

// removes newlines and tabs from a string
QString DescriptionFileReader::cleanString(QString s) {
    return s.replace(QRegExp("\t|\n|\r"),"");
}

// converts a string to a bool
bool DescriptionFileReader::stringToBool(QString boolString)
{
    if(boolString == "true")
        return true;
    else
        return false;
}

/*
 * MAIN SECTION FUNCTIONS
 */

// read meta section of description file
QXmlStreamReader::Error DescriptionFileReader::readMetaData()
{
    this->readNext();
    QString currentTag = this->name().toString();
    QXmlStreamAttributes attributes;
    QString tString;

    // qDebug() << "==================================START META==================================";
    while(!this->atEnd() && !(currentTag == "meta" && this->isEndElement())) {

        tString = (this->tokenString().replace("Characters", "String") +
                           (this->tokenString().contains("Element") ? " " : "") + this->name().toString());
        // qDebug() << ">> Found Token in meta:" << tString;

        // description
        if(currentTag == "description" && this->isStartElement()) {
            this->diagramDescription = cleanString(this->readElementText());
        }
        // common_source
        else if(currentTag == "common_source" && this->isStartElement()) {

            // keep reading elements until the end of the common_source tag
            while(!this->atEnd() && !(currentTag == "common_source" && this->isEndElement())) {
                // for a given source
                if(currentTag == "source" && this->isStartElement()) {

                    CommonSource source;
                    QString sourceid;

                    // get the source's id
                    attributes = this->attributes();
                    if(attributes.hasAttribute("id")) {
                        sourceid = attributes.value("id").toString();
                    }

                    // keep reading elements until the end of this source tag
                    while(!this->atEnd() && !(currentTag == "source" && this->isEndElement())) {
                        // get the source name
                        if(currentTag == "name" && this->isStartElement()) {
                            source.name = this->readElementText();
                        }
                        // get the source type
                        if(currentTag == "type" && this->isStartElement()) {
                            source.type = this->readElementText();
                        }

                        // update token and tag to read next element in common_source
                        this->readNext();
                        currentTag = this->name().toString();
                    }
                    if(this->hasError()) {
                        return this->error();
                    }

                    // add this source to the sources QMap
                    sources[sourceid] = source;
                }

                // update token and tag to read next element in common_source
                this->readNext();
                currentTag = this->name().toString();
            }
        }
        // status_types
        else if(currentTag == "status_types" && this->isStartElement()) {

            // keep reading elements until the end of the status_types tag
            while(!this->atEnd() && !(currentTag == "status_types" && this->isEndElement())) {
                // for a given status
                if(currentTag == "status" && this->isStartElement()) {

                    StatusTypes status;
                    QString statusid;

                    // get the status's id
                    attributes = this->attributes();
                    if(attributes.hasAttribute("id")) {
                        statusid = attributes.value("id").toString();
                    }

                    // keep reading elements until the end of this status tag
                    while(!(currentTag == "status" && this->isEndElement())) {
                        // get the status color
                        if(currentTag == "color" && this->isStartElement()) {
                            status.color = this->readElementText();
                        }
                        // get the status text color
                        else if(currentTag == "textcolor" && this->isStartElement()) {
                            status.textColor = this->readElementText();
                        }
                        // get the status text color
                        else if(currentTag == "italics" && this->isStartElement()) {
                            status.italics = this->stringToBool(this->readElementText());
                        }
                        // get the status text color
                        else if(currentTag == "bold" && this->isStartElement()) {
                            status.bold = this->stringToBool(this->readElementText());
                        }
                        // get the status text color
                        else if(currentTag == "underline" && this->isStartElement()) {
                            status.underline = this->stringToBool(this->readElementText());
                        }

                        // update token and tag to read next element in common_source
                        this->readNext();
                        currentTag = this->name().toString();
                    }

                    // add this source to the sources QMap
                    statuses[statusid] = status;
                }

                // update token and tag to read next element in common_source
                this->readNext();
                currentTag = this->name().toString();
            }
        }

        // update token and tag to read next element
        this->readNext();
        currentTag = this->name().toString();
    }

    // qDebug() << "Sources:" << sources.count() << sources.keys();
    // qDebug() << "Statuses:" << statuses.count() << statuses.keys();

    // qDebug() << "===================================END META===================================";

    return this->error();
}

// read blocks section of description file
QXmlStreamReader::Error DescriptionFileReader::readBlocks()
{
    this->readNext();
    QString currentTag = this->name().toString();
    QXmlStreamAttributes attributes;
    Block *block = 0; // block pointer

    // qDebug() << "==================================START BLOCKS==================================";
    while(!this->atEnd() && !(currentTag == "blocks" && this->isEndElement())) {

        // get block attributes
        if(currentTag == "block" && this->isStartElement()) {
            // qDebug() << "------ Start reading block ------";
            // get the block attributes
            attributes = this->attributes();

            // create a block object with the id
            if(attributes.hasAttribute("id")) {
                block = new Block(attributes.value("id").toString()); // create a new block
                // qDebug() << "Found block id" << block->id();

            }
            // get and save the source id to the block
            if(attributes.hasAttribute("source")) {
                block->setSourceId(attributes.value("source").toString());
                // qDebug() << "Found block source" << block->getSourceId();

                // CONTACT STIMULATOR HERE -> save and use ref_id if available
            }
        }
        // get dimensions data
        else if(currentTag == "dimensions" && this->isStartElement()) {
            // call function to save the dimensions as a QMap
            QMap<QString, int> dimension = this->getDimensions();

            // set the block's width and height
            block->setItemSizing(dimension["width"],dimension["height"]);

            // print to make sure they were save properly
            // qDebug() << "Dimensions Set!" << block->width() << block->height();
        }
        // get location data
        else if(currentTag == "location" && this->isStartElement()) {
            QPointF location = this->getLocationPoint("location");    // call get location point function
            block->setLocation(location);                    // set the block's location
            // qDebug() << "Location Set!" << block->pos();     // print
        }
        // get status info
        else if(currentTag == "status_info" && this->isStartElement()) {
            // get the default status value
            QString defaultStatus;
            if(this->attributes().hasAttribute("default_status")) {
                defaultStatus = this->attributes().value("default_status").toString();
            }
            block->setStatusInfoDataList(this->getStatusInfo());
            block->setStatus(defaultStatus, statuses);
            // this->getStatusInfo(block);
        }
        // at end of block element
        else if(currentTag == "block" && this->isEndElement()) {
            allBlocks.append(block);  // add block pointer to allGates list in header file
            allItems[block->id()] = block;
            // qDebug() << "------ Stored Block! ------";
        }

        // update token and tag to read next element
        this->readNext();
        currentTag = this->name().toString();
    }

    // print amount of items in allBlocks list
    // qDebug() << "Blocks stored:" << allBlocks.count();

    // qDebug() << "===================================END BLOCKS===================================";


    return this->error();
}

// read gates section of description file
QXmlStreamReader::Error DescriptionFileReader::readGates()
{

    // update token and tag
    this->readNext();
    QString currentTag = this->name().toString();
    QXmlStreamAttributes attributes;
    Gate *gate = 0; // gate pointer

    // qDebug() << "==================================START GATES==================================";
    while(!this->atEnd() && !(currentTag == "gates" && this->isEndElement())) {

        // get gate attributes
        if(currentTag == "gate" && this->isStartElement()) {
            // qDebug() << "------ Start reading gate ------";
            // get the gate attributes
            attributes = this->attributes();

            // create a gate object with the id
            if(attributes.hasAttribute("id")) {
                gate = new Gate(attributes.value("id").toString()); // create a new gate
                // qDebug() << "Found gate id" << gate->id();

            }
            // get and save the gate type to the gate
            if(attributes.hasAttribute("type")) {
                gate->setGateType(attributes.value("type").toString());
                // qDebug() << "Found gate type" << attributes.value("type").toString();
            }
            // get and save the source id to the gate
            if(attributes.hasAttribute("source")) {
                gate->setSourceId(attributes.value("source").toString());
                // qDebug() << "Found gate source" << gate->getSourceId();

                // CONTACT STIMULATOR HERE -> save and use ref_id if available

                // gate->setXXX
            }
        }
        // get dimensions data
        else if(currentTag == "dimensions" && this->isStartElement()) {
            // call function to save the dimensions as a QMap
            QMap<QString, int> dimension = this->getDimensions();

            // set the gate's width and height
            gate->setGateSizing(dimension["width"],dimension["height"]);

            // print to make sure they were save properly
            // qDebug() << "Dimensions Set!" << gate->width() << gate->height();
        }
        // get location data
        else if(currentTag == "location" && this->isStartElement()) {
            QPointF location = this->getLocationPoint("location");    // call get location point function
            gate->setLocation(location);                    // set the gate's location
            // qDebug() << "Location Set!" << gate->pos();     // print
        }
        // get status info
        else if(currentTag == "status_info" && this->isStartElement()) {
            QString defaultStatus;
            if(this->attributes().hasAttribute("default_status")) {
                defaultStatus = this->attributes().value("default_status").toString();
            }
            gate->setStatusInfoDataList(this->getStatusInfo());
            gate->setStatus(defaultStatus, statuses);
        }
        // at end of gate element
        else if(currentTag == "gate" && this->isEndElement()) {
            allGates.append(gate);  // add gate pointer to allGates list in header file
            allItems[gate->id()] = gate;
            // qDebug() << "------ Stored gate! ------";
        }

        // update token and tag to read next element
        this->readNext();
        currentTag = this->name().toString();
    }

    // print amount of items in allGates list
    // qDebug() << "Gates stored:" << allGates.count();

    // qDebug() << "===================================END GATES===================================";


    return this->error();
}

// read subdiagrams section of description file
QXmlStreamReader::Error DescriptionFileReader::readSubdiagrams()
{
    // loop through each line in the subdiagrams element
        // **be careful not to overuse "this->readNext();"

    // update token and tag
    this->readNext();
    QString currentTag = this->name().toString();
    Subdiagram *subdiagram = 0;
    Block *root = 0;

    // qDebug() << "==================================START SUBDIAGRAMS==================================";

    // go through all elements in subdiagrams tag
    while(!this->atEnd() && !(currentTag == "subdiagrams" && this->isEndElement())) {

        // Subdiagram(Block *root, QString name, QString description);

        // create a subdiagram object pointer
        bool isMainDiagram = false;

        // get subdiagram attributes
        if(currentTag == "subdiagram" && this->isStartElement()) {
            // qDebug() << "------ Start reading subdiagram ------";

            // qDebug() << "Reading attributes";

            if(attributes().hasAttribute("root_item_id")) {
                // qDebug() << "Reading root id";
                QString rootId(attributes().value("root_item_id").toString());
                for(int i = 0; i < allBlocks.count(); i++) {
                    if(allBlocks.at(i)->id().compare(rootId) == 0) {
                        // qDebug() << allBlocks.at(i)->id() << "equals" << rootId;
                        root = allBlocks.at(i);
                    }
                }
            }
            if(attributes().hasAttribute("main_diagram")) {
                // qDebug() << "Reading main diagram";
                isMainDiagram = stringToBool(attributes().value("main_diagram").toString());
            }

            // create a new subdiagram
            subdiagram = new Subdiagram(root);
        }
        // get subdiagram data
        else if(currentTag == "subdiagram_data" && this->isStartElement()) {
            // qDebug() << "Reading subdiagram data";
            while(!this->atEnd() && !(currentTag == "subdiagram_data" && this->isEndElement())) {

                if(currentTag == "name" && this->isStartElement()) {
                    QString name = this->readElementText();
                    subdiagram->setName((name.isEmpty() ? "No Subdiagram Name" : name));
                    // qDebug() << "Subdiagram name" << subdiagram->getName();
                }
                else if(currentTag == "description" && this->isStartElement()) {
                    QString description = this->readElementText();
                    subdiagram->setDescription((description.isEmpty() ? "No Subdiagram Description Available" : description));
                    // qDebug() << "Subdiagram description" << subdiagram->getDescription();
                }
                // get root item location data
                else if(currentTag == "root_item_location" && this->isStartElement()) {
                    QPointF location = this->getLocationPoint("root_item_location");    // call get location point function
                    root->setRootLocation(location);                    // set the root's location
                    // qDebug() << "Root item Location Set!" << subdiagram->getRoot()->getRootLocation();     // print
                }

                // update token and tag to read next element
                this->readNext();
                currentTag = this->name().toString();
            }
        }
        else if(currentTag == "connections" && this->isStartElement()) {
            // qDebug() << "-- Start connections";
            while(!this->atEnd() && !(currentTag == "connections" && this->isEndElement())) {

                // process a connection
                if(currentTag == "connect" && this->isStartElement()) {
                    // get the ids for the input and output
                    QMap<QString, QString> inputOutput = this->makeConnectMap();

                    // get the diagramitems from the diagram item list
                    DiagramItem *input = allItems[inputOutput["input"]];
                    DiagramItem *output = allItems[inputOutput["output"]];

                    // add output and input as input items for the subdiagram
                    subdiagram->addInputItem(input);
                    subdiagram->addInputItem(output);

                    // connect the two items
                    subdiagram->connectItems(input, output);
                    // qDebug() << "connected items" << inputOutput["input"] << inputOutput["output"];
                }

                // update token and tag to read next element
                this->readNext();
                currentTag = this->name().toString();
            }
            // qDebug() << "-- End connections";
        }
        else if(currentTag == "subdiagram" && this->isEndElement()) {

            // add diagram to front if it is the main diagram
            if(isMainDiagram)
                allSubdiagrams.prepend(subdiagram);
            else
                allSubdiagrams.append(subdiagram);

            // qDebug() << "------ finished subdiagram! ------";
        }

        // update token and tag to read next element
        this->readNext();
        currentTag = this->name().toString();
    }
    // print amount of items in allSubdiagrams list
    // qDebug() << "Subdiagrams stored:" << allSubdiagrams.count();

    // qDebug() << "===================================END SUBDIAGRAMS===================================";

    return this->error();
}



/*
 * INDIVIDUAL TAG FUNCTIONS
 */

// read connect tag in a subdiagram's connections tag
// return a QMap with input and output item id
QMap<QString, QString> DescriptionFileReader::makeConnectMap()
{
    QMap<QString, QString> inputOutput;

    this->readNext();
    QString currentTag = this->name().toString();

    // while this is not an end connect tag
    while(!this->atEnd() && !(currentTag == "connect" && this->isEndElement())) {

        if(currentTag == "input" && this->isStartElement()) {
            inputOutput["input"] = this->readElementText();
        }
        else if(currentTag == "output" && this->isStartElement()) {
            inputOutput["output"] = this->readElementText();
        }

        // update token and tag to read next element
        this->readNext();
        currentTag = this->name().toString();
    }
    if(this->hasError()) {
        currentError = this->error();
    }

    return inputOutput;
}

// read status_info of a block or gate
QMap<QString, DiagramItemData> DescriptionFileReader::getStatusInfo()
{
    // qDebug() << "--\nStart reading status_info tag";

    // go to next element
    this->readNext();
    QString currentTag = this->name().toString();

    // to store item info to a list compatible to what's in diagramitem objects
    QMap<QString, DiagramItemData> itemDataMap;

    // read until end of status_info element
    while(!this->atEnd() && !(currentTag == "status_info" && this->isEndElement())) {
        // for each data tag
        if(currentTag == "data" && this->isStartElement()) {
            // qDebug() << "Found data tag";

            QString currentForStatusID; // holds the key for the QMap
            DiagramItemData data;       // holds the value for the QMap

            // get the for_status attribute
            if(this->attributes().hasAttribute("for_status")) {
                // qDebug() << "getting for_status" << this->attributes().value("for_status").toString();
                currentForStatusID = this->attributes().value("for_status").toString();
            }

            // get each data element
            while(!this->atEnd() && !(currentTag == "data" && this->isEndElement())) {
                // get the name/title
                if(currentTag == "name" && this->isStartElement()) {

                    // if the query attribute exists
                    if(this->attributes().hasAttribute("query")) {
                        // qDebug() << "setting query value name";
                        // save the query
                        data.titleQuery = this->attributes().value("query").toString();
                    }

                    data.title = this->readElementText();   // set the title

                }
                // get the description
                if(currentTag == "description" && this->isStartElement()) {

                    // if the query attribute exists
                    if(this->attributes().hasAttribute("query")) {
                        // qDebug() << "setting query value desc";
                        // save the query
                        data.descriptionQuery = this->attributes().value("query").toString();
                    }

                    data.description = this->readElementText();   // set the title

                }
                // get the hovertext
                if(currentTag == "hovertext" && this->isStartElement()) {

                    // if the query attribute exists
                    if(this->attributes().hasAttribute("query")) {
                        // qDebug() << "setting query value hover";
                        // save the query
                        data.hovertextQuery = this->attributes().value("query").toString();
                    }

                    data.hovertext = this->readElementText();   // set the title

                }

                // update token and tag
                this->readNext();
                currentTag = this->name().toString();
            }

            /* print data in current diagramitem data object
            // qDebug() << "Data for" << currentForStatusID
                     << data.title << data.description
                     << data.hovertext
                     << data.titleQuery << data.descriptionQuery
                     << data.hovertextQuery;
            */

            // add the data to the item map
            itemDataMap[currentForStatusID] = data;
        }

        // update token and tag
        this->readNext();
        currentTag = this->name().toString();
    }
    if(this->hasError()) {
        currentError = this->error();   // save the error if there was one
    }

    // qDebug() << "Number of data items:" << itemDataMap.count() << itemDataMap.keys();

    // qDebug() << "End reading status_info tag\n--";

    return itemDataMap;
}


// function to read a location tag with a X and Y subtag
QPointF DescriptionFileReader::getLocationPoint(QString tagName) {

    // qDebug() << "--\nStart reading location tag";

    // goes to next xml item
    this->readNext();
    QString currentTag = this->name().toString();

    // store the x and y values
    int x = 0, y = 0;

    // loop until end of location tag
    while(!this->atEnd() && !(currentTag == tagName && this->isEndElement())) {
        // qDebug() << "Looping through location";

        // get width text and save as int
        if(currentTag == "X") {
            x = this->readElementText().toInt();
            // qDebug() << "found x value" << x;
        }
        // get height text and save as int
        else if(currentTag == "Y") {
            y = this->readElementText().toInt();
            // qDebug() << "found y value" << y;
        }

        // goes to next xml item with each loop
        this->readNext();
        currentTag = this->name().toString();
    }
    if(this->hasError()) {
        currentError = this->error();
    }

    // qDebug() << "End reading location tag\n--";

    return QPointF(x,y);
}

// function to read a dimensions tag with a width and height subtag
QMap<QString, CommonSource> DescriptionFileReader::getSources() const
{
    return sources;
}

QMap<QString, StatusTypes> DescriptionFileReader::getStatuses() const
{
    return statuses;
}

QMap<QString, int> DescriptionFileReader::getDimensions() {

    // qDebug() << "--\nStart reading dimensions tag";

    // to store the dimensions
    QMap<QString, int> dimension;

    // goes to next xml item
    this->readNext();
    QString currentTag = this->name().toString();

    // loop until end of dimensions tag
    while(!this->atEnd() && !(currentTag == "dimensions" && this->isEndElement())) {
        // qDebug() << "Looping through dimensions";

        // get width text and save as int
        if(currentTag == "width") {
            dimension["width"] = this->readElementText().toInt();
            // qDebug() << "found width" << dimension["width"];
        }
        // get height text and save as int
        else if(currentTag == "height") {
            dimension["height"] = this->readElementText().toInt();
            // qDebug() << "found height" << dimension["height"];
        }

        // goes to next xml token with each loop
        this->readNext();
        currentTag = this->name().toString();   // save the token's name
    }
    if(this->hasError()) {
        currentError = this->error();
    }

    // qDebug() << "End reading dimensions tag\n--";

    return dimension;
}


/*
 * GETTERS
 */

QString DescriptionFileReader::getDiagramName() const
{
    return diagramName;
}

QString DescriptionFileReader::getDescription() const
{
    return diagramDescription;
}


QList<Subdiagram *> DescriptionFileReader::getAllSubdiagrams() const
{
    return allSubdiagrams;
}

QList<Block *> DescriptionFileReader::getAllBlocks() const
{
    return allBlocks;
}

QList<Gate *> DescriptionFileReader::getAllGates() const
{
    return allGates;
}

QMap<QString, DiagramItem *> DescriptionFileReader::getAllItems() const
{
    return allItems;
}


// for each gate - NEED TO CHECK STIMULATOR AS WELL

    // create a gate object pointer

    // get id and source attributes and save to gate object

    // loop through each line in gate

    // get dimensions - handle situation when width and height are empty

    // get location, save as QPointF object, set gate location

    // *check stimulator for gate status*
        // otherwise get status_info default_status attribute

    // loop through each line in status_info

        // get data element with correct for_status attribute

        // for each of name, description, hovertext

            // save column attribute to array if present

            // check for hard-coded text
                // if present, save text to gate object
                // otherwise, go to stimulator

        // end loop when currentToken is EndElement and element name is "status_info"

    // end loop when currentToken is EndElement and element name is "gate"

    // append gate object pointer to a QList

// end loop when currentToken is EndElement and element name is "gates"
