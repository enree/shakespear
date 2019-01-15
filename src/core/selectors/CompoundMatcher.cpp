#include "CompoundMatcher.h"

#include "InvalidSelector.h"

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/type_index.hpp>

namespace shakespear
{

CompoundMatcher::CompoundMatcher() : ObjectMatcher(false) {}

void CompoundMatcher::add(std::unique_ptr<ObjectMatcher> matcher)
{
    if (matcher->unique())
    {
        for (const auto& matcherInVector : m_matchers)
        {
            if (boost::typeindex::type_id_runtime(*matcherInVector)
                == boost::typeindex::type_id_runtime(*matcher))
            {
                BOOST_THROW_EXCEPTION(exception::InvalidSelector());
            }
        }
    }

    m_matchers.push_back(std::move(matcher));
}

bool CompoundMatcher::match(const QObject& object) const
{
    return boost::algorithm::all_of(m_matchers, [&object](const auto& matcher) {
        return matcher->match(object);
    });
}

} // namespace shakespear
