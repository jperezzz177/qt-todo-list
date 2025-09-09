#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnRemoveAll_clicked();

    void saveTasks();

protected:
    //void keyPressEvent(QKeyEvent *event) override;

    bool eventFilter (QObject *obj, QEvent *event) override;

    //void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

    QString path= QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+ "/toDoFileOG.txt";
};
#endif // MAINWINDOW_H
