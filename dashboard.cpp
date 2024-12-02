#include "dashboard.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QtXml>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QTabWidget *tabWidget = new QTabWidget(this);

    // Aba Geral
    QWidget *geralWidget = new QWidget();
    QVBoxLayout *geralLayout = new QVBoxLayout(geralWidget);

    labelNumNFs = new QLabel("Número total de NFs: 0");
    labelNumProdutos = new QLabel("Número total de produtos: 0");
    labelValorTotal = new QLabel("Valor total das NFs: 0.00");
    labelImpostosTotais = new QLabel("Valor total de impostos: 0.00");

    geralLayout->addWidget(labelNumNFs);
    geralLayout->addWidget(labelNumProdutos);
    geralLayout->addWidget(labelValorTotal);
    geralLayout->addWidget(labelImpostosTotais);

    tabWidget->addTab(geralWidget, "Geral");

    // Aba Notas Fiscais
    QWidget *nfWidget = new QWidget();
    QVBoxLayout *nfLayout = new QVBoxLayout(nfWidget);
    tableNotasFiscais = new QTableWidget(this);
    tableNotasFiscais->setColumnCount(4);
    tableNotasFiscais->setHorizontalHeaderLabels({"ID NF", "Data", "Valor", "Impostos"});
    tableNotasFiscais->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    nfLayout->addWidget(tableNotasFiscais);

    // Botão para upload do XML
    buttonUploadXML = new QPushButton("Upload XML", this);
    connect(buttonUploadXML, &QPushButton::clicked, this, &Dashboard::handleUploadXML);
    nfLayout->addWidget(buttonUploadXML);

    tabWidget->addTab(nfWidget, "Notas Fiscais");

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void Dashboard::updateGeralInfo(int numNFs, int numProdutos, double valorTotal, double impostosTotais) {
    labelNumNFs->setText(QString("Número total de NFs: %1").arg(numNFs));
    labelNumProdutos->setText(QString("Número total de produtos: %1").arg(numProdutos));
    labelValorTotal->setText(QString("Valor total das NFs: %1").arg(valorTotal, 0, 'f', 2));
    labelImpostosTotais->setText(QString("Valor total de impostos: %1").arg(impostosTotais, 0, 'f', 2));
}

void Dashboard::updateNotasFiscaisTable(const QList<QList<QString>> &dadosNF) {
    tableNotasFiscais->setRowCount(dadosNF.size());
    for (int i = 0; i < dadosNF.size(); ++i) {
        for (int j = 0; j < dadosNF[i].size(); ++j) {
            tableNotasFiscais->setItem(i, j, new QTableWidgetItem(dadosNF[i][j]));
        }
    }
}

bool Dashboard::validateWithDTD(const QString &xmlFilePath, const QString &dtdFilePath) {
    // Abrir e analisar o arquivo XML
    xmlDocPtr doc = xmlParseFile(xmlFilePath.toStdString().c_str());
    if (!doc) {
        qWarning() << "Erro ao carregar o arquivo XML.";
        return false;
    }

    // Usar BAD_CAST para a conversão correta para xmlChar*.
    xmlDtdPtr dtd = xmlParseDTD(NULL, reinterpret_cast<const xmlChar *>(dtdFilePath.toStdString().c_str()));
    if (!dtd) {
        xmlFreeDoc(doc);
        qWarning() << "Erro ao carregar o DTD.";
        return false;
    }

    // Criar contexto de validação
    xmlValidCtxtPtr validCtxt = xmlNewValidCtxt();
    if (!validCtxt) {
        xmlFreeDoc(doc);
        xmlFreeDtd(dtd);
        qWarning() << "Erro ao criar o contexto de validação DTD.";
        return false;
    }

    // Validar o documento XML contra o DTD
    bool isValid = xmlValidateDocument(validCtxt, doc);
    xmlFreeValidCtxt(validCtxt);
    xmlFreeDoc(doc);
    xmlFreeDtd(dtd);

    return isValid;
}

bool Dashboard::validateWithXSD(const QString &xmlFilePath, const QString &xsdFilePath) {
    xmlSchemaParserCtxtPtr parserCtxt = xmlSchemaNewParserCtxt(xsdFilePath.toStdString().c_str());
    if (!parserCtxt) {
        qWarning() << "Erro ao criar o contexto do schema.";
        return false;
    }

    xmlSchemaPtr schema = xmlSchemaParse(parserCtxt);
    xmlSchemaFreeParserCtxt(parserCtxt);
    if (!schema) {
        qWarning() << "Erro ao carregar o schema.";
        return false;
    }

    xmlSchemaValidCtxtPtr validCtxt = xmlSchemaNewValidCtxt(schema);
    if (!validCtxt) {
        xmlSchemaFree(schema);
        qWarning() << "Erro ao criar o contexto de validação.";
        return false;
    }

    int result = xmlSchemaValidateFile(validCtxt, xmlFilePath.toStdString().c_str(), 0);
    xmlSchemaFreeValidCtxt(validCtxt);
    xmlSchemaFree(schema);

    return (result == 0); // 0 indica validação bem-sucedida
}


void Dashboard::handleUploadXML() {
    // Carregar os arquivos XML
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Selecione arquivos XML", "", "Arquivos XML (*.xml)");
    if (fileNames.isEmpty()) {
        QMessageBox::information(this, "Nenhum Arquivo Selecionado", "Nenhum arquivo XML foi carregado.");
        return;
    }

    // Carregar o arquivo XSD (opcional)
    QString xsdFilePath = QFileDialog::getOpenFileName(this, "Selecione o arquivo XSD (opcional)", "", "Arquivos XSD (*.xsd)");
    bool useXSD = !xsdFilePath.isEmpty();

    // Carregar o arquivo DTD (opcional)
    QString dtdFilePath = QFileDialog::getOpenFileName(this, "Selecione o arquivo DTD (opcional)", "", "Arquivos DTD (*.dtd)");
    bool useDTD = !dtdFilePath.isEmpty();

    // Validar se foi escolhido XSD ou DTD
    if (!useXSD && !useDTD) {
        QMessageBox::warning(this, "Erro", "Você deve escolher ao menos um arquivo XSD ou DTD para validação.");
        return;
    }

    QList<QList<QString>> dadosNF;
    int totalNotas = 0;
    int totalProdutos = 0;
    double valorTotal = 0.0;
    double impostosTotais = 0.0;

    for (const QString &fileName : fileNames) {
        bool isValid = false;

        // Validar com XSD, se necessário
        if (useXSD) {
            isValid = validateWithXSD(fileName, xsdFilePath);
        }

        // Validar com DTD, se necessário
        if (useDTD) {
            isValid = validateWithDTD(fileName, dtdFilePath);
        }

        // Caso o XML não seja válido
        if (!isValid) {
            QMessageBox::warning(this, "Erro na Validação", "O arquivo " + fileName + " não é válido.");
            continue;
        }

        // Processar o XML se for válido
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Erro ao Abrir Arquivo", "Não foi possível abrir o arquivo: " + fileName);
            continue;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::warning(this, "Erro no XML", "O arquivo " + fileName + " não é um XML válido.");
            file.close();
            continue;
        }
        file.close();

        // Lendo os dados do XML
        QDomElement root = doc.documentElement();
        QString id = root.firstChildElement("ID").text();
        QString data = root.firstChildElement("Data").text();
        QString valor = root.firstChildElement("ValorTotal").text();
        QString impostos = root.firstChildElement("Impostos").text();

        int numProdutos = 0;
        QDomNodeList produtos = root.firstChildElement("Produtos").elementsByTagName("Produto");
        numProdutos = produtos.count();

        totalNotas++;
        totalProdutos += numProdutos;
        valorTotal += valor.toDouble();
        impostosTotais += impostos.toDouble();

        dadosNF.append({id, data, valor, impostos});
    }

    if (!dadosNF.isEmpty()) {
        updateNotasFiscaisTable(dadosNF);
        updateGeralInfo(totalNotas, totalProdutos, valorTotal, impostosTotais);
    }
}
