/** @file
 * @brief
 *
 * @ingroup
 * $Id: $
 */

#pragma once

#include <boost/log/utility/setup/from_settings.hpp>

namespace logger
{

class TextEditGetter;

/**
    One can use the following config for TextEditSinkHubFactory
    Destination - type of sink, should be TextEditHub (mandatory)
    TabTitle - string for QTabWidget tab title
    Filter - regular log filter pattern
    Format - regular log formatter pattern
    Decorator - decorator (like in TextEditSink)

    EXAMPLE:

    TabTitle="Critical alarms"
    Destination=TextEditHub
    Filter="%Severity% >= WARNING"
    Format="%Severity% [%TimeStamp(format=\"%Y.%m.%d %H:%M:%S\")%]:
<%pid%,%ppid%,%uid%,%gid%,%euid%> %Message%" Decorator="FATAL:<font
color=\"red\">%1</font>|ERROR:<font color=\"red\">%1</font>|WARNING:<font
color=\"red\">%1</font>"

**/
class TextEditSinkHubFactory : public boost::log::sink_factory<char>
{
public:
    /**
     * Create TextEditSinkHubFactory for @a sinkHub
     */
    explicit TextEditSinkHubFactory(TextEditGetter* textEditGetter);

    // Creates the sink with the provided parameters
    boost::shared_ptr<boost::log::sinks::sink>
    create_sink(const settings_section& settings);

private:
    TextEditGetter* m_textEditGetter;
};

/**
 * Init sink factory
 */
void initTextEditSinkHubFactory(
    const boost::shared_ptr<TextEditSinkHubFactory>& factory);

} // namespace logger
