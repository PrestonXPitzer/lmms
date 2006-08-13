#ifndef SINGLE_SOURCE_COMPILE

/*
 * sample_play_handle.cpp - implementation of class samplePlayHandle
 *
 * Copyright (c) 2005-2006 Tobias Doerffel <tobydox/at/users.sourceforge.net>
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */


#include "sample_play_handle.h"
#include "sample_buffer.h"
#include "sample_track.h"
#include "buffer_allocator.h"
#include "audio_port.h"



samplePlayHandle::samplePlayHandle( const QString & _sample_file,
							engine * _engine ) :
	playHandle( SAMPLE_PLAY_HANDLE ),
	engineObject( _engine ),
	m_sampleBuffer( new sampleBuffer( eng(), _sample_file ) ),
	m_ownSampleBuffer( TRUE ),
	m_doneMayReturnTrue( TRUE ),
	m_frame( 0 ),
	m_audioPort( new audioPort( "samplePlayHandle", eng() ) ),
	m_ownAudioPort( TRUE ),
	m_volume( 1.0f ),
	m_track( NULL )
{
}




samplePlayHandle::samplePlayHandle( sampleBuffer * _sample_buffer ) :
	playHandle( SAMPLE_PLAY_HANDLE ),
	engineObject( _sample_buffer->eng() ),
	m_sampleBuffer( _sample_buffer ),
	m_ownSampleBuffer( FALSE ),
	m_doneMayReturnTrue( TRUE ),
	m_frame( 0 ),
	m_audioPort( new audioPort( "samplePlayHandle", eng() ) ),
	m_ownAudioPort( TRUE ),
	m_volume( 1.0f ),
	m_track( NULL )
{
}




samplePlayHandle::samplePlayHandle( sampleTCO * _tco ) :
	playHandle( SAMPLE_PLAY_HANDLE ),
	engineObject( _tco->eng() ),
	m_sampleBuffer( _tco->getSampleBuffer() ),
	m_ownSampleBuffer( FALSE ),
	m_doneMayReturnTrue( TRUE ),
	m_frame( 0 ),
	m_audioPort( ( (sampleTrack *)_tco->getTrack() )->getAudioPort() ),
	m_ownAudioPort( FALSE ),
	m_volume( 1.0f ),
	m_track( _tco->getTrack() )
{
}




samplePlayHandle::~samplePlayHandle()
{
	if( m_ownSampleBuffer == TRUE )
	{
		delete m_sampleBuffer;
	}
	if( m_ownAudioPort )
	{
		delete m_audioPort;
	}
}




void samplePlayHandle::play( void )
{
	play( 0 );
}




void samplePlayHandle::play( const fpab_t _frame_base )
{
	if( framesDone() >= totalFrames() )
	{
		return;
	}

	const fpab_t frames = eng()->getMixer()->framesPerAudioBuffer()
								- _frame_base;
	if( !( m_track && m_track->muted() ) )
	{
		sampleFrame * buf = bufferAllocator::alloc<sampleFrame>(
								frames );
		volumeVector v = { { m_volume, m_volume
#ifndef DISABLE_SURROUND
						, m_volume, m_volume
#endif
				} } ;
		m_sampleBuffer->play( buf, m_frame, frames );
		eng()->getMixer()->bufferToPort( buf, frames, _frame_base, v,
								m_audioPort );

		bufferAllocator::free( buf );
	}

	m_frame += frames;
}




bool samplePlayHandle::done( void ) const
{
	return( framesDone() >= totalFrames() && m_doneMayReturnTrue == TRUE );
}




f_cnt_t samplePlayHandle::totalFrames( void ) const
{
	return( m_sampleBuffer->endFrame() - m_sampleBuffer->startFrame() );
}




void samplePlayHandle::setVolume( float _new_volume )
{
	if( _new_volume <= MAX_VOLUME )
	{
		m_volume = _new_volume / 100.0f;
	}
}




#include "sample_play_handle.moc"


#endif
