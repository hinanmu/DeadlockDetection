#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simplifyfun.h"
#include "string"
#include "vector"
#include "iostream"
#include <QFileDialog>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                          tr("Open Spreadsheet"), ".",
                                          tr("Spreadsheet files (*.txt)"));
    if (!fileName.isEmpty()){
        ui->lineEdit->setText(fileName);
       }

}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->setText("");
    string fileName = ui->lineEdit->text().toStdString();
    vector<string> simplify_msg = simplify(fileName);

    for(int i = 0; i < simplify_msg.size();i++){
        ui->textBrowser->append(QString::fromStdString(simplify_msg[i] + "\n"));
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->lineEdit->setText("");
    ui->textBrowser->setText("");
}
