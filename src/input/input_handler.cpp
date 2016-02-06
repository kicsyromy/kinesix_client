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

#include "input_handler.h"

#include <QEvent>
#include <QKeyEvent>

using namespace kinesix;

namespace
{
    bool isKeyModifier(const int64_t key)
    {
        bool value = false;
        switch(key)
        {
        case Qt::Key_Control:
        case Qt::Key_Shift:
        case Qt::Key_Meta:
        case Qt::Key_Alt:
        case Qt::Key_AltGr:
            value = true;
            break;
        default:
            break;
        }

        return value;
    }

    const std::map<int64_t, const char*> keyName =
    {
        /* Modifiers */
        { Qt::Key_Control, "Control" },
        { Qt::Key_Shift,   "Shift"   },
        { Qt::Key_Alt,     "Alt"     },
        { Qt::Key_Meta,    "Super"   },
        { Qt::Key_AltGr,   "Alt"     },

        /* Numeric keys */
        { Qt::Key_0, "0" }, { Qt::Key_1, "1" },
        { Qt::Key_2, "2" }, { Qt::Key_3, "3" },
        { Qt::Key_4, "4" }, { Qt::Key_5, "5" },
        { Qt::Key_6, "6" }, { Qt::Key_7, "7" },
        { Qt::Key_8, "8" }, { Qt::Key_9, "9" },

        /* Character keys */
        { Qt::Key_A, "A" }, { Qt::Key_B, "B" },
        { Qt::Key_C, "C" }, { Qt::Key_D, "D" },
        { Qt::Key_E, "E" }, { Qt::Key_F, "F" },
        { Qt::Key_G, "G" }, { Qt::Key_H, "H" },
        { Qt::Key_I, "I" }, { Qt::Key_J, "J" },
        { Qt::Key_K, "K" }, { Qt::Key_L, "L" },
        { Qt::Key_M, "M" }, { Qt::Key_N, "N" },
        { Qt::Key_O, "O" }, { Qt::Key_P, "P" },
        { Qt::Key_Q, "Q" }, { Qt::Key_R, "R" },
        { Qt::Key_S, "S" }, { Qt::Key_T, "T" },
        { Qt::Key_U, "U" }, { Qt::Key_V, "V" },
        { Qt::Key_W, "W" }, { Qt::Key_X, "X" },
        { Qt::Key_Y, "Y" }, { Qt::Key_Z, "Z" },

        /* Special keys */
        { Qt::Key_Escape,    "Escape"    },
        { Qt::Key_Tab,       "Tab"       },
        { Qt::Key_Backspace, "Backspace" },
        { Qt::Key_Return,    "Return"    },
        { Qt::Key_Enter,     "Enter"     },
        { Qt::Key_Insert,    "Insert"    },
        { Qt::Key_Delete,    "Delete"    },
        { Qt::Key_Pause,     "Pause"     },
        { Qt::Key_Print,     "Print"     },
        { Qt::Key_SysReq,    "Sys Req"   },
        { Qt::Key_Clear,     "Clear"     },

        /* Movement keys */
        { Qt::Key_Home,     "Home"     },
        { Qt::Key_End,      "End"      },
        { Qt::Key_Left,     "Left"     },
        { Qt::Key_Up,       "Up"       },
        { Qt::Key_Right,    "Right"    },
        { Qt::Key_Down,     "Down"     },
        { Qt::Key_PageUp,   "Page Up"   },
        { Qt::Key_PageDown, "Page Down" },

        /* Function keys */
        { Qt::Key_F1,  "F1"  }, { Qt::Key_F2,  "F2"  },
        { Qt::Key_F3,  "F3"  }, { Qt::Key_F4,  "F4"  },
        { Qt::Key_F5,  "F5"  }, { Qt::Key_F6,  "F6"  },
        { Qt::Key_F7,  "F7"  }, { Qt::Key_F8,  "F8"  },
        { Qt::Key_F9,  "F9"  }, { Qt::Key_F10, "F10" },
        { Qt::Key_F11, "F11" }, { Qt::Key_F12, "F12" },

        /* Secial character keys */
        { Qt::Key_Space,        "Space"       },
        { Qt::Key_Dollar,       "Dollar"      },
        { Qt::Key_Apostrophe,   "Apostrophe"  },
        { Qt::Key_Comma,        "Comma"       },
        { Qt::Key_Minus,        "Minus"       },
        { Qt::Key_Period,       "Period"      },
        { Qt::Key_Slash,        "Slash"       },
        { Qt::Key_Semicolon,    "Semicolon"   },
        { Qt::Key_Equal,        "Equal"       },
        { Qt::Key_Question,     "Question"    },
        { Qt::Key_Backslash,    "Backslash"   },
        { Qt::Key_BraceLeft,    "Left Brace"  },
        { Qt::Key_BraceRight,   "Right Brace" }
    };
}

InputHandler::InputHandler() :
    m_pressedKeys()
{
}

void InputHandler::startListening(QObject &target)
{
    m_target = &target;
    m_target->installEventFilter(this);
}

bool InputHandler::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        int64_t pressedKey = static_cast<int64_t>(static_cast<QKeyEvent *>(event)->key());
        emit keyPressed(keyName.at(pressedKey), m_pressedKeys.empty());
        m_pressedKeys.push_back(pressedKey);

        if (!isKeyModifier(pressedKey))
        {
            m_target->removeEventFilter(this);
            m_target = Q_NULLPTR;
            emit keybindFinished(m_pressedKeys);
            m_pressedKeys.clear();
        }

        return true;
    }

    if (event->type() == QEvent::FocusOut)
    {
        m_target->removeEventFilter(this);
        m_target = Q_NULLPTR;
        emit keybindFinished(m_pressedKeys, true);
        m_pressedKeys.clear();
    }

    return QObject::eventFilter(object, event);
}
