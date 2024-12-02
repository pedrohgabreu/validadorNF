#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QList>
#include <QString>

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

    // Atualiza as informações gerais
    void updateGeralInfo(int numNFs, int numProdutos, double valorTotal, double impostosTotais);

    // Atualiza a tabela de Notas Fiscais
    void updateNotasFiscaisTable(const QList<QList<QString>> &dadosNF);

private slots:
    void handleUploadXML();

private:
    QLabel *labelNumNFs;
    QLabel *labelNumProdutos;
    QLabel *labelValorTotal;
    QLabel *labelImpostosTotais;
    QTableWidget *tableNotasFiscais;
    QPushButton *buttonUploadXML;

    // Função de validação de XML com XSD
    bool validateWithXSD(const QString &xmlFilePath, const QString &xsdFilePath);

    // Função de validação de XML com DTD
    bool validateWithDTD(const QString &xmlFilePath);

    // No arquivo dashboard.h, dentro da classe Dashboard:
    bool validateWithDTD(const QString &xmlFilePath, const QString &dtdFilePath);


};

#endif // DASHBOARD_H
