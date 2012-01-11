/* HippoPlot AxisWidgetPlugin implementation
 *
 * Copyright (C) 2003   The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: AxisWidgetPlugin.cxx,v 1.8 2005/11/06 19:41:39 pfkeb Exp $
 */

#include <qglobal.h> // for version
#if QT_VERSION < 0x040000

#include "AxisWidgetPlugin.h"
#include "AxisWidget.h"

namespace hippodraw {

AxisWidgetPlugin::AxisWidgetPlugin()
{
}

QStringList AxisWidgetPlugin::keys() const
{
    QStringList list;
    list << "AxisWidget";
    return list;
}

QWidget* AxisWidgetPlugin::create( const QString &key, QWidget* parent, const char* name )
{
    if ( key == "AxisWidget" )
      return new AxisWidget( parent, name );
    return 0;
}

QString AxisWidgetPlugin::group( const QString& feature ) const
{
    if ( feature == "AxisWidget" )
      return "Input";
    return QString::null;
}

//QIconSet AxisWidgetPlugin::iconSet( const QString& ) const
//{
//   return QIconSet( QPixmap( filechooser_pixmap ) );
//}

QString AxisWidgetPlugin::includeFile( const QString& feature ) const
{
    if ( feature == "AxisWidget" )
	return "AxisWidget.h";
    return QString::null;
}

QString AxisWidgetPlugin::toolTip( const QString& feature ) const
{
    if ( feature == "AxisWidget" )
      return tr("Axis Widget");
    return QString::null;
}

QString AxisWidgetPlugin::whatsThis( const QString& feature ) const
{
    if ( feature == "AxisWidget" )
      return tr("A widget that provides axes text boxes and sliders" 
		" and zoom/pan checkbox.");
    return QString::null;
}

bool AxisWidgetPlugin::isContainer( const QString& ) const
{
  return true;
}
#endif

#if QT_VERSION < 0x040000
Q_EXPORT_PLUGIN( AxisWidgetPlugin )
#else
// Q_EXPORT_PLUGIN( AxisWidgetPlugin )  Need to learn replacement
#endif

#if QT_VERSION < 0x040000
} // namespace hippodra
#endif
