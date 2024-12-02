#ifndef XMLPROCESSOR_H
#define XMLPROCESSOR_H

#include <QString>
#include <QDomDocument>

class XmlProcessor {
public:
    XmlProcessor();
    ~XmlProcessor();

    QString extractValue(const QString &xmlFilePath, const QString &xpathExpression, QString &errorMsg);
    QStringList listElements(const QString &xmlFilePath, QString &errorMsg);

private:
    QDomDocument loadXml(const QString &xmlFilePath, QString &errorMsg);
};

#endif // XMLPROCESSOR_H
