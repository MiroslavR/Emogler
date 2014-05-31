#include "conversationbrowser.h"

#include "managers/emoglercore.h"

const QString ConversationBrowser::mStyle =
        "p, li { white-space: pre-wrap; }"
        ".msg {"
        "   font-style: normal;"
        "   font-family: arial, tahoma, helvetica, sans-serif;"
        "   font-size: 20px;"
        "   color: %1;" //#FFFFFF
        "}"
        ".info {"
        "   color: %2;" //#009133
        "   font-style: italic;"
        "   font-weight: bold;"
        "}"
        ".stranger {"
        "   color: %3;" //#BF3030
        "   font-weight: bold;"
        "}"
        ".you {"
        "   color: %4;" //#574DD8
        "   font-weight: bold;"
        "}"
        ".error {"
        "   color: %5;" //#FF0000
        "}";

ConversationBrowser::ConversationBrowser(QWidget * parent) :
    QTextBrowser(parent)
{
    updateStyleSheet();
}

void ConversationBrowser::addMessage(Conversation::Message type, const QTime & time, const QString & msg, const QString & who)
{
    QString html;
    if (time.isValid())
        html += QString("<tt>[%1]</tt>&nbsp;").arg(time.toString("hh:mm:ss"));

    if (who.size()) {
        html += QString("%1:&nbsp;").arg(who);
    }

    html += QString("<span class=\"msg\">%1</span>").arg(msg.toHtmlEscaped());
    append(html);
}

void ConversationBrowser::updateStyleSheet()
{
    EmoglerCore & core = EmoglerCore::instance();
    document()->setDefaultStyleSheet(mStyle.arg(
        core.settings().value("Style/msgColor", QColor("#000000")).value<QColor>().name(),
        core.settings().value("Style/infoColor", QColor("#009133")).value<QColor>().name(),
        core.settings().value("Style/strangerColor", QColor("#BF3030")).value<QColor>().name(),
        core.settings().value("Style/youColor", QColor("#574DD8")).value<QColor>().name(),
        core.settings().value("Style/errorColor", QColor("#FF0000")).value<QColor>().name()
    ));
}
