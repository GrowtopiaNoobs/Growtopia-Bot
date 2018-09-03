
//    Grotopia Bot
//    Copyright (C) 2018  Growtopia Noobs
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include "corefunc.h"
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

using namespace std;

struct BotSettings {
    string username;
    string password;
    GrowtopiaBot* bot;
};

vector<BotSettings> botsSettings;
string choosenWorld;
int owner;
bool publicOwner=false;
bool backwardWalk=false;
MainWindow* mainWindow;
void refreshBotList()
{
    mainWindow->refreshBotList();
}

bool removeNextUser=false; // hacky, don't touch

string gameVersion="2.98";

void onLoginError(string username, string errorMsg)
{
    removeNextUser=true;
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(errorMsg+" The account was removed from the list."));
    msgBox.exec();
}

#include "utilsfunc.h"
#include "userfunc.h"
#include "addbots.cpp"

 QTimer *timer;

 int runTime=0;
int renderType=0;

 void MainWindow::runLoop()
 {
    if(botsSettings.size()>0)
    {
        if(renderType==0)
            botsSettings.at(0).bot->userRender();
        else if(renderType==1)
            botsSettings.at(0).bot->userRender2();
        ui->followButton->setText(botsSettings.at(0).bot->isFollowing ? "Stop following":"Start following");
        if(!ui->followButton->isEnabled())
            ui->followButton->setEnabled(true);
    } else {
        ui->followButton->setEnabled(false);
    }
    for(int i=0; i<botsSettings.size(); i++)
    {
        botsSettings.at(i).bot->eventLoop();

        if(removeNextUser)
        {
                botsSettings.erase(botsSettings.begin()+i);
                i--;
                refreshBotList();
                removeNextUser=false;
                continue;
        }
        ui->currentWorld->setText(QString::fromStdString(botsSettings.at(i).bot->currentWorld));
        if(!botsSettings.at(i).bot->publicOwnership)
            botsSettings.at(i).bot->owner = owner;
        for (GrowtopiaBot::ObjectData x : botsSettings.at(0).bot->objects)
        {
            if(x.isGone && x.netId==owner)
                ui->ownerLabel->setText(QString::fromStdString("--- owner left the world ---"));
            else if(x.netId==botsSettings.at(i).bot->owner)
                ui->ownerLabel->setText(QString::fromStdString(x.name));
        }

    }
    if(botsSettings.size()>0 && runTime%100==0)
    {
        QString selected = ui->ownerSelect->itemText(ui->ownerSelect->currentIndex());
        ui->ownerSelect->clear();
        ui->ownerSelect->addItem(tr("-"), -1);
        for (GrowtopiaBot::ObjectData x : botsSettings.at(0).bot->objects)
        {
            if(!x.isGone)
                ui->ownerSelect->addItem(tr(x.name.c_str()), x.netId);
        }
        int a=-1;
        for (int index = 0; index < ui->ownerSelect->count(); index++)
            if(!QString::compare(ui->ownerSelect->itemText(index), selected, Qt::CaseInsensitive))
                a=index;
        if(a!=-1)
            ui->ownerSelect->setCurrentIndex(a);
    }
    runTime++;
 }

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindow=this;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(runLoop()));
    timer->start(10); //time specified in ms
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshBotList()
{
    QListView *mylist = this->findChild<QListView*>("bots_list");
    QStandardItemModel *model = new QStandardItemModel();
    mylist->setModel( model );
    QStandardItem *item;
    for(int i=0; i<botsSettings.size(); i++)
    {
        item = new QStandardItem();
        item->setData( QString::fromStdString(botsSettings.at(i).username), Qt::DisplayRole );
        item->setEditable( false );
        model->appendRow( item );
    }
}

void MainWindow::on_pushButton_clicked()
{
    AddBots * dialog = new AddBots(this);;
    //dialog.setModal(true);
    dialog->setAttribute(Qt::WA_DeleteOnClose);//we don't want memory leak

    dialog->show();
    //dialog.exec();
    //cout << "Done" << endl;
}

void MainWindow::on_pushButton_2_clicked()
{
    QModelIndexList indexes = ui->bots_list->selectionModel()->selectedIndexes();
      if(indexes.size()>0) {
        string toRemove = ui->bots_list->model()->data(indexes.first()).toString().toStdString();
        for(int i=0; i<botsSettings.size(); i++) {
            if(toRemove==botsSettings.at(i).username)
            {
                botsSettings.erase(botsSettings.begin()+i);
            }
        }
        ui->bots_list->model()->removeRow(indexes.first().row());
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    choosenWorld = ui->worldEdit->text().toStdString();
    for(int i=0; i<botsSettings.size(); i++)
    {
        string upW="";
        std::locale loc;
        for (std::string::size_type j=0; j<choosenWorld.length(); ++j)
            upW += std::toupper(choosenWorld[j],loc);
        botsSettings.at(i).bot->worldName = upW;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    owner = ui->ownerSelect->currentData().toInt();
    ui->ownerLabel->setText(ui->ownerSelect->currentText());
}

void MainWindow::on_ownershipCheckbox_toggled(bool checked)
{
    for(int i=0; i<botsSettings.size(); i++)
    {
        botsSettings.at(i).bot->publicOwnership = checked;
    }
    publicOwner=true;
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    for(int i=0; i<botsSettings.size(); i++)
    {
        botsSettings.at(i).bot->backwardWalk = checked;
    }
    backwardWalk=true;
}

void MainWindow::on_followButton_clicked()
{
    for(int i=0; i<botsSettings.size(); i++)
    {
        botsSettings.at(i).bot->isFollowing = !botsSettings.at(i).bot->isFollowing;
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    for(int i=0; i<botsSettings.size(); i++)
    {
        botsSettings.at(i).bot->respawn();
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    loadGrowtopiaTextures();
    ui->pushButton_9->setEnabled(false);
    ui->worldViewButton->setEnabled(true);
}

void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Export current bots"), "",
            tr("Growtopia bot (*.gtbot);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_3);
        botsSettings.clear();
        QString jsonData;
        in >> jsonData;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject obj;
        if(!doc.isNull())
        {
            if(doc.isObject())
            {
                obj = doc.object();
                if(obj["dataVersion"]!=1)
                {
                    QMessageBox::information(this, tr("What? Newest file than app?"),
                                    tr("Seems like you got newer file than app version. Please update your app from my channel Growtopia Noobs or return this file back to its owner."));
                    return;
                }
                for(QJsonValueRef box: obj["boxes"].toArray())
                    for(QJsonValueRef bot: box.toObject()["bots"].toArray())
                    {
                        BotSettings settings;

                        settings.username = bot.toObject()["username"].toString().toStdString();
                        settings.password = bot.toObject()["password"].toString().toStdString();
                        settings.bot = new GrowtopiaBot(settings.username,settings.password);
                        settings.bot->worldName = choosenWorld;
                        settings.bot->publicOwnership = publicOwner;
                        settings.bot->backwardWalk=backwardWalk;
                        settings.bot->gameVersion=gameVersion;
                        if(botsSettings.size()!=0)
                            settings.bot->isFollowing = botsSettings.at(0).bot->isFollowing;
                        settings.bot->userInit();
                        botsSettings.push_back(settings);

                    }
                refreshBotList();
            }
            else
            {
                QMessageBox::information(this, tr("Weird stuff inside"),
                                tr("Such weird stuff in that file. Why no JSON object? :("));
            }
        }
        else
        {
            QMessageBox::information(this, tr("No JSON???"),
                            tr("What are you actually trying to open? (Error)"));
        }
    }
    refreshBotList();
}

void MainWindow::on_actionExport_triggered()
{
    QJsonObject jobject;

    jobject["boxes"].toObject().insert("MAIN_BOX", {});
    jobject["boxes"].toObject()["MAIN_BOX"].toObject().insert("bots", {});
    QJsonArray bots;
    for(int i=0; i<botsSettings.size(); i++)
    {
        QJsonObject accountObject;
        accountObject["username"]=QString::fromStdString(botsSettings.at(i).username);
        accountObject["password"]=QString::fromStdString(botsSettings.at(i).password);
        bots.push_back(accountObject);
    }
    QJsonObject boxData;
    boxData["bots"]=bots;
    boxData["boxName"]="MAIN_BOX";
    QJsonArray boxes;
    boxes.push_back(boxData);
    jobject.insert("boxes", boxes);
    jobject["dataVersion"]=1;
    QJsonDocument doc(jobject);
    QString botsTxt = doc.toJson();
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Export current bots"), "",
            tr("Growtopia bot (*.gtbot);;All Files (*)"));
    if (fileName.isEmpty())
            return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
        QDataStream out(&file);
                    out.setVersion(QDataStream::Qt_5_3);
                    out << botsTxt;
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    // Enable minimap
    renderType=0;
}

void MainWindow::on_worldViewButton_clicked()
{
    // Enable world view
    renderType=1;
}

void MainWindow::on_pushButton_7_clicked()
{
    for(int i=0; i<botsSettings.size(); i++)
        botsSettings.at(i).bot->gameVersion=gameVersion;
    gameVersion=ui->worldEdit->text().toStdString();
}
