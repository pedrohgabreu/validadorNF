#include "XmlValidator.h"
#include <libxml/xmlschemas.h>
#include <QString>

XmlValidator::XmlValidator(const QString& schemaFile)
    : schema(nullptr), schemaCtx(nullptr) {

    // Converte o arquivo XSD para xmlChar* (tipo aceito pela libxml2)
    const xmlChar* schemaXmlChar = reinterpret_cast<const xmlChar*>(schemaFile.toUtf8().constData());

    // Modifique o cast para const char* para resolver o problema de conversão
    xmlSchemaParserCtxtPtr parserCtx = xmlSchemaNewParserCtxt(reinterpret_cast<const char*>(schemaXmlChar));

    // Analisa o esquema
    schema = xmlSchemaParse(parserCtx);

    if (schema == nullptr) {
        // Erro de parsing do esquema
        // Trate o erro aqui
    }

    schemaCtx = xmlSchemaNewValidCtxt(schema);
}

XmlValidator::~XmlValidator() {
    // Libera recursos alocados
    if (schema) {
        xmlSchemaFree(schema);
    }
    if (schemaCtx) {
        xmlSchemaFreeValidCtxt(schemaCtx);
    }
}

bool XmlValidator::validate(const QString& xmlFile, QString& errorMsg) {
    // Conversão de QString para xmlChar*
    const xmlChar* xmlCharFile = reinterpret_cast<const xmlChar*>(xmlFile.toUtf8().constData());

    // Criação do contexto de validação
    xmlDocPtr doc = xmlParseFile(reinterpret_cast<const char*>(xmlCharFile));

    if (doc == nullptr) {
        errorMsg = "Erro ao carregar o arquivo XML.";
        return false;
    }

    // Validação do XML
    int result = xmlSchemaValidateDoc(schemaCtx, doc);
    xmlFreeDoc(doc);  // Libere o documento XML depois de validá-lo

    if (result != 0) {
        errorMsg = "Falha na validação do XML.";
        return false;
    }

    return true;
}
