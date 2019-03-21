/** @file
 * @brief Helper function to create sink (used for both TextEditSink and
 * TextEditSinkHub)
 *
 * @ingroup
 *
 * $Id: $
 */

#pragma once

#include <boost/log/utility/setup/from_settings.hpp>

class QTextEdit;

namespace logger
{

/**
 * Create sink from @a settings and @a editor
 * @param settings
 * @param editor
 * @return
 */
boost::shared_ptr<boost::log::sinks::sink> createSink(
    const boost::log::basic_settings_section<char>& settings,
    QTextEdit* editor);

} // namespace logger
