#ifndef CONVERSATIONBROWSER_H
#define CONVERSATIONBROWSER_H

#include <QTextBrowser>
#include <QTime>

#include "conversation/conversation.h"

class ConversationBrowser : public QTextBrowser
{
    Q_OBJECT

    public:
        explicit ConversationBrowser(QWidget * parent = 0);

    signals:

    public slots:
        void addMessage(Conversation::Message type, const QTime & time, const QString & msg, const QString & who);
        void updateStyleSheet();

    private:
        static const QString mStyle;

};

#endif // CONVERSATIONBROWSER_H
