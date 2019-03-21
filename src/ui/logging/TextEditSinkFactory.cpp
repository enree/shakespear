/** @file
 * @brief
 *
 * @ingroup
 * $Id: $
 */

#include "TextEditSinkFactory.h"

#include "TextEditSink.h"
#include "TextEditSinkBuilder.h"

#include <QTextEdit>

#include <boost/typeof/typeof.hpp>

#include <vector>

namespace logger
{

boost::shared_ptr<boost::log::sinks::sink>
TextEditSinkFactory::create_sink(const settings_section& settings)
{
    // Read sink parameters
    std::string editorName;
    if (boost::optional<std::string> param = settings["EditorName"])
    {
        editorName = param.get();
        BOOST_AUTO(it, m_editors.find(editorName));
        if (it != m_editors.end() && it->second != nullptr)
        {
            return createSink(settings, it->second);
        }
    }
    throw std::runtime_error("No target editor specified in settings");
}

void TextEditSinkFactory::addEdit(const std::string& name, QTextEdit* editor)
{
    m_editors.insert(std::make_pair(name, editor));
}

void initTextEditSinkFactory(
    const boost::shared_ptr<TextEditSinkFactory>& factory)
{
    boost::log::register_sink_factory("TextEdit", factory);
}

} // namespace logger
