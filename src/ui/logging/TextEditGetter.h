/** @file
 * @brief     Sink text edit getter interface specification
 *
 * $Id: $
 */

#pragma once

class QString;
class QTextEdit;

namespace logger
{

/**
 * Sink text edit getter interface. Use in text edit sink hub factory
 */
class TextEditGetter
{
public:
    virtual ~TextEditGetter() {}

    /**
     * @return text edit to use for sink log
     */
    virtual QTextEdit* textEdit(const QString& name) = 0;
};

} // namespace logger
