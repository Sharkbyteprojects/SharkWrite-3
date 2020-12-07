#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList openfile(bool&);
    void setmessages(QString, int);
    void savefile();
    QByteArray gethash();
    void sveopt(bool);
    bool susa();

private slots:
    void on_actionAbout_triggered();

    void on_actionLoat_triggered();

    void on_actionSave_triggered();

    void on_actionClose_File_triggered();

    void on_actionSave_as_triggered();

    void on_actionQuit_triggered();

    void on_actionFileHash_triggered();

    void on_plainTextEdit_textChanged();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *bar);
};
#endif // MAINWINDOW_H
