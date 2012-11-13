
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef GLOBE__CHANNELS_TO_SHOW_HPP__INCLUDED
#define GLOBE__CHANNELS_TO_SHOW_HPP__INCLUDED

// Qt include.
#include <QtGui/QWidget>
#include <QtCore/QScopedPointer>


namespace Globe {

//! Shown channels.
enum ShownChannels {
	//! Show only connected channels.
	ShowConnectedOnly = 0,
	//! Show only disconnected channels.
	ShowDisconnectedOnly = 1,
	//! Show all channels.
	ShowAll = 2
}; // enum ShownChannels


class ChannelsToShowPrivate;

//
// ChannelsToShow
//

//! Widget that allow user to select display mode of the channels.
class ChannelsToShow
	:	public QWidget
{

	Q_OBJECT

signals:
	//! "Display all channels" mode selected.
	void displayAllChannels();
	//! "Display connected channels" mode selected.
	void displayConnectedChannels();
	//! "Display disconnected channels" mode selected.
	void displayDisconnectedChannels();

public:
	ChannelsToShow( ShownChannels shownChannels = ShowAll,
		QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~ChannelsToShow();

	//! \return Shown channels mode.
	ShownChannels shownChannelsMode() const;

private slots:
	//! User changed the display mode.
	void displayModeChanged( int index );

private:
	//! Init.
	void init();

private:
	Q_DISABLE_COPY( ChannelsToShow )

	QScopedPointer< ChannelsToShowPrivate > d;
}; // class ChannelsToShow

} /* namespace Globe */

#endif // GLOBE__CHANNELS_TO_SHOW_HPP__INCLUDED
