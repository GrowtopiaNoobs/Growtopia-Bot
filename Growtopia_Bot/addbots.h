
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

#ifndef ADDBOTS_H
#define ADDBOTS_H

#include <QDialog>

namespace Ui {
class AddBots;
}

class AddBots : public QDialog
{
    Q_OBJECT

public:
    explicit AddBots(QWidget *parent = 0);
    ~AddBots();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddBots *ui;
};

#endif // ADDBOTS_H
