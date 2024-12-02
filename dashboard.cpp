#include "dashboard.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QFont>
#include <QColor>
#include <QDomDocument>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <QScreen>
#include <QInputDialog>

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent)
{
    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabWidget::pane {border: 1px solid #ddd; background-color: #f0f0f0;}");

    // Aba Geral
    QWidget *geralWidget = new QWidget();
    QVBoxLayout *geralLayout = new QVBoxLayout(geralWidget);

    // Labels para mostrar as informações gerais
    labelNumNFs = new QLabel("Número total de NFs: 0");
    labelNumProdutos = new QLabel("Número total de produtos: 0");
    labelValorTotal = new QLabel("Valor total das NFs: 0.00");
    labelImpostosTotais = new QLabel("Valor total de impostos: 0.00");

    // Estilizando as labels
    labelNumNFs->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    labelNumProdutos->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    labelValorTotal->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    labelImpostosTotais->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");

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
    tableNotasFiscais->setStyleSheet("QTableWidget {font-size: 14px; border: 1px solid #ddd; border-collapse: collapse;}"
                                     "QTableWidget::item {padding: 5px;}"
                                     "QHeaderView::section {background-color: #f2f2f2; border: 1px solid #ddd;}");

    nfLayout->addWidget(tableNotasFiscais);

    // Botão para upload de XML
    buttonUploadXML = new QPushButton("Upload XML", this);
    buttonUploadXML->setStyleSheet("QPushButton {background-color: #4CAF50; color: white; font-size: 16px; border: none; padding: 10px 20px; border-radius: 5px;} QPushButton:hover {background-color: #45a049;}");

    connect(buttonUploadXML, &QPushButton::clicked, this, &Dashboard::handleUploadXML);
    nfLayout->addWidget(buttonUploadXML);

    tabWidget->addTab(nfWidget, "Notas Fiscais");

    // Aba Fornecedores
    QWidget *fornecedoresWidget = new QWidget();
    QVBoxLayout *fornecedoresLayout = new QVBoxLayout(fornecedoresWidget);

    tableFornecedores = new QTableWidget(this);
    tableFornecedores->setColumnCount(2);
    tableFornecedores->setHorizontalHeaderLabels({"Fornecedor", "NFs"});
    tableFornecedores->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    fornecedoresLayout->addWidget(tableFornecedores);
    tabWidget->addTab(fornecedoresWidget, "Fornecedores");

    // Aba Transportadoras
    QWidget *transportadorasWidget = new QWidget();
    QVBoxLayout *transportadorasLayout = new QVBoxLayout(transportadorasWidget);

    tableTransportadoras = new QTableWidget(this);
    tableTransportadoras->setColumnCount(2);
    tableTransportadoras->setHorizontalHeaderLabels({"Transportadora", "NFs"});
    tableTransportadoras->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    transportadorasLayout->addWidget(tableTransportadoras);
    tabWidget->addTab(transportadorasWidget, "Transportadoras");

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    // Centralizando a janela e ajustando o tamanho com QScreen
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->availableSize();
    int width = 900;  // Largura desejada
    int height = 600; // Altura desejada
    int x = (screenSize.width() - width) / 2;
    int y = (screenSize.height() - height) / 2;

    setGeometry(x, y, width, height); // Define a posição e o tamanho
}

bool Dashboard::validateWithDTD(const QString &xmlFilePath, const QString &dtdFilePath) {
    // Converte os caminhos para xmlChar* para a função xmlParseDTD
    xmlDocPtr doc = xmlReadFile(xmlFilePath.toUtf8().constData(), nullptr, 0);
    if (doc == nullptr) {
        qWarning() << "Erro ao carregar o arquivo XML.";
        return false;
    }

    // Converte o caminho do DTD para xmlChar*
    xmlDtdPtr dtd = xmlParseDTD(nullptr, (const xmlChar*)dtdFilePath.toUtf8().constData());
    if (dtd == nullptr) {
        qWarning() << "Erro ao carregar o arquivo DTD.";
        xmlFreeDoc(doc);
        return false;
    }

    // Cria o validador
    xmlValidCtxtPtr ctxt = xmlNewValidCtxt();
    if (ctxt == nullptr) {
        qWarning() << "Erro ao criar o contexto de validação.";
        xmlFreeDoc(doc);
        xmlFreeDtd(dtd);
        return false;
    }

    // Realiza a validação
    int result = xmlValidateDocument(ctxt, doc);
    xmlFreeValidCtxt(ctxt);
    xmlFreeDoc(doc);
    xmlFreeDtd(dtd);

    return result == 1;
}

bool Dashboard::validateWithXSD(const QString &xmlFilePath, const QString &xsdFilePath) {
    // Carrega o arquivo XML
    xmlDocPtr doc = xmlReadFile(xmlFilePath.toUtf8().constData(), nullptr, 0);
    if (doc == nullptr) {
        qWarning() << "Erro ao carregar o arquivo XML.";
        return false;
    }

    // Carrega o arquivo XSD
    xmlSchemaParserCtxtPtr schemaParserCtxt = xmlSchemaNewParserCtxt(xsdFilePath.toUtf8().constData());
    if (schemaParserCtxt == nullptr) {
        qWarning() << "Erro ao carregar o arquivo XSD.";
        xmlFreeDoc(doc);
        return false;
    }

    // Cria o esquema a partir do XSD
    xmlSchemaPtr schema = xmlSchemaParse(schemaParserCtxt);
    xmlSchemaFreeParserCtxt(schemaParserCtxt);
    if (schema == nullptr) {
        qWarning() << "Erro ao parsear o arquivo XSD.";
        xmlFreeDoc(doc);
        return false;
    }

    // Cria o validador
    xmlSchemaValidCtxtPtr validCtxt = xmlSchemaNewValidCtxt(schema);
    if (validCtxt == nullptr) {
        qWarning() << "Erro ao criar o contexto de validação do XSD.";
        xmlFreeDoc(doc);
        xmlSchemaFree(schema);
        return false;
    }

    // Realiza a validação
    int result = xmlSchemaValidateDoc(validCtxt, doc);
    xmlSchemaFreeValidCtxt(validCtxt);
    xmlFreeDoc(doc);
    xmlSchemaFree(schema);

    return result == 0; // Retorna true se a validação for bem-sucedida
}



void Dashboard::handleUploadXML() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Selecione arquivos XML", "", "Arquivos XML (*.xml)");
    if (fileNames.isEmpty()) {
        QMessageBox::information(this, "Nenhum Arquivo Selecionado", "Nenhum arquivo XML foi carregado.");
        return;
    }

    QString validationType = QInputDialog::getItem(this, "Tipo de Validação", "Escolha o tipo de validação:", {"DTD", "XSD"}, 0, false);
    if (validationType.isEmpty()) {
        QMessageBox::information(this, "Nenhuma Validação Selecionada", "Nenhum tipo de validação foi escolhido.");
        return;
    }

    QString schemaFilePath;
    if (validationType == "DTD") {
        schemaFilePath = QFileDialog::getOpenFileName(this, "Selecione o arquivo DTD", "", "Arquivos DTD (*.dtd)");
    } else {
        schemaFilePath = QFileDialog::getOpenFileName(this, "Selecione o arquivo XSD", "", "Arquivos XSD (*.xsd)");
    }

    if (schemaFilePath.isEmpty()) {
        QMessageBox::information(this, "Nenhum Arquivo Selecionado", "Nenhum arquivo de esquema foi carregado.");
        return;
    }

    QList<QList<QString>> dadosNF;
    int totalNotas = 0;
    int totalProdutos = 0;
    double valorTotal = 0.0;
    double impostosTotais = 0.0;

    for (const QString &fileName : fileNames) {
        bool isValid = false;
        if (validationType == "DTD") {
            isValid = validateWithDTD(fileName, schemaFilePath);
        } else {
            isValid = validateWithXSD(fileName, schemaFilePath);
        }

        if (!isValid) {
            QMessageBox::warning(this, "Erro na Validação", "O arquivo " + fileName + " não é válido.");
            continue;
        }

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Erro ao Abrir Arquivo", "Não foi possível abrir o arquivo: " + fileName);
            continue;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            QMessageBox::warning(this, "Erro ao Processar Arquivo", "Erro ao processar o conteúdo do arquivo: " + fileName);
            continue;
        }
        file.close();

        QDomElement root = doc.documentElement();
        QString id = root.firstChildElement("ID").text();
        QString data = root.firstChildElement("Data").text();
        QString valor = root.firstChildElement("ValorTotal").text();
        QString impostos = root.firstChildElement("Impostos").text();

        int numProdutos = root.firstChildElement("Produtos").elementsByTagName("Produto").count();

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


// Função para atualizar as informações gerais no Dashboard
void Dashboard::updateGeralInfo(int numNFs, int numProdutos, double valorTotal, double impostosTotais) {
    labelNumNFs->setText(QString("Número total de NFs: %1").arg(numNFs));
    labelNumProdutos->setText(QString("Número total de produtos: %1").arg(numProdutos));
    labelValorTotal->setText(QString("Valor total das NFs: %1").arg(valorTotal, 0, 'f', 2));
    labelImpostosTotais->setText(QString("Valor total de impostos: %1").arg(impostosTotais, 0, 'f', 2));
}

// Função para atualizar a tabela de Notas Fiscais
void Dashboard::updateNotasFiscaisTable(const QList<QList<QString>> &dadosNF) {
    tableNotasFiscais->setRowCount(dadosNF.size());
    for (int i = 0; i < dadosNF.size(); ++i) {
        for (int j = 0; j < dadosNF[i].size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(dadosNF[i][j]);
            item->setTextAlignment(Qt::AlignCenter); // Centralizando o texto
            tableNotasFiscais->setItem(i, j, item);
        }
    }
}

// Função para exibir os Fornecedores
void Dashboard::displayFornecedores(const QStringList &fornecedores, const QMap<QString, QList<QString>> &fornecedoresData) {
    tableFornecedores->setRowCount(fornecedores.size());
    int row = 0;
    for (const QString &fornecedor : fornecedores) {
        QTableWidgetItem *itemFornecedor = new QTableWidgetItem(fornecedor);
        tableFornecedores->setItem(row, 0, itemFornecedor);

        QStringList nfIds = fornecedoresData[fornecedor];
        QString nfList = nfIds.join(", ");
        QTableWidgetItem *itemNFs = new QTableWidgetItem(nfList);
        tableFornecedores->setItem(row, 1, itemNFs);

        row++;
    }
}

// Função para exibir as Transportadoras
void Dashboard::displayTransportadoras(const QStringList &transportadoras, const QMap<QString, QList<QString>> &transportadorasData) {
    tableTransportadoras->setRowCount(transportadoras.size());
    int row = 0;
    for (const QString &transportadora : transportadoras) {
        QTableWidgetItem *itemTransportadora = new QTableWidgetItem(transportadora);
        tableTransportadoras->setItem(row, 0, itemTransportadora);

        QStringList nfIds = transportadorasData[transportadora];
        QString nfList = nfIds.join(", ");
        QTableWidgetItem *itemNFs = new QTableWidgetItem(nfList);
        tableTransportadoras->setItem(row, 1, itemNFs);

        row++;
    }
}
