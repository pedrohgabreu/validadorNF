#include "XmlProcessor.h"
#include <QFile>
#include <QDomElement>
#include <QDebug>

XmlProcessor::XmlProcessor() {}

XmlProcessor::~XmlProcessor() {}

QDomDocument XmlProcessor::loadXml(const QString &xmlFilePath, QString &errorMsg) {
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        errorMsg = "Unable to open XML file: " + xmlFilePath;
        return QDomDocument();
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        errorMsg = "Failed to parse XML content.";
        return QDomDocument();
    }

    return doc;
}

QString XmlProcessor::extractValue(const QString &xmlFilePath, const QString &xpathExpression, QString &errorMsg) {
    QDomDocument doc = loadXml(xmlFilePath, errorMsg);
    if (doc.isNull()) {
        return QString();
    }

    QDomNodeList nodes = doc.elementsByTagName(xpathExpression);
    if (nodes.isEmpty()) {
        errorMsg = "No element found for XPath: " + xpathExpression;
        return QString();
    }

    return nodes.at(0).toElement().text();
}

QStringList XmlProcessor::listElements(const QString &xmlFilePath, QString &errorMsg) {
    QDomDocument doc = loadXml(xmlFilePath, errorMsg);
    if (doc.isNull()) {
        return QStringList();
    }

    QStringList elements;
    QDomElement root = doc.documentElement();
    QDomNodeList children = root.childNodes();

    for (int i = 0; i < children.count(); ++i) {
        QDomNode node = children.at(i);
        if (node.isElement()) {
            elements.append(node.toElement().tagName());
        }
    }

    return elements;
}
