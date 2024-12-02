#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QString>
#include <libxml/xmlschemas.h>

class XmlValidator {
public:
    XmlValidator(const QString& schemaFile);
    ~XmlValidator();

    bool validate(const QString& xmlFile, QString& errorMsg);

private:
    xmlSchemaPtr schema;
    xmlSchemaValidCtxtPtr schemaCtx;
};

#endif // XMLVALIDATOR_H
