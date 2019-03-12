/** @file
 * @brief
 *
 * $Id: $
 */

#pragma once

#include "log/SeverityLevel.h"

#include <boost/log/core/record_view.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sinks/frontend_requirements.hpp>
#include <boost/log/trivial.hpp>

#include <QPointer>
#include <QString>

#include <map>

class QTextEdit;

namespace logger
{

/**
 * Class for formatting messages depending on it's severity
 */
class MessageDecorator
{
public:
    /**
     * Result type (for apply_visitor)
     */
    typedef QString result_type;
    /**
     * Format operation
     */
    result_type
    format(appkit::logger::SeverityLevel level, const QString& message);
    /**
     * Add decoration for specific severity @a level
     */
    void addDecoration(
        appkit::logger::SeverityLevel level, const QString& decoration);

private:
    std::map<appkit::logger::SeverityLevel, QString> m_decorators;
};

/**
 * Sync backend for QTextEdit
 */
class TextEditSink : public boost::log::sinks::basic_formatted_sink_backend<
                         char,
                         boost::log::sinks::combine_requirements<
                             boost::log::sinks::synchronized_feeding>::type>
{
public:
    explicit TextEditSink(QTextEdit* textEdit);
    /**
     * Consume incoming message
     */
    void consume(
        const boost::log::record_view& rec, const string_type& command_line);
    /**
     * Set decorator for message
     */
    void setMessageDecorator(const MessageDecorator& decorator);
    /**
     * Set maximum block count for text edit
     */
    void setBlockLimit(int maximumBlockCount);

private:
    QPointer<QTextEdit> m_textEdit;
    MessageDecorator m_messageDecorator;
};

} // namespace logger
