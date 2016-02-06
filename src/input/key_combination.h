/*
 * Copyright © 2016 Romeo Calota
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Romeo Calota
 */

#ifndef KEYCOMBINATION_H
#define KEYCOMBINATION_H

#include <cstdlib>
#include <vector>

#include "action_base.h"

namespace kinesix
{
    template <class Delegate>
    class KeyCombination : public ActionBase
    {
    public:
        KeyCombination(const std::vector<int64_t> &keys) :
            m_keys(keys)
        {
        }

    public:
        virtual void execute() override
        {
            m_delegate.execute(m_keys);
        }

    private:
        const std::vector<int64_t> m_keys;
        Delegate m_delegate;
    };
}

#endif // KEYCOMBINATION_H
