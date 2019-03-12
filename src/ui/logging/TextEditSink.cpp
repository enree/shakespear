/** @file
 * @brief
 *
 * @ingroup
 * $Id: $
 */

#include "TextEditSink.h"

#include <QTextEdit>

#include <boost/bind.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/value_ref.hpp>

namespace logging = boost::log;

namespace logger
{

TextEditSink::TextEditSink(QTextEdit* textEdit) : m_textEdit(textEdit) {}

void TextEditSink::consume(
    const boost::log::record_view& rec, const string_type& command_line)
{
    if (m_textEdit != nullptr)
    {
        logging::value_ref<appkit::logger::SeverityLevel> level
            = logging::extract<appkit::logger::SeverityLevel>("Severity", rec);

        QString message = QString::fromUtf8(command_line.c_str());
        // Replace angle brackets
        message.replace("<", "&lt;");
        message.replace(">", "&gt;");

        if (!!level)
        {
            message = level.apply_visitor(boost::bind(
                &MessageDecorator::format, m_messageDecorator, _1, message));
        }

        m_textEdit->append(message);
    }
}

void TextEditSink::setMessageDecorator(const MessageDecorator& decorator)
{
    m_messageDecorator = decorator;
}

void TextEditSink::setBlockLimit(int maximumBlockCount)
{
    if (m_textEdit != nullptr)
    {
        m_textEdit->document()->setMaximumBlockCount(maximumBlockCount);
    }
}

MessageDecorator::result_type MessageDecorator::format(
    appkit::logger::SeverityLevel level, const QString& message)
{
    auto it = m_decorators.find(level);
    return it != m_decorators.end() ? it->second.arg(message) : message;
}

void MessageDecorator::addDecoration(
    appkit::logger::SeverityLevel level, const QString& decoration)
{
    m_decorators.insert(std::make_pair(level, decoration));
}

} // namespace logger
