/** @file
 * @brief
 *
 * @ingroup
 * $Id: $
 */

#include "TextEditSinkHubFactory.h"

#include "TextEditGetter.h"
#include "TextEditSinkBuilder.h"

#include <QString>

namespace logger
{

TextEditSinkHubFactory::TextEditSinkHubFactory(TextEditGetter* textEditGetter)
    : m_textEditGetter(textEditGetter)
{
}

boost::shared_ptr<boost::log::sinks::sink>
TextEditSinkHubFactory::create_sink(const settings_section& settings)
{
    if (boost::optional<std::string> param = settings["TabTitle"])
    {
        return createSink(
            settings,
            m_textEditGetter->textEdit(QString::fromUtf8(param->c_str())));
    }

    throw std::runtime_error("No TabTitle specified in settings");
}

void initTextEditSinkHubFactory(
    const boost::shared_ptr<TextEditSinkHubFactory>& factory)
{
    boost::log::register_sink_factory("TextEditHub", factory);
}

} // namespace logger
