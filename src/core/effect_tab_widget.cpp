#ifndef SINGLE_SOURCE_COMPILE

/*
 * effect_tab_widget.cpp - tab-widget in channel-track-window for setting up
 *                         effects
 *
 * Copyright (c) 2006-2008 Danny McRae <khjklujn/at/users.sourceforge.net>
 * 
 * This file is part of Linux MultiMedia Studio - http://lmms.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */


#include "effect_tab_widget.h"


#include <QtGui/QMenu>
#include <QtGui/QToolButton>


#include "audio_port.h"
#include "effect_select_dialog.h"
#include "embed.h"
#include "group_box.h"
#include "instrument_track.h"
#include "rack_plugin.h"
#include "rack_view.h"
#include "sample_track.h"
#include "tooltip.h"
#include "automatable_model_templates.h"



effectTabWidget::effectTabWidget( instrumentTrack * _track,
					audioPort * _port ) :
	QWidget( _track->tabWidgetParent() ),
	m_track( dynamic_cast<track *>( _track ) ),
	m_port( _port )
{
	m_port->getEffects()->m_enabledModel.setTrack( m_track );
	setupWidget();
}




effectTabWidget::effectTabWidget( QWidget * _parent, 
					sampleTrack * _track, 
					audioPort * _port ) :
	QWidget( _parent ),
	m_track( dynamic_cast<track *>( _track ) ),
	m_port( _port )
{
	m_port->getEffects()->m_enabledModel.setTrack( m_track );
	setupWidget();
}




effectTabWidget::~effectTabWidget()
{
}




void effectTabWidget::setupWidget( void )
{
	m_effectsGroupBox = new groupBox( tr( "EFFECTS CHAIN" ), this );
	m_effectsGroupBox->setModel( &m_port->getEffects()->m_enabledModel );
	m_effectsGroupBox->setGeometry( 2, 2, 242, 244 );
	
	m_rack = new rackView( m_effectsGroupBox, m_track, m_port );
	m_rack->move( 6, 22 );
		
	m_addButton = new QPushButton( m_effectsGroupBox/*, "Add Effect"*/ );
	m_addButton->setText( tr( "Add" ) );
	m_addButton->move( 75, 210 );
	connect( m_addButton, SIGNAL( clicked( void ) ), 
					this, SLOT( addEffect( void ) ) );
}




void effectTabWidget::saveSettings( QDomDocument & _doc, QDomElement & _this )
{
	_this.setAttribute( "fxenabled",
				m_port->getEffects()->m_enabledModel.value() );
	m_rack->saveState( _doc, _this );

}




void effectTabWidget::loadSettings( const QDomElement & _this )
{
	m_port->getEffects()->m_enabledModel.setValue( 
				_this.attribute( "fxenabled" ).toInt() );
	
	QDomNode node = _this.firstChild();
	while( !node.isNull() )
	{
		if( node.isElement() )
		{
			if( m_rack->nodeName() == node.nodeName() )
			{
				m_rack->restoreState( node.toElement() );
			}
		}
		node = node.nextSibling();
	}
}




void effectTabWidget::addEffect( void )
{
	effectSelectDialog esd( this );
	esd.exec();

	if( esd.result() == QDialog::Rejected )
	{
		return;
	}

	effect * e = esd.instantiateSelectedPlugin();
	m_rack->addEffect( e );
}




#include "effect_tab_widget.moc"

#endif
