#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QMap>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

    // Atualiza as informações gerais
    void updateGeralInfo(int numNFs, int numProdutos, double valorTotal, double impostosTotais);

    // Atualiza a tabela de Notas Fiscais
    void updateNotasFiscaisTable(const QList<QList<QString>> &dadosNF);

    // Exibe fornecedores
    void displayFornecedores(const QStringList &fornecedores, const QMap<QString, QList<QString>> &fornecedoresData);

    // Exibe transportadoras
    void displayTransportadoras(const QStringList &transportadoras, const QMap<QString, QList<QString>> &transportadorasData);

private slots:
    void handleUploadXML();

private:
    QLabel *labelNumNFs;
    QLabel *labelNumProdutos;
    QLabel *labelValorTotal;
    QLabel *labelImpostosTotais;
    QTableWidget *tableNotasFiscais;
    QTableWidget *tableFornecedores; // Tabela de fornecedores
    QTableWidget *tableTransportadoras; // Tabela de transportadoras
    QPushButton *buttonUploadXML;

    // Função de validação de XML com XSD usando libxml2
    bool validateWithXSD(const QString &xmlFilePath, const QString &xsdFilePath);

    // Função de validação de XML com DTD usando libxml2
    bool validateWithDTD(const QString &xmlFilePath, const QString &dtdFilePath);

};

#endif // DASHBOARD_H
