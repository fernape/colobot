/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2015, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
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

#pragma once

// Modules inlcude
#include "CBot/CBotUtils.h"

#include "CBot/CBotEnums.h"

// Local include

// Global include
#include <map>

/**
 * \brief CBotString Class used to work on strings
 * TODO: do not use this class, code should be refactored to use std::string instead
 */
class CBotString
{
public:

    /**
     * \brief CBotString Creates an empty string
     */
    CBotString();

    /**
     * \brief CBotString
     * \param p
     */
    CBotString(const char* p);

    /**
     * \brief CBotString
     * \param p
     */
    CBotString(const std::string& p);

    /**
     * \brief CBotString
     * \param p
     */
    CBotString(const CBotString& p);

    /**
     * \brief CBotString Destructor.
     */
    ~CBotString();

    /**
     * \brief Empty Clear the internal string.
     */
    void Empty();

    /**
     * \brief IsEmpty Check if the string is empty.
     * \return True if the sting is empty false otherwise.
     */
    bool IsEmpty() const;

    /**
     * \brief GetLength Get the string length.
     * \return The size of the string.
     */
    int GetLength();

    /**
     * \brief Find Find the position of a character in a string starting from
     *             the beginning of the string.
     * \param c    The character to find.
     * \return     The position of the character or -1 if the character was not
     *             found.
     * \see        ReverseFind(const char c)
     */
    int Find(const char c);

    /**
     * \brief Find Find the position of a string in a string starting from the
     *             beginning of the string.
     * \param lpsz The string to find.
     * \return     The position of the string or -1 if the string was not
     *             found.
     * \see        ReverseFind(const char* lpsz)
     */
    int Find(const char* lpsz);

    /**
     * \brief Find Find the position of a character in a string starting from
     *             the end of the string.
     * \param c    The character to find.
     * \return     The position of the character or -1 if the character was not
     *             found.
     * \see        Find(const char c)
     */
    int ReverseFind(const char c);

    /**
     * \brief Find Find the position of a string in a string starting from the
     *             end of the string.
     * \param lpsz The string to find.
     * \return     The string of the character or -1 if the string was not
     *             found.
     * \see        Find(const char* lpsz)
     */
    int ReverseFind(const char* lpsz);

    /**
     * \brief Mid    Return a part of a string from a starting index and until
     *               the end of the string with a limited size.
     * \param nFirst The start index of the character in the string.
     * \param lg     The size limit. Default value is 2000.
     * \return       The exctracted string.
     */
    CBotString Mid(int start, int lg=-1);

    /**
     * \brief Left   Return a part of a string starting from the left.
     * \param nCount The number of character to retreive.
     * \return       The exctracted string.
     */
    CBotString Left(int nCount) const;

    /**
     * \brief Right  Return a part of a string starting from the right.
     * \param nCount The number of character to retreive.
     * \return       The exctracted string.
     */
    CBotString Right(int nCount) const;

    /**
     * \brief MakeUpper Uppercase the string.
     */
    void MakeUpper();

    /**
     * \brief MakeLower Lowercase the string.
     */
    void MakeLower();

    /**
     * @brief CStr Convert the CBotString to a C string.
     * @return     A C string string.
     */
    const char* CStr() const;

    /**
     * \brief Overloaded oprators to work on CBotString classes
     */
    const CBotString& operator=(const CBotString& stringSrc);
    const CBotString& operator=(const char ch);
    const CBotString& operator=(const char* pString);
    CBotString operator+(const CBotString& str);

    const CBotString& operator+=(const char ch);
    const CBotString& operator+=(const CBotString& str);
    bool              operator==(const CBotString& str);
    bool              operator==(const char* p);
    bool              operator!=(const CBotString& str);
    bool              operator!=(const char* p);
    bool              operator>(const CBotString& str);
    bool              operator>(const char* p);
    bool              operator>=(const CBotString& str);
    bool              operator>=(const char* p);
    bool              operator<(const CBotString& str);
    bool              operator<(const char* p);
    bool              operator<=(const CBotString& str);
    bool              operator<=(const char* p);

                      operator const char*() const;           // as a C string


private:

    //! \brief String
    std::string m_str;
};
