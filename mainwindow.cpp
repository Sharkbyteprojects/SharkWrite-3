#ifndef heeaderinclude
#include "mainwindow.h"
#include "ui_mainwindow.h"
#endif
#define stdstatus 6
#define hashalg QCryptographicHash::Md5

QString filename{""};
QByteArray hash;
QString textblocks{""};
QString currentmsg{""};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hash=gethash();
    ui->actionAutoave->setEnabled(false);
    ui->actionOverwrite_TXT->setChecked(ui->plainTextEdit->overwriteMode());
    ui->actionReload->setEnabled(false);
    setmessages("Idle", 3);
    if(QCoreApplication::arguments().count() > 1){
        setmessages("Args found, Call Open File", stdstatus);
        filename = QCoreApplication::arguments().at(1);
        relo();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
QByteArray MainWindow::gethash(){
    QCryptographicHash xelem(hashalg);
    xelem.addData(ui->plainTextEdit->toPlainText().toUtf8());
    return xelem.result();
}
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox mbd;
    mbd.setIcon(QMessageBox::Information);
    mbd.setText(u8"\u00a9 Sharkbyteprojects \u00b6\nMIT License \u00b6\nGITHUB: https://github.com/Sharkbyteprojects/SharkWrite-3#\u00b6");
    setmessages("About \u00a9 Sharkbyteprojects", stdstatus*3);
    mbd.exec();
}
void MainWindow::setmessages(QString text = "", int timeout=0){
    statusBar()->clearMessage();
    currentmsg=text;
    QString message{text};
    message.append(u8" \u00a6 ");
    message.append(textblocks);
    statusBar()->showMessage(message, timeout*1000);
}
/////////////////////////////////////////////////////////////////////////

//Load
void MainWindow::on_actionLoat_triggered()
{
    if(susa()){
        setmessages("Choosing File", stdstatus);
        bool error{false};
        QString files{openfile(error)[0]};
        if(!error){
            filename=files;
            on_actionReload_triggered();
        }else{
            setmessages("No File Selected!", stdstatus);
        }
    }
}

//LOAD HELPER
QStringList MainWindow::openfile(bool& error){
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec()){
        error=false;
        return dialog.selectedFiles();
    }
    error=true;
    QStringList xx;
    xx.append("");
    return xx;
}

//NEW FILE
void MainWindow::on_actionClose_File_triggered()
{
    if(susa()){
        setmessages("Cleaning up the Mess!", stdstatus);
        ui->plainTextEdit->clear();
        filename="";
        ui->actionAutoave->setEnabled(false);
        ui->actionReload->setEnabled(false);
        ui->actionAutoave->setChecked(false);
        setmessages("Complete cleaned up", stdstatus);
    }
}

//SAVE (AS)
void MainWindow::on_actionSave_triggered()
{
    sveopt(false);
}
void MainWindow::on_actionSave_as_triggered()
{
    sveopt(true);
}

//SAVE HELPER FUNCTIONS
void MainWindow::savefile(){
    ui->actionAutoave->setEnabled(true);
    MainWindow::setmessages("Saving File", stdstatus);
    std::ofstream dataout(filename.toStdString());
    if(dataout.fail()){
        QString tex{"Error in writing File out: "};
        tex.append(filename);
        setmessages(tex, stdstatus);
        return;
    }
    dataout << (ui->plainTextEdit->toPlainText().toStdString()) << std::flush;
    hash=gethash();
    ui->actionReload->setEnabled(true);
    dataout.close();
    MainWindow::setmessages("Written", stdstatus);
}
void MainWindow::sveopt(bool x){
    if(filename!=(QString)""&&!x){
        savefile();
    }else{
        setmessages("Open File Save Dialog", stdstatus);
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setFileMode(QFileDialog::AnyFile);
        if(dialog.exec()){
            filename=dialog.selectedFiles()[0];
            savefile();
        }else{
            setmessages("Abort Save", stdstatus);
        }
    }
}

//CLOSE ACTIONS
void MainWindow::closeEvent (QCloseEvent *event){
    if(!susa()){
        event->ignore();
    } else {
        event->accept();
    }
}
bool MainWindow::susa(){
    if(gethash().toBase64()!=hash.toBase64()){
        setmessages("You have unsaved Changes!", 5);
        QMessageBox msgBox;
        msgBox.setText("SharkWrite 3");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Save:{
            on_actionSave_triggered();
            return true;
        };
        case QMessageBox::Close:{
            return true;
        };
        case QMessageBox::Cancel:{
            return false;
        };
        }
    }
    return true;
}
void MainWindow::on_actionQuit_triggered()
{
    if(susa()){
        QApplication::quit();
    }
}

//SHOWHASH
void MainWindow::on_actionFileHash_triggered()
{
    setmessages("Processing Filehash", stdstatus);
    QCryptographicHash xelem(QCryptographicHash::Md5);
    QCryptographicHash sxelem(QCryptographicHash::Sha256);
    xelem.addData(ui->plainTextEdit->toPlainText().toUtf8());
    sxelem.addData(ui->plainTextEdit->toPlainText().toUtf8());
    QByteArray md5 = xelem.result();
    QByteArray sha256 = sxelem.result();
    QString message{"Hash of your Text:\nMD5: "};
    message.append(md5.toHex());
    message.append("\nSHA 256: ");
    message.append(sha256.toHex());
    message.append("\n------\n");
    message.append(textblocks);
    QMessageBox mbd;
    mbd.setIcon(QMessageBox::Information);
    QString se{filename};
    if(se==(QString)""){
        se="unnamed";
    }
    se.append(u8" \u00a6 Hash");
    mbd.setText(se);
    mbd.setInformativeText(message);
    setmessages("Open Filehash", stdstatus);
    mbd.exec();
}
//AUTOSAVE
void MainWindow::on_plainTextEdit_textChanged()
{
    if(filename!=(QString)""&&ui->actionAutoave->isChecked()){
        sveopt(false);
        setmessages("Autosave", 1);
    }
    textblocks="Lines: ";
    textblocks.append(QString::number(ui->plainTextEdit->blockCount()));
    setmessages(currentmsg, stdstatus);
}
//Reopen
void MainWindow::on_actionReload_triggered()
{
    relo();
}
//REOPEN and Open
void MainWindow::relo(){
    setmessages("Loading File", stdstatus);
    std::ifstream file(filename.toStdString());
    if(file.fail()){
        setmessages("Can't read file", stdstatus);
        return;
    }
    QString datas{""};
    int filelenght{ui->spinBox->value()+1};
    char * buf = (char*) malloc (filelenght);
    if (buf==NULL||buf==nullptr){
        setmessages("Error allocating the Buffer, try to lower the ReadSpeed", stdstatus);
        return;
    }
    setmessages("Loading File into Memory", stdstatus);
    while(file.getline(buf, filelenght)){
        if(file.fail()){
            setmessages("Can't read file", stdstatus);
            break;
        }
        datas.append(buf);
        datas.append(u8"\n");
    }
    free(buf);
    file.close();
    setmessages("Loading File into Textfield", stdstatus);
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(datas);
    ui->actionAutoave->setEnabled(true);
    ui->actionReload->setEnabled(true);
    hash=gethash();
    setmessages("Loading Complete", stdstatus);
}

void MainWindow::on_actionOverwrite_TXT_triggered()
{
    ui->plainTextEdit->setOverwriteMode(ui->actionOverwrite_TXT->isChecked());
}
