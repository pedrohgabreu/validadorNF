#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include "XmlValidator.h"
#include "dashboard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dashboard(nullptr)
{
    ui->setupUi(this);

    // Conecta o botão para abrir o QFileDialog
    connect(ui->addFileButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "Selecione o arquivo XML de Nota Fiscal",
            "",
            "Arquivos XML (*.xml);;Todos os Arquivos (*)");

        if (!filePath.isEmpty()) {
            ui->filePathLineEdit->setText(filePath);
            QMessageBox::information(this, "Arquivo Selecionado", "Arquivo carregado com sucesso!");
        }
    });

    // Conecta o botão de validação ao slot
    connect(ui->validateButton, &QPushButton::clicked, this, &MainWindow::onValidateButtonClicked);
}

void MainWindow::onValidateButtonClicked() {
    QString filePath = ui->filePathLineEdit->text();

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Por favor, selecione um arquivo XML.");
        return;
    }

    // Caminho do arquivo XSD corrigido
    QString schemaPath = "/home/pedro/trabxml2/exemplo.xsd"; // Certifique-se de que o caminho esteja correto

    // Validação do arquivo XML
    XmlValidator validator(schemaPath); // Passa o caminho correto do XSD
    QString errorMsg;
    if (validator.validate(filePath, errorMsg)) {
        QMessageBox::information(this, "Validação", "Arquivo XML validado com sucesso!");

        // Abra o dashboard após a validação
        if (!dashboard) {
            dashboard = new Dashboard(this);
        }
        dashboard->show();
    } else {
        QMessageBox::critical(this, "Erro de Validação", "Erro ao validar o arquivo XML: " + errorMsg);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    if (dashboard) {
        delete dashboard;
    }
}
