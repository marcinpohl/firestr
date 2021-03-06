/*
 * Copyright (C) 2013  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRESTR_APP_APP_EDITOR_H
#define FIRESTR_APP_APP_EDITOR_H

#include "gui/list.hpp"
#include "gui/message.hpp"
#include "session/session.hpp"
#include "message/mailbox.hpp"
#include "messages/sender.hpp"
#include "gui/app/lua_script_api.hpp"
#include "gui/app/app_service.hpp"

#include <QObject>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSignalMapper>
#include <QSyntaxHighlighter>


#include "slb/SLB.hpp"

#include <string>

namespace fire
{
    namespace gui
    {
        namespace app
        {
            struct highlight_rule
            {
                QTextCharFormat format;
                QRegExp regex;
            };

            using highlight_rules = std::vector<highlight_rule>;
            class lua_highlighter : public QSyntaxHighlighter
            {
                Q_OBJECT
                public:
                    lua_highlighter(QTextDocument* parent = 0);

                public:
                    void highlightBlock(const QString&);

                private:
                    highlight_rules _rules;
            };

            class app_editor : public message
            {
                Q_OBJECT

                public:
                    app_editor(app_service_ptr, session::session_ptr, app_ptr a = nullptr);
                    app_editor(const std::string& id, app_service_ptr, session::session_ptr, app_ptr a = nullptr);
                    ~app_editor();

                public:
                    const std::string& id();
                    const std::string& type();
                    fire::message::mailbox_ptr mail();

                public slots:
                    void run_script();
                    void send_script();
                    void save_app();
                    void check_mail();
                    void update();

                private:
                    void init();

                private:
                    std::string _id;
                    app_service_ptr _app_service;
                    session::session_ptr _session;
                    fire::message::mailbox_ptr _mail;
                    messages::sender_ptr _sender;
                    user::contact_list _contacts;

                    QTextEdit* _script;
                    lua_highlighter* _highlighter;
                    QPushButton* _save;
                    QWidget* _canvas;
                    QGridLayout* _canvas_layout;
                    list* _output;

                    lua_script_api_ptr _api;
                    app_ptr _app;
                    std::string _prev_code;
            };
            extern const std::string SCRIPT_SAMPLE;

        }
    }
}

#endif

