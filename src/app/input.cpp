/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#include "app/input.h"


#include "common/logger.h"
#include "common/restext.h"

#include "graphics/engine/engine.h"

#include "object/robotmain.h"


#include <sstream>


template<> CInput* CSingleton<CInput>::m_instance = nullptr;

CInput::CInput()
{
    m_kmodState = 0;
    m_trackedKeys = 0;
    m_mousePos = Math::Point();
    m_mouseButtonsState = 0;
    
    m_joystickDeadzone = 0.2f;
    SetDefaultInputBindings();
}

void CInput::EventProcess(Event& event)
{
    if(event.type == EVENT_KEY_DOWN ||
       event.type == EVENT_KEY_UP)
    {
        // Use the occasion to update kmods
        m_kmodState = event.kmodState;
    }
    
    // Use the occasion to update mouse button state
    if (event.type == EVENT_MOUSE_BUTTON_DOWN)
    {
        m_mouseButtonsState |= event.mouseButton.button;
    }
    if(event.type == EVENT_MOUSE_BUTTON_UP)
    {
        m_mouseButtonsState &= ~event.mouseButton.button;
    }
    
    if (event.type == EVENT_KEY_DOWN)
    {
        if      (event.key.key == KEY(KP8))
            m_trackedKeys |= TRKEY_NUM_UP;
        else if (event.key.key == KEY(KP2))
            m_trackedKeys |= TRKEY_NUM_DOWN;
        else if (event.key.key == KEY(KP4))
            m_trackedKeys |= TRKEY_NUM_LEFT;
        else if (event.key.key == KEY(KP6))
            m_trackedKeys |= TRKEY_NUM_RIGHT;
        else if (event.key.key == KEY(KP_PLUS))
            m_trackedKeys |= TRKEY_NUM_PLUS;
        else if (event.key.key == KEY(KP_MINUS))
            m_trackedKeys |= TRKEY_NUM_MINUS;
        else if (event.key.key == KEY(PAGEUP))
            m_trackedKeys |= TRKEY_PAGE_UP;
        else if (event.key.key == KEY(PAGEDOWN))
            m_trackedKeys |= TRKEY_PAGE_DOWN;
    }
    else if (event.type == EVENT_KEY_UP)
    {
        if      (event.key.key == KEY(KP8))
            m_trackedKeys &= ~TRKEY_NUM_UP;
        else if (event.key.key == KEY(KP2))
            m_trackedKeys &= ~TRKEY_NUM_DOWN;
        else if (event.key.key == KEY(KP4))
            m_trackedKeys &= ~TRKEY_NUM_LEFT;
        else if (event.key.key == KEY(KP6))
            m_trackedKeys &= ~TRKEY_NUM_RIGHT;
        else if (event.key.key == KEY(KP_PLUS))
            m_trackedKeys &= ~TRKEY_NUM_PLUS;
        else if (event.key.key == KEY(KP_MINUS))
            m_trackedKeys &= ~TRKEY_NUM_MINUS;
        else if (event.key.key == KEY(PAGEUP))
            m_trackedKeys &= ~TRKEY_PAGE_UP;
        else if (event.key.key == KEY(PAGEDOWN))
            m_trackedKeys &= ~TRKEY_PAGE_DOWN;
    }
    
    
    
    if(event.type == EVENT_KEY_DOWN ||
       event.type == EVENT_KEY_UP)
    {
        event.key.slot = FindBinding(event.key.key);
    }
    
    event.trackedKeysState = m_trackedKeys;
    event.kmodState = m_kmodState;
    event.mousePos = m_mousePos;
    event.mouseButtonsState = m_mouseButtonsState;
    
    
    
    /* Motion vector management */
    
    if (event.type == EVENT_KEY_DOWN)
    {
        if (event.key.slot == INPUT_SLOT_UP   ) m_keyMotion.y =  1.0f;
        if (event.key.slot == INPUT_SLOT_DOWN ) m_keyMotion.y = -1.0f;
        if (event.key.slot == INPUT_SLOT_LEFT ) m_keyMotion.x = -1.0f;
        if (event.key.slot == INPUT_SLOT_RIGHT) m_keyMotion.x =  1.0f;
        if (event.key.slot == INPUT_SLOT_GUP  ) m_keyMotion.z =  1.0f;
        if (event.key.slot == INPUT_SLOT_GDOWN) m_keyMotion.z = -1.0f;
    }
    else if (event.type == EVENT_KEY_UP)
    {
        if (event.key.slot == INPUT_SLOT_UP   ) m_keyMotion.y = 0.0f;
        if (event.key.slot == INPUT_SLOT_DOWN ) m_keyMotion.y = 0.0f;
        if (event.key.slot == INPUT_SLOT_LEFT ) m_keyMotion.x = 0.0f;
        if (event.key.slot == INPUT_SLOT_RIGHT) m_keyMotion.x = 0.0f;
        if (event.key.slot == INPUT_SLOT_GUP  ) m_keyMotion.z = 0.0f;
        if (event.key.slot == INPUT_SLOT_GDOWN) m_keyMotion.z = 0.0f;
    }
    else if (event.type == EVENT_JOY_AXIS)
    {
        if (event.joyAxis.axis == GetJoyAxisBinding(JOY_AXIS_SLOT_X).axis)
        {
            m_joyMotion.x = Math::Neutral(event.joyAxis.value / 32768.0f, m_joystickDeadzone);
            if (GetJoyAxisBinding(JOY_AXIS_SLOT_X).invert)
                m_joyMotion.x *= -1.0f;
        }
        
        if (event.joyAxis.axis == GetJoyAxisBinding(JOY_AXIS_SLOT_Y).axis)
        {
            m_joyMotion.y = Math::Neutral(event.joyAxis.value / 32768.0f, m_joystickDeadzone);
            if (GetJoyAxisBinding(JOY_AXIS_SLOT_Y).invert)
                m_joyMotion.y *= -1.0f;
        }
        
        if (event.joyAxis.axis == GetJoyAxisBinding(JOY_AXIS_SLOT_Z).axis)
        {
            m_joyMotion.z = Math::Neutral(event.joyAxis.value / 32768.0f, m_joystickDeadzone);
            if (GetJoyAxisBinding(JOY_AXIS_SLOT_Z).invert)
                m_joyMotion.z *= -1.0f;
        }
    }
    
    event.motionInput = Math::Clamp(m_joyMotion + m_keyMotion, Math::Vector(-1.0f, -1.0f, -1.0f), Math::Vector(1.0f, 1.0f, 1.0f));
}

void CInput::MouseMove(Math::IntPoint pos)
{
    m_mousePos = Gfx::CEngine::GetInstancePointer()->WindowToInterfaceCoords(pos);
}

int CInput::GetKmods() const
{
    return m_kmodState;
}

bool CInput::GetKmodState(int kmod) const
{
    return (m_kmodState & kmod) != 0;
}

bool CInput::GetTrackedKeyState(TrackedKey key) const
{
    return (m_trackedKeys & key) != 0;
}

bool CInput::GetMouseButtonState(int index) const
{
    return (m_mouseButtonsState & (1<<index)) != 0;
}

void CInput::ResetKeyStates()
{
    CLogger::GetInstancePointer()->Trace("Reset key states\n");
    m_trackedKeys = 0;
    m_kmodState = 0;
    m_keyMotion = Math::Vector(0.0f, 0.0f, 0.0f);
    m_joyMotion = Math::Vector(0.0f, 0.0f, 0.0f);
}

Math::Point CInput::GetMousePos() const
{
    return m_mousePos;
}

void CInput::SetDefaultInputBindings()
{
    for (int i = 0; i < INPUT_SLOT_MAX; i++)
    {
        m_inputBindings[i].primary = m_inputBindings[i].secondary = KEY_INVALID;
    }
    
    for (int i = 0; i < JOY_AXIS_SLOT_MAX; i++)
    {
        m_joyAxisBindings[i].axis = AXIS_INVALID;
        m_joyAxisBindings[i].invert = false;
    }
    
    m_inputBindings[INPUT_SLOT_LEFT   ].primary   = KEY(LEFT);
    m_inputBindings[INPUT_SLOT_RIGHT  ].primary   = KEY(RIGHT);
    m_inputBindings[INPUT_SLOT_UP     ].primary   = KEY(UP);
    m_inputBindings[INPUT_SLOT_DOWN   ].primary   = KEY(DOWN);
    m_inputBindings[INPUT_SLOT_LEFT   ].secondary = KEY(a);
    m_inputBindings[INPUT_SLOT_RIGHT  ].secondary = KEY(d);
    m_inputBindings[INPUT_SLOT_UP     ].secondary = KEY(w);
    m_inputBindings[INPUT_SLOT_DOWN   ].secondary = KEY(s);
    m_inputBindings[INPUT_SLOT_GUP    ].primary   = VIRTUAL_KMOD(SHIFT);
    m_inputBindings[INPUT_SLOT_GDOWN  ].primary   = VIRTUAL_KMOD(CTRL);
    m_inputBindings[INPUT_SLOT_CAMERA ].primary   = KEY(SPACE);
    //    m_inputBindings[INPUT_SLOT_CAMERA ].secondary = VIRTUAL_JOY(2);
    m_inputBindings[INPUT_SLOT_DESEL  ].primary   = KEY(KP0);
    //    m_inputBindings[INPUT_SLOT_DESEL  ].secondary = VIRTUAL_JOY(6);
    m_inputBindings[INPUT_SLOT_ACTION ].primary   = KEY(RETURN);
    //    m_inputBindings[INPUT_SLOT_ACTION ].secondary = VIRTUAL_JOY(1);
    m_inputBindings[INPUT_SLOT_ACTION ].secondary = KEY(e);
    m_inputBindings[INPUT_SLOT_NEAR   ].primary   = KEY(KP_PLUS);
    //    m_inputBindings[INPUT_SLOT_NEAR   ].secondary = VIRTUAL_JOY(5);
    m_inputBindings[INPUT_SLOT_AWAY   ].primary   = KEY(KP_MINUS);
    //    m_inputBindings[INPUT_SLOT_AWAY   ].secondary = VIRTUAL_JOY(4);
    m_inputBindings[INPUT_SLOT_NEXT   ].primary   = KEY(TAB);
    //    m_inputBindings[INPUT_SLOT_NEXT   ].secondary = VIRTUAL_JOY(3);
    m_inputBindings[INPUT_SLOT_HUMAN  ].primary   = KEY(HOME);
    //    m_inputBindings[INPUT_SLOT_HUMAN  ].secondary = VIRTUAL_JOY(7);
    m_inputBindings[INPUT_SLOT_QUIT   ].primary   = KEY(ESCAPE);
    m_inputBindings[INPUT_SLOT_HELP   ].primary   = KEY(F1);
    m_inputBindings[INPUT_SLOT_PROG   ].primary   = KEY(F2);
    m_inputBindings[INPUT_SLOT_CBOT   ].primary   = KEY(F3);
    m_inputBindings[INPUT_SLOT_VISIT  ].primary   = KEY(KP_PERIOD);
    m_inputBindings[INPUT_SLOT_SPEED10].primary   = KEY(F4);
    m_inputBindings[INPUT_SLOT_SPEED15].primary   = KEY(F5);
    m_inputBindings[INPUT_SLOT_SPEED20].primary   = KEY(F6);
    
    m_joyAxisBindings[JOY_AXIS_SLOT_X].axis = 0;
    m_joyAxisBindings[JOY_AXIS_SLOT_Y].axis = 1;
    m_joyAxisBindings[JOY_AXIS_SLOT_Z].axis = 2;
}

void CInput::SetInputBinding(InputSlot slot, InputBinding binding)
{
    unsigned int index = static_cast<unsigned int>(slot);
    m_inputBindings[index] = binding;
}

const InputBinding& CInput::GetInputBinding(InputSlot slot)
{
    unsigned int index = static_cast<unsigned int>(slot);
    return m_inputBindings[index];
}

void CInput::SetJoyAxisBinding(JoyAxisSlot slot, JoyAxisBinding binding)
{
    unsigned int index = static_cast<unsigned int>(slot);
    m_joyAxisBindings[index] = binding;
}

const JoyAxisBinding& CInput::GetJoyAxisBinding(JoyAxisSlot slot)
{
    unsigned int index = static_cast<unsigned int>(slot);
    return m_joyAxisBindings[index];
}

void CInput::SetJoystickDeadzone(float zone)
{
    m_joystickDeadzone = zone;
}

float CInput::GetJoystickDeadzone()
{
    return m_joystickDeadzone;
}

InputSlot CInput::FindBinding(unsigned int key)
{
    for (int i = 0; i < INPUT_SLOT_MAX; i++)
    {
        InputSlot slot = static_cast<InputSlot>(i);
        InputBinding b = GetInputBinding(slot);
        if(b.primary == key || b.secondary == key)
            return slot;
    }
    return INPUT_SLOT_MAX;
}

std::string CInput::SaveKeyBindings()
{
    std::stringstream key;
    for (int i = 0; i < INPUT_SLOT_MAX; i++)
    {
        InputBinding b = GetInputBinding(static_cast<InputSlot>(i));
        
        key << b.primary << " ";
        key << b.secondary << "  ";
    }
    return key.str();
}

void CInput::LoadKeyBindings(std::string keys)
{
    std::stringstream skey;
    skey.str(keys);
    for (int i = 0; i < INPUT_SLOT_MAX; i++)
    {
        InputBinding b;
        skey >> b.primary;
        skey >> b.secondary;
        SetInputBinding(static_cast<InputSlot>(i), b);
    }
}

static std::map<InputSlot, std::string> keyTable =
{
    { INPUT_SLOT_LEFT,     "left"    },
    { INPUT_SLOT_RIGHT,    "right"   },
    { INPUT_SLOT_UP,       "up"      },
    { INPUT_SLOT_DOWN,     "down"    },
    { INPUT_SLOT_GUP,      "gup"     },
    { INPUT_SLOT_GDOWN,    "gdown"   },
    { INPUT_SLOT_CAMERA,   "camera"  },
    { INPUT_SLOT_DESEL,    "desel"   },
    { INPUT_SLOT_ACTION,   "action"  },
    { INPUT_SLOT_NEAR,     "near"    },
    { INPUT_SLOT_AWAY,     "away"    },
    { INPUT_SLOT_NEXT,     "next"    },
    { INPUT_SLOT_HUMAN,    "human"   },
    { INPUT_SLOT_QUIT,     "quit"    },
    { INPUT_SLOT_HELP,     "help"    },
    { INPUT_SLOT_PROG,     "prog"    },
    { INPUT_SLOT_CBOT,     "cbot"    },
    { INPUT_SLOT_VISIT,    "visit"   },
    { INPUT_SLOT_SPEED10,  "speed10" },
    { INPUT_SLOT_SPEED15,  "speed15" },
    { INPUT_SLOT_SPEED20,  "speed20" }
};

InputSlot CInput::SearchKeyById(std::string id)
{
    for(auto& key : keyTable)
    {
        if ( id == key.second )
        {
            return key.first;
        }
    }
    return INPUT_SLOT_MAX;
}

std::string CInput::GetKeysString(InputBinding b)
{
    std::ostringstream ss;
    if ( b.primary != KEY_INVALID )
    {
        std::string iNameStr;
        if ( GetResource(RES_KEY, b.primary, iNameStr) )
        {
            ss << iNameStr;
            
            if ( b.secondary != KEY_INVALID )
            {
                if ( GetResource(RES_KEY, b.secondary, iNameStr) )
                {
                    std::string textStr;
                    GetResource(RES_TEXT, RT_KEY_OR, textStr);
                    
                    ss << textStr << iNameStr;
                }
            }
        }
    }
    else
    {
        return "?";
    }
    return ss.str();
}


std::string CInput::GetKeysString(InputSlot slot)
{
    InputBinding b = GetInputBinding(slot);
    return GetKeysString(b);
}