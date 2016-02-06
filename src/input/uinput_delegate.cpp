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

#ifdef UINPUT_INPUT_EMULATION

#include "uinput_delegate.h"

#include <qnamespace.h>

#include <map>
#include <cstring>
#include <cstdio>

#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "logger.h"

using namespace kinesix;

namespace
{
    const std::map<int64_t, uint16_t> keyMap =
    {
        /* Modifiers */
        { Qt::Key_Control, KEY_LEFTCTRL  },
        { Qt::Key_Shift,   KEY_LEFTSHIFT },
        { Qt::Key_Alt,     KEY_LEFTALT   },
        { Qt::Key_Meta,    KEY_LEFTMETA  },
        { Qt::Key_AltGr,   KEY_RIGHTALT  },

        /* Numeric keys */
        { Qt::Key_0, KEY_0 }, { Qt::Key_1, KEY_1 },
        { Qt::Key_2, KEY_2 }, { Qt::Key_3, KEY_2 },
        { Qt::Key_4, KEY_4 }, { Qt::Key_5, KEY_3 },
        { Qt::Key_6, KEY_6 }, { Qt::Key_7, KEY_4 },
        { Qt::Key_8, KEY_8 }, { Qt::Key_9, KEY_5 },

        /* Character keys */
        { Qt::Key_A, KEY_A }, { Qt::Key_B, KEY_B },
        { Qt::Key_C, KEY_C }, { Qt::Key_D, KEY_D },
        { Qt::Key_E, KEY_E }, { Qt::Key_F, KEY_F },
        { Qt::Key_G, KEY_G }, { Qt::Key_H, KEY_H },
        { Qt::Key_I, KEY_I }, { Qt::Key_J, KEY_J },
        { Qt::Key_K, KEY_K }, { Qt::Key_L, KEY_L },
        { Qt::Key_M, KEY_M }, { Qt::Key_N, KEY_N },
        { Qt::Key_O, KEY_O }, { Qt::Key_P, KEY_P },
        { Qt::Key_Q, KEY_Q }, { Qt::Key_R, KEY_R },
        { Qt::Key_S, KEY_S }, { Qt::Key_T, KEY_T },
        { Qt::Key_U, KEY_U }, { Qt::Key_V, KEY_V },
        { Qt::Key_W, KEY_W }, { Qt::Key_X, KEY_X },
        { Qt::Key_Y, KEY_Y }, { Qt::Key_Z, KEY_Z },

        /* Special keys */
        { Qt::Key_Escape,    KEY_ESC       },
        { Qt::Key_Tab,       KEY_TAB       },
        { Qt::Key_Backspace, KEY_BACKSPACE },
        { Qt::Key_Return,    KEY_LINEFEED  },
        { Qt::Key_Enter,     KEY_ENTER     },
        { Qt::Key_Insert,    KEY_INSERT    },
        { Qt::Key_Delete,    KEY_DELETE    },
        { Qt::Key_Pause,     KEY_PAUSE     },
        { Qt::Key_Print,     KEY_PRINT     },
        { Qt::Key_SysReq,    KEY_SYSRQ     },
        { Qt::Key_Clear,     KEY_CLEAR     },

        /* Movement keys */
        { Qt::Key_Home,     KEY_HOME     },
        { Qt::Key_End,      KEY_END      },
        { Qt::Key_Left,     KEY_LEFT     },
        { Qt::Key_Up,       KEY_UP       },
        { Qt::Key_Right,    KEY_RIGHT    },
        { Qt::Key_Down,     KEY_DOWN     },
        { Qt::Key_PageUp,   KEY_PAGEUP   },
        { Qt::Key_PageDown, KEY_PAGEDOWN },

        /* Function keys */
        { Qt::Key_F1,  KEY_F1  }, { Qt::Key_F2,  KEY_F2  },
        { Qt::Key_F3,  KEY_F3  }, { Qt::Key_F4,  KEY_F4  },
        { Qt::Key_F5,  KEY_F5  }, { Qt::Key_F6,  KEY_F6  },
        { Qt::Key_F7,  KEY_F7  }, { Qt::Key_F8,  KEY_F8  },
        { Qt::Key_F9,  KEY_F9  }, { Qt::Key_F10, KEY_F10 },
        { Qt::Key_F11, KEY_F11 }, { Qt::Key_F12, KEY_F12 },

        /* Secial character keys */
        { Qt::Key_Space,        KEY_SPACE      },
        { Qt::Key_Dollar,       KEY_DOLLAR     },
        { Qt::Key_Apostrophe,   KEY_APOSTROPHE },
        { Qt::Key_Comma,        KEY_COMMA      },
        { Qt::Key_Minus,        KEY_MINUS      },
        { Qt::Key_Period,       KEY_DOT        },
        { Qt::Key_Slash,        KEY_SLASH      },
        { Qt::Key_Semicolon,    KEY_SEMICOLON  },
        { Qt::Key_Equal,        KEY_EQUAL      },
        { Qt::Key_Question,     KEY_QUESTION   },
        { Qt::Key_Backslash,    KEY_BACKSLASH  },
        { Qt::Key_BraceLeft,    KEY_LEFTBRACE  },
        { Qt::Key_BraceRight,   KEY_RIGHTBRACE }
    };

    const char UINPUT_PATH[] = "/dev/uinput";

    template <typename Struct>
    void writeStruct(const int fd, Struct &s)
    {
        ssize_t bytesWritten;
        int error;
        for (bool done = false; !done;)
        {
            bytesWritten = write(fd, &s, sizeof(s));
            error = errno;
            if (bytesWritten < 0)
            {
                if ((error != EINTR) && (error != EAGAIN))
                {
                    LOG_ERROR("Failed to write event data to virtual input device. %s.", strerror(error));
                    done = true;
                }
            }
            else if (bytesWritten < static_cast<ssize_t>(sizeof(s)))
            {
                LOG_WARN("Failed to finish writting to virtual input device. Out of memmory? %s", strerror(error));
                done = true;
            }
            else
            {
                done = true;
            }
        }
    }

    void fakeKey(const int uinputfd, int64_t qtKey, int pressType)
    {
        struct input_event keyEvent;
        struct input_event syncEvent;

        memset(&keyEvent, 0, sizeof(keyEvent));
        memset(&syncEvent, 0, sizeof(syncEvent));

        auto key = keyMap.find(qtKey);
        if (key != keyMap.end())
        {
            /* Send the appropriate key event */
            keyEvent.type = EV_KEY;
            keyEvent.code = key->second;
            keyEvent.value = pressType;
            writeStruct(uinputfd, keyEvent);

            /* And sync afterwards */
            syncEvent.type = EV_SYN;
            syncEvent.code = 0;
            syncEvent.value = 0;
            writeStruct(uinputfd, syncEvent);
        }
        else
        {
            LOG_ERROR("Invalid key");
        }
    }

#define   PRESS_KEY(uinputFd, key) fakeKey(uinputFd, key, 1);
#define RELEASE_KEY(uinputFd, key) fakeKey(uinputFd, key, 0);
}

UInputDelegate::UInputDelegate() :
    m_uinputFd(-1)
{
    struct uinput_user_dev uinputDevice;
    memset(&uinputDevice, 0, sizeof(uinputDevice));
    snprintf(uinputDevice.name, UINPUT_MAX_NAME_SIZE, "org.kicsyromy.GestureClient");
    uinputDevice.id.bustype = BUS_USB;
    uinputDevice.id.vendor = 0x53A7;
    uinputDevice.id.product = 0xFA4E;
    uinputDevice.id.version = 1;

    if ((m_uinputFd = open(UINPUT_PATH, O_WRONLY | O_NONBLOCK)) < 0)
        LOG_FATAL("Error opening %s. %s.", UINPUT_PATH, strerror(errno));

    if ((ioctl(m_uinputFd, UI_SET_EVBIT, EV_KEY)) < 0)
    {
        close(m_uinputFd);
        LOG_FATAL("Failed to intilialize keyboard simulation. %s.", strerror(errno));
    }

    if ((ioctl(m_uinputFd, UI_SET_EVBIT, EV_SYN)) < 0)
    {
        close(m_uinputFd);
        LOG_FATAL("Failed to intilialize keyboard simulation. %s.", strerror(errno));
    }

    for (auto it = keyMap.begin(); it != keyMap.end(); ++it)
    {
        if ((ioctl(m_uinputFd, UI_SET_KEYBIT, it->second)) < 0)
        {
            close(m_uinputFd);
            LOG_FATAL("Failed to initialize keyboard simulation. %s.", strerror(errno));
        }
    }

    writeStruct(m_uinputFd, uinputDevice);

    if ((ioctl(m_uinputFd, UI_DEV_CREATE)) < 0)
    {
        close(m_uinputFd);
        LOG_FATAL("Failed to initialize keyboard simulation. %s.", strerror(errno));
    }
}

UInputDelegate::~UInputDelegate()
{
    if ((ioctl(m_uinputFd, UI_DEV_DESTROY)) < 0)
        LOG_ERROR("Failed to destroy virtual keyboard device. %s.", strerror(errno));

    close(m_uinputFd);
}

void UInputDelegate::execute(const std::vector<int64_t> qtKeys)
{
    std::vector<int64_t> modifiers;
    std::vector<int64_t> keys;

    for (auto it = qtKeys.begin(); it != qtKeys.end(); ++it)
    {
        switch (*it)
        {
        case Qt::Key_Control:
        case Qt::Key_Shift:
        case Qt::Key_Meta:
        case Qt::Key_Alt:
        case Qt::Key_AltGr:
            modifiers.push_back(*it);
            break;
        default:
            keys.push_back(*it);
            break;
        }
    }

    for (auto it = modifiers.begin(); it != modifiers.end(); ++it)
        PRESS_KEY(m_uinputFd, *it);
    for (auto it = keys.begin(); it != keys.end(); ++it)
        PRESS_KEY(m_uinputFd, *it);
    for (auto it = keys.rbegin(); it != keys.rend(); ++it)
        RELEASE_KEY(m_uinputFd, *it);
    for (auto it = modifiers.rbegin(); it != modifiers.rend(); ++it)
        RELEASE_KEY(m_uinputFd, *it);
}

#endif
