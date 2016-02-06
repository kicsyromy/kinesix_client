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

#ifdef X11_INPUT_EMULATION

#include "x11delegate.h"

/* Due to Qt magic this has to be included first */
/* Do not move this include at any cost          */
#include <QX11Info>

#include <map>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#include <X11/keysymdef.h>

#include <unistd.h>

using namespace kinesix;

namespace
{
    const std::map<int64_t, const char*> keyMap =
    {
        /* Modifiers */
        { Qt::Key_Control, "Control_L" },
        { Qt::Key_Shift,   "Shift_L"  },
        { Qt::Key_Alt,     "Alt_L"    },
        { Qt::Key_Meta,    "Super_L"  },
        { Qt::Key_AltGr,   "Alt_R"    },

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
        { Qt::Key_Backspace, "BackSpace" },
        { Qt::Key_Return,    "Return"    },
        { Qt::Key_Enter,     "LineFeed"  },
        { Qt::Key_Insert,    "Insert"    },
        { Qt::Key_Delete,    "Delete"    },
        { Qt::Key_Pause,     "Pause"     },
        { Qt::Key_Print,     "Print"     },
        { Qt::Key_SysReq,    "Sys_Req"   },
        { Qt::Key_Clear,     "Clear"     },

        /* Movement keys */
        { Qt::Key_Home,     "Home"     },
        { Qt::Key_End,      "End"      },
        { Qt::Key_Left,     "Left"     },
        { Qt::Key_Up,       "Up"       },
        { Qt::Key_Right,    "Right"    },
        { Qt::Key_Down,     "Down"     },
        { Qt::Key_PageUp,   "Page_Up"   },
        { Qt::Key_PageDown, "Page_Down" },

        /* Function keys */
        { Qt::Key_F1,  "F1"  }, { Qt::Key_F2,  "F2"  },
        { Qt::Key_F3,  "F3"  }, { Qt::Key_F4,  "F4"  },
        { Qt::Key_F5,  "F5"  }, { Qt::Key_F6,  "F6"  },
        { Qt::Key_F7,  "F7"  }, { Qt::Key_F8,  "F8"  },
        { Qt::Key_F9,  "F9"  }, { Qt::Key_F10, "F10" },
        { Qt::Key_F11, "F11" }, { Qt::Key_F12, "F12" },

        /* Secial character keys */
        { Qt::Key_Space,        "space"        },
        { Qt::Key_Dollar,       "dollar"       },
        { Qt::Key_Apostrophe,   "apostrophe"   },
        { Qt::Key_Comma,        "comma"        },
        { Qt::Key_Minus,        "minus"        },
        { Qt::Key_Period,       "period"       },
        { Qt::Key_Slash,        "slash"        },
        { Qt::Key_Semicolon,    "semicolon"    },
        { Qt::Key_Equal,        "equal"        },
        { Qt::Key_Question,     "question"     },
        { Qt::Key_Backslash,    "backslash"    },
        { Qt::Key_BraceLeft,    "braceleft"    },
        { Qt::Key_BraceRight,   "braceright"   }
    };

    inline void prepareX11()
    {
        Window winFocus;
        int revert;
        XGetInputFocus(QX11Info::display(), &winFocus, &revert);
        XClientMessageEvent event;
        event.window = winFocus;
        event.type = ClientMessage;
        event.message_type = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", false);
        event.format = 32;
        event.data.l[0] = 2;
        event.data.l[1] = CurrentTime;
        event.data.l[2] = 0;

        XSendEvent(QX11Info::display(), winFocus, false,
                   (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&event);
        XFlush(QX11Info::display());
    }

    inline void simulateKeyPress(const KeyCode &keyCode)
    {
        prepareX11();
        XTestFakeKeyEvent(QX11Info::display(), keyCode, true, CurrentTime);
    }

    inline void simulateKeyRelease(const KeyCode &keyCode)
    {
        prepareX11();
        XTestFakeKeyEvent(QX11Info::display(), keyCode, false, CurrentTime);
    }

}

void X11Delegate::execute(std::vector<int64_t> qtKeys)
{
    Display *display = QX11Info::display();
    KeySym keySym;
    KeyCode keyCode;
    std::vector<KeyCode> modifiers;
    std::vector<KeyCode> keys;

    for (auto it = qtKeys.begin(); it != qtKeys.end(); ++it)
    {
        switch (*it)
        {
        case Qt::Key_Control:
        case Qt::Key_Shift:
        case Qt::Key_Meta:
        case Qt::Key_Alt:
        case Qt::Key_AltGr:
        {
            keySym = XStringToKeysym(keyMap.at(*it));
            keyCode = XKeysymToKeycode(display, keySym);
            modifiers.push_back(keyCode);

            break;
        }
        default:
        {
            auto key = keyMap.find(*it);
            if (key != keyMap.end())
            {
                keySym = XStringToKeysym(key->second);
                keyCode = XKeysymToKeycode(display, keySym);
                keys.push_back(keyCode);
            }
            break;
        }
        }
    }

    for (auto it = modifiers.begin(); it != modifiers.end(); ++it)
        simulateKeyPress(*it);
    for (auto it = keys.begin(); it != keys.end(); ++it)
        simulateKeyPress(*it);
    for (auto it = keys.rbegin(); it != keys.rend(); ++it)
        simulateKeyRelease(*it);
    for (auto it = modifiers.rbegin(); it != modifiers.rend(); ++it)
        simulateKeyRelease(*it);
}

#endif
