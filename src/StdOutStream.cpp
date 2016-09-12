/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "StdOutStream.hpp"

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

StdOutStream::StdOutStream(std::ostream& stream)
    : m_stream(stream)
{
    m_oldBuf = stream.rdbuf();
    stream.rdbuf(this);
}

/* ************************************************************************ */

StdOutStream::~StdOutStream()
{
    // Send remaining text
    if (!m_text.isEmpty())
        append(m_text + "\n");

    // Reset buffer
    m_stream.rdbuf(m_oldBuf);
}

/* ************************************************************************ */

StdOutStream::int_type StdOutStream::overflow(int_type v)
{
    if (v == '\n')
    {
        append(m_text);
        m_text.clear();
    }
    else
        m_text.push_back(v);

    return v;
}

/* ************************************************************************ */

std::streamsize StdOutStream::xsputn(const char* p, std::streamsize n)
{
    m_text.append(QString::fromUtf8(p, n));

    int pos = 0;
    while (pos != -1)
    {
        pos = m_text.indexOf('\n');
        if (pos != -1)
        {
            QString tmp(m_text.left(pos + 1));
            append(tmp);
            m_text.remove(0, pos + 1);
        }
    }

    return n;
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
