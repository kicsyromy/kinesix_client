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

#ifndef UINPUTDELEGATE_H
#define UINPUTDELEGATE_H
#ifdef UINPUT_INPUT_EMULATION

#include <cstdlib>
#include <vector>

namespace kinesix
{
    class UInputDelegate
    {
    public:
        UInputDelegate();
        ~UInputDelegate();

    public:
        void execute(const std::vector<int64_t> keys);

    private:
        int m_uinputFd;
    };
}

#endif // UINPUT_INPUT_EMULATION
#endif // UINPUTDELEGATE_H
