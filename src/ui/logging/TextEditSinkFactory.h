/** @file
 * @brief
 *
 * @ingroup
 * $Id: $
 */

#pragma once

#include <boost/log/utility/setup/from_settings.hpp>

#include <QPointer>
#include <map>

class QTextEdit;

namespace logger
{

/**
    One can use the following config for TextEditSink
    Destination - type of sink, should be TextEdit (mandatory)
    EditorName - name of QTextEdit previously added to TextEditSinkFactory
    Filter - regular log filter pattern
    Format - regular log formatter pattern
    Decorator - decorator description in form
<SEVERITY>:<DECORATOR>|<SEVERITY>:<DECORATOR> <SEVERITY> should be one of
predefined severity levels (DEBUG, INFO etc) <DECORATOR> is a valid html code
with %1 placeholder

    EXAMPLE:

        [Sinks.TextEditExample]

        Destination=TextEdit
        EditorName=sample
        Filter="%Scope% = TestLogger and %Severity% > DEBUG"
        Format="%Scope% [%TimeStamp%]: <%Severity%> %Message%"
        Decorator="FATAL:<b><font color=\"red\">%1</font></b>|ERROR:<font
color=\"red\">%1</font>|WARNING:<b><font color=\"yellow\">%1</font></b>"
**/

/**
 * Sink factory for QTextEdit sink.
 */
class TextEditSinkFactory : public boost::log::sink_factory<char>
{
public:
    // Creates the sink with the provided parameters
    boost::shared_ptr<boost::log::sinks::sink>
    create_sink(const settings_section& settings);

    /**
     * Add editor to factory
     */
    void addEdit(const std::string& name, QTextEdit* editor);

private:
    std::map<std::string, QPointer<QTextEdit>> m_editors;
};

/**
 * Init sink factory
 */
void initTextEditSinkFactory(
    const boost::shared_ptr<TextEditSinkFactory>& factory);

} // namespace logger
