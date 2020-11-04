/* poppler-private.cc: qt interface to poppler
 * Copyright (C) 2005, Net Integration Technologies, Inc.
 * Copyright (C) 2006, 2011, 2015, 2017 by Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2008, 2010, 2011 by Pino Toscano <pino@kde.org>
 * Copyright (C) 2013 by Thomas Freitag <Thomas.Freitag@alfa.de>
 * Copyright (C) 2013 Adrian Johnson <ajohnson@redneon.com>
 * Copyright (C) 2016 Jakub Alba <jakubalba@gmail.com>
 * Inspired on code by
 * Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>
 * Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "poppler-private.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QVariant>

#include <Link.h>
#include <Outline.h>
#include <PDFDocEncoding.h>
#include <UnicodeMap.h>

namespace Poppler {

namespace Debug {

    static void qDebugDebugFunction(const QString &message, const QVariant & /*closure*/)
    {
        qDebug() << message;
    }

    PopplerDebugFunc debugFunction = qDebugDebugFunction;
    QVariant debugClosure;

}

    void setDebugErrorFunction(PopplerDebugFunc function, const QVariant &closure)
    {
        Debug::debugFunction = function ? function : Debug::qDebugDebugFunction;
        Debug::debugClosure = closure;
    }

    void qt4ErrorFunction(ErrorCategory /*category*/, Goffset pos, const char *msg)
    {
        QString emsg;

        if (pos >= 0)
        {
            emsg = QString::fromLatin1("Error (%1): ").arg(pos);
        }
        else
        {
            emsg = QString::fromLatin1("Error: ");
        }
        emsg += QString::fromAscii(msg);
        (*Debug::debugFunction)(emsg, Debug::debugClosure);
    }

    QString unicodeToQString(const Unicode* u, int len) {
        const UnicodeMap *utf8Map = globalParams->getUtf8Map();

        // ignore the last character if it is 0x0
        if ((len > 0) && (u[len - 1] == 0))
        {
            --len;
        }

        GooString convertedStr;
        for (int i = 0; i < len; ++i)
        {
            char buf[8];
            const int n = utf8Map->mapUnicode(u[i], buf, sizeof(buf));
            convertedStr.append(buf, n);
        }

        return QString::fromUtf8(convertedStr.c_str(), convertedStr.getLength());
    }

    QString UnicodeParsedString(const GooString *s1) {
        return (s1) ? UnicodeParsedString(s1->toStr()) : QString();
    }

    QString UnicodeParsedString(const std::string& s1) {
        if ( s1.empty() )
            return QString();

        if ( GooString::hasUnicodeMarker(s1) || GooString::hasUnicodeMarkerLE(s1) )
        {
            return QString::fromUtf16(reinterpret_cast<const ushort *>(s1.c_str()), s1.size() / 2);
        }
        else
        {
            int stringLength;
            const char *cString = pdfDocEncodingToUTF16(s1, &stringLength);
            auto result = QString::fromUtf16(reinterpret_cast<const ushort *>(cString), stringLength / 2);
            delete[] cString;
            return result;
        }
    }

    GooString *QStringToUnicodeGooString(const QString &s) {
        int len = s.length() * 2 + 2;
        char *cstring = (char *)gmallocn(len, sizeof(char));
        cstring[0] = (char)0xfe;
        cstring[1] = (char)0xff;
        for (int i = 0; i < s.length(); ++i)
        {
            cstring[2+i*2] = s.at(i).row();
            cstring[3+i*2] = s.at(i).cell();
        }
        GooString *ret = new GooString(cstring, len);
        gfree(cstring);
        return ret;
    }

    GooString *QStringToGooString(const QString &s) {
        int len = s.length();
        char *cstring = (char *)gmallocn(s.length(), sizeof(char));
        for (int i = 0; i < len; ++i)
            cstring[i] = s.at(i).unicode();
        GooString *ret = new GooString(cstring, len);
        gfree(cstring);
        return ret;
    }

    GooString *QDateTimeToUnicodeGooString(const QDateTime &dt) {
        if (!dt.isValid()) {
            return NULL;
        }

        return QStringToUnicodeGooString(dt.toUTC().toString("yyyyMMddhhmmss+00'00'"));
    }

    static void linkActionToTocItem( const ::LinkAction * a, DocumentData * doc, QDomElement * e )
    {
        if ( !a || !e )
            return;

        switch ( a->getKind() )
        {
            case actionGoTo:
            {
                // page number is contained/referenced in a LinkGoTo
                const LinkGoTo * g = static_cast< const LinkGoTo * >( a );
                const LinkDest * destination = g->getDest();
                if ( !destination && g->getNamedDest() )
                {
                    // no 'destination' but an internal 'named reference'. we could
                    // get the destination for the page now, but it's VERY time consuming,
                    // so better storing the reference and provide the viewport on demand
                    const GooString *s = g->getNamedDest();
                    QChar *charArray = new QChar[s->getLength()];
                    for (int i = 0; i < s->getLength(); ++i) charArray[i] = QChar(s->c_str()[i]);
                    QString aux(charArray, s->getLength());
                    e->setAttribute( "DestinationName", aux );
                    delete[] charArray;
                }
                else if ( destination && destination->isOk() )
                {
                    LinkDestinationData ldd(destination, NULL, doc, false);
                    e->setAttribute( "Destination", LinkDestination(ldd).toString() );
                }
                break;
            }
            case actionGoToR:
            {
                // page number is contained/referenced in a LinkGoToR
                const LinkGoToR * g = static_cast< const LinkGoToR * >( a );
                const LinkDest * destination = g->getDest();
                if ( !destination && g->getNamedDest() )
                {
                    // no 'destination' but an internal 'named reference'. we could
                    // get the destination for the page now, but it's VERY time consuming,
                    // so better storing the reference and provide the viewport on demand
                    const GooString *s = g->getNamedDest();
                    QChar *charArray = new QChar[s->getLength()];
                    for (int i = 0; i < s->getLength(); ++i) charArray[i] = QChar(s->c_str()[i]);
                    QString aux(charArray, s->getLength());
                    e->setAttribute( "DestinationName", aux );
                    delete[] charArray;
                }
                else if ( destination && destination->isOk() )
                {
                    LinkDestinationData ldd(destination, NULL, doc, g->getFileName() != 0);
                    e->setAttribute( "Destination", LinkDestination(ldd).toString() );
                }
                e->setAttribute( "ExternalFileName", g->getFileName()->c_str() );
                break;
            }
            case actionURI:
            {
                const LinkURI * u = static_cast< const LinkURI * >( a );
                e->setAttribute( "DestinationURI", u->getURI().c_str() );
            }
            default: ;
        }
    }
    
    DocumentData::~DocumentData()
    {
        qDeleteAll(m_embeddedFiles);
        delete (OptContentModel *)m_optContentModel;
        delete doc;
        delete m_fontInfoIterator;
      }
    
    void DocumentData::init()
    {
        m_fontInfoIterator = 0;
        m_backend = Document::SplashBackend;
        paperColor = Qt::white;
        m_hints = 0;
        m_optContentModel = 0;
    }


    void DocumentData::addTocChildren( QDomDocument * docSyn, QDomNode * parent, const std::vector<::OutlineItem*> * items )
    {

        for ( ::OutlineItem * outlineItem :  *items )
        {
            // iterate over every object in 'items'


            // 1. create element using outlineItem's title as tagName
            QString name;
            const Unicode * uniChar = outlineItem->getTitle();
            int titleLength = outlineItem->getTitleLength();
            name = unicodeToQString(uniChar, titleLength);
            if ( name.isEmpty() )
                continue;

            QDomElement item = docSyn->createElement( name );
            parent->appendChild( item );

            // 2. find the page the link refers to
            const ::LinkAction * a = outlineItem->getAction();
            linkActionToTocItem( a, this, &item );

            item.setAttribute( "Open", QVariant( (bool)outlineItem->isOpen() ).toString() );

            // 3. recursively descend over children
            outlineItem->open();
            const std::vector<::OutlineItem*> * children = outlineItem->getKids();
            if ( children )
                addTocChildren( docSyn, &item, children );
        }
    }

}
