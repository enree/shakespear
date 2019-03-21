/** @file
 * @brief Helper function to create sink (used for both TextEditSink and
 * TextEditSinkHub)
 *
 * @ingroup
 * $Id: $
 */

#include "TextEditSinkBuilder.h"
#include "TextEditSink.h"

#include <boost/make_shared.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

#include <vector>

namespace logger
{

boost::shared_ptr<boost::log::sinks::sink> createSink(
    const boost::log::basic_settings_section<char>& settings, QTextEdit* editor)
{
    boost::shared_ptr<TextEditSink> backend
        = boost::make_shared<TextEditSink>(editor);

    boost::shared_ptr<boost::log::sinks::synchronous_sink<TextEditSink>> sink
        = boost::make_shared<boost::log::sinks::synchronous_sink<TextEditSink>>(
            backend);

    if (boost::optional<std::string> param = settings["Filter"])
    {
        sink->set_filter(boost::log::parse_filter(param.get()));
    }
    if (boost::optional<std::string> param = settings["Format"])
    {
        sink->set_formatter(boost::log::parse_formatter(param.get()));
    }

    if (boost::optional<std::string> param = settings["Limit"])
    {
        backend->setBlockLimit(boost::lexical_cast<int>(*param));
    }

    if (boost::optional<std::string> param = settings["Decorator"])
    {
        MessageDecorator decorator;

        std::vector<std::string> decorationStrings;
        boost::split(
            decorationStrings, *param, boost::is_any_of(std::string("|")));

        std::vector<std::string>::const_iterator first(
            decorationStrings.begin());
        std::vector<std::string>::const_iterator last(decorationStrings.end());
        for (; first != last; ++first)
        {
            std::vector<std::string> parsed;
            boost::split(parsed, *first, boost::is_any_of(std::string(":")));
            // One "*first" element should be in form SEVERITY:DECORATION
            if (parsed.size() != 2)
            {
                throw std::runtime_error(
                    std::string("Parse error in ") + *first);
            }
            decorator.addDecoration(
                appkit::logger::toLevel(parsed.at(0)), parsed.at(1).c_str());
        }
        backend->setMessageDecorator(decorator);
    }

    return sink;
}

} // namespace logger
