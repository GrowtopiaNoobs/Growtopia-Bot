
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void refreshBotList();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void runLoop();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_ownershipCheckbox_toggled(bool checked);

    void on_checkBox_toggled(bool checked);

    void on_followButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_9_clicked();

    void on_actionImport_triggered();

    void on_actionExport_triggered();

    void on_pushButton_5_clicked();

    void on_worldViewButton_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
