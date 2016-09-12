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

#pragma once

/* ************************************************************************ */

// C++
#include <streambuf>
#include <ostream>

// Qt
#include <QObject>

/* ************************************************************************ */

namespace cece {
namespace gui {

/* ************************************************************************ */

/**
 * @brief Standard output stream.
 */
class StdOutStream : public QObject, public std::basic_streambuf<char>
{
    Q_OBJECT

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param stream Original stream.
     */
    explicit StdOutStream(std::ostream& stream);


    /**
     * @brief Destructor.
     */
    ~StdOutStream();


// Public Signals
signals:


    /**
     * @brief Text has been submitted.
     *
     * @param text Text to send.
     */
    void append(QString text);


// Protected Operations
protected:


    int_type overflow(int_type v) override;


    std::streamsize xsputn(const char* p, std::streamsize n) override;


// Private Data Members
private:

    /// Stream the buffer is bound to.
    std::ostream& m_stream;

    /// Pointer to old buffer.
    std::streambuf* m_oldBuf;

    /// Text buffer.
    QString m_text;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
