#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir().mkpath(QFileInfo(path).absolutePath());

    ui->listWidget->installEventFilter(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listWidget->setFocusPolicy(Qt::StrongFocus);
    ui->listWidget->setFocus();
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);

    connect(ui->txtTask, &QLineEdit::returnPressed, this, &MainWindow::on_btnAdd_clicked);
    connect(ui->listWidget->model(), &QAbstractItemModel::rowsMoved, this, &MainWindow::saveTasks);
    connect(ui->listWidget, &QListWidget::itemChanged, this, &MainWindow::saveTasks);

    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()){

        QString line= in.readLine().trimmed();
        if(line.isEmpty()) continue;
        QStringList parts = line.split("|");
        if(parts.size()==2){
            QListWidgetItem* item = new QListWidgetItem(parts[1], ui->listWidget);
            item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
            item->setCheckState(parts[0] == "1" ? Qt::Checked : Qt::Unchecked);
            ui->listWidget->addItem(item);
        }

    }

    file.close();

}

MainWindow::~MainWindow()
{
    delete ui;

    QFile file(path);

    if(!file.open(QIODevice::ReadWrite)){
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream out(&file);

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        out<< (item->checkState() == Qt::Checked ? "1|" : "0|")<<item->text() << "\n";
    }


    file.close();

}

void MainWindow::saveTasks()
{
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream out(&file);
    for(int i=0; i < ui->listWidget->count(); ++i){
        QListWidgetItem* item = ui->listWidget->item(i);
        out << (item->checkState() == Qt::Checked ? "1|" : "0|") << item->text() << "\n";
        }

    file.close();
}


void MainWindow::on_btnAdd_clicked()
{

    QString task = ui->txtTask->text();
    if(!task.isEmpty()){
        QListWidgetItem* item = new QListWidgetItem(task, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
        ui->txtTask->clear();
        ui->txtTask->setFocus();
        saveTasks();
    }

}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->listWidget && event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key()==Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace){
            QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
            for(QListWidgetItem* item : selectedItems){
                delete ui->listWidget->takeItem(ui->listWidget->row(item));
              }
            saveTasks();
            return true;
        }

    }
    return QMainWindow::eventFilter(obj, event);
}


void MainWindow::on_btnRemove_clicked()
{
    QListWidgetItem* item= ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;
    saveTasks();

}


void MainWindow::on_btnRemoveAll_clicked()
{
    if(QMessageBox::question(this, "Confirm", "Remove all tasks?") == QMessageBox::Yes){
        ui->listWidget->clear();
        saveTasks();
    }

}





