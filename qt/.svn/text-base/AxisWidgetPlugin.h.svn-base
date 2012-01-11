/* -*- mode: c++ -*-
 *
 * HippoPlot AxisWidgetPlugin class interface
 *
 * Copyright (C) 2003   The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: AxisWidgetPlugin.h,v 1.4 2005/10/30 00:10:15 pfkeb Exp $
 *
 */

#include <qglobal.h> // for version
#if QT_VERSION < 0x040000
#include <qwidgetplugin.h>

namespace hippodraw {

/** A plugin that allows AxisWidget to appear in QT Designer.

    @author Sanket Biren Malde <sanket@stanford.edu>
*/

class AxisWidgetPlugin : public QWidgetPlugin
{
public:
    AxisWidgetPlugin();

    QStringList keys() const;
    QWidget* create( const QString &classname, QWidget* parent = 0, const char* name = 0 );
    QString group( const QString& ) const;
    //QIconSet iconSet( const QString& ) const;
    QString includeFile( const QString& ) const;
    QString toolTip( const QString& ) const;
    QString whatsThis( const QString& ) const;
    bool isContainer( const QString& ) const;
};

} // namespace hippodraw

#endif
