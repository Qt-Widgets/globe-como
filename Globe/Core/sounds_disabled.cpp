
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012-2017 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Globe include.
#include <Globe/Core/sounds_disabled.hpp>
#include <Globe/Core/sounds_disabled_data.hpp>
#include <Globe/Core/sounds_disabled_cfg.hpp>
#include <Globe/Core/log.hpp>

// Qt include.
#include <QTimer>
#include <QMessageBox>
#include <QCoreApplication>

// QtConfFile include.
#include <QtConfFile/Utils>


namespace Globe {

//
// DisabledSoundsPrivate
//

class DisabledSoundsPrivate {
public:
	DisabledSoundsPrivate()
		:	m_timer( 0 )
	{
	}

	//! Timer.
	QTimer * m_timer;
	//! Map of disabled sounds.
	DisabledSoundsMap m_map;
}; // class DisabledSoundsPrivate


//
// DisabledSounds
//

DisabledSounds::DisabledSounds( QObject * parent )
	:	QObject( parent )
	,	d( new DisabledSoundsPrivate )
{
	init();
}

DisabledSounds::~DisabledSounds()
{
}

static DisabledSounds * disabledSoundsInstancePointer = 0;

void
DisabledSounds::cleanup()
{
	delete disabledSoundsInstancePointer;

	disabledSoundsInstancePointer = 0;
}

DisabledSounds &
DisabledSounds::instance()
{
	if( !disabledSoundsInstancePointer )
	{
		disabledSoundsInstancePointer = new DisabledSounds;

		qAddPostRoutine( &DisabledSounds::cleanup );
	}

	return *disabledSoundsInstancePointer;
}

bool
DisabledSounds::isSoundsEnabled( const Como::Source & source,
	const QString & channelName )
{
	const int index = d->m_map[ channelName ].indexOf(
		DisabledSoundsData( source ) );

	if( index == -1 )
		return true;
	else
		return false;
}

void
DisabledSounds::disableSounds( const Como::Source & source,
	const QString & channelName,
	const QDateTime & to )
{
	const int index = d->m_map[ channelName ].indexOf(
		DisabledSoundsData( source ) );

	if( index == -1 )
	{
		d->m_map[ channelName ].append( DisabledSoundsData( source, to ) );

		emit soundsDisabled( source, channelName, to );
	}
	else
		d->m_map[ channelName ][ index ].dateTime() = to;
}

void
DisabledSounds::enableSounds( const Como::Source & source,
	const QString & channelName )
{
	const int index = d->m_map[ channelName ].indexOf(
		DisabledSoundsData( source ) );

	if( index != -1 )
	{
		d->m_map[ channelName ].removeAt( index );

		emit soundsEnabled( source, channelName );
	}
}

void
DisabledSounds::readCfg( const QString & fileName )
{
	DisabledSoundsCfgTag tag;

	try {
		QtConfFile::readQtConfFile( tag, fileName,
			QTextCodec::codecForName( "UTF-8" ) );

		Log::instance().writeMsgToEventLog( LogLevelInfo, QString(
			"Disabled sounds configuration loaded from file \"%1\"." )
				.arg( fileName ) );
	}
	catch( const QtConfFile::Exception & x )
	{
		Log::instance().writeMsgToEventLog( LogLevelError, QString(
			"Unable to read disabled sounds configuration from file \"%1\".\n"
			"%2" )
				.arg( fileName )
				.arg( x.whatAsQString() ) );

		QMessageBox::critical( 0,
			tr( "Unable to read disabled sounds configuration..." ),
			tr( "Unable to read disabled sounds configuration...\n\n%1" )
				.arg( x.whatAsQString() ) );

		return;
	}

	d->m_map = tag.cfg().map();

	checkAndEnableIf();

	notifyAboutDisabledSounds();
}

void
DisabledSounds::saveCfg( const QString & fileName )
{
	try {
		DisabledSoundsCfg cfg;
		cfg.setMap( d->m_map );

		DisabledSoundsCfgTag tag( cfg );

		QtConfFile::writeQtConfFile( tag, fileName,
			QTextCodec::codecForName( "UTF-8" ) );

		Log::instance().writeMsgToEventLog( LogLevelInfo, QString(
			"Disabled sounds configuration saved to file \"%1\"." )
				.arg( fileName ) );
	}
	catch( const QtConfFile::Exception & x )
	{
		Log::instance().writeMsgToEventLog( LogLevelError, QString(
			"Unable to save disabled sounds configuration to file \"%1\".\n"
			"%2" )
				.arg( fileName )
				.arg( x.whatAsQString() ) );

		QMessageBox::critical( 0,
			tr( "Unable to save disabled sounds configuration..." ),
			tr( "Unable to save disabled sounds configuration...\n\n%1" )
				.arg( x.whatAsQString() ) );
	}
}

void
DisabledSounds::checkAndEnableIf()
{
	const QDateTime now = QDateTime::currentDateTime();

	QMutableMapIterator< QString, QList< DisabledSoundsData > > it( d->m_map );

	while( it.hasNext() )
	{
		it.next();

		QList< DisabledSoundsData > toRemove;

		foreach( const DisabledSoundsData & data, it.value() )
		{
			if( data.dateTime() <= now )
				toRemove.append( data );
		}

		foreach( const DisabledSoundsData & data, toRemove )
		{
			it.value().removeOne( data );

			emit soundsEnabled( data.source(), it.key() );
		}
	}
}

void
DisabledSounds::initTimer()
{
	d->m_timer->start( 60 * 1000 );

	connect( d->m_timer, &QTimer::timeout,
		this, &DisabledSounds::checkAndEnableIf );

	checkAndEnableIf();
}

void
DisabledSounds::init()
{
	d->m_timer = new QTimer( this );

	QTimer::singleShot( ( 60 - QTime::currentTime().second() ) * 1000,
		this, &DisabledSounds::initTimer );
}

void
DisabledSounds::notifyAboutDisabledSounds()
{
	for( DisabledSoundsMap::ConstIterator it = d->m_map.begin(),
		last= d->m_map.end(); it != last; ++it )
	{
		foreach( const DisabledSoundsData & data, it.value() )
			emit soundsDisabled( data.source(), it.key(), data.dateTime() );
	}
}

} /* namespace Globe */
