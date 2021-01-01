#include "cstrike.h"

#include "hooked/hooked.h"

DWORD WINAPI cstrike::setup( void* parameter ) {

	const auto handle = static_cast< HMODULE >( parameter );

	m_console.setup( "cstrike-hack" );

	if ( !m_interfaces.setup( ) || !m_netvars.setup( ) || !m_hooked.setup( ) ) {

		FreeLibraryAndExitThread( handle, EXIT_FAILURE );

		return EXIT_FAILURE;

	}

	while ( !GetAsyncKeyState( VK_DELETE ) )
		m_utils.sleep( 100 );

	FreeLibraryAndExitThread( handle, EXIT_SUCCESS );

	return EXIT_SUCCESS;

}

void cstrike::unload( ) {

	m_hooked.unload( );

	m_console.unload( );

	Beep( 400, 400 );

}

void cstrike::iterate_players( std::function< void( cs_player* ) > function, iterate_player_flags flags ) {

	for ( int i = 1; i <= m_interfaces.m_globals->m_max_clients; i++ ) {

		cs_player* player = m_interfaces.m_entity_list->get< cs_player* >( i );
		if ( !player )
			continue;

		if ( !( flags & iterate_dead ) )
			if ( !player->is_alive( ) )
				continue;

		if ( !( flags & iterate_dormant ) )
			if ( player->get_client_networkable( )->is_dormant( ) )
				continue;

		if ( !( flags & iterate_teammates ) )
			if ( player->get_team( ) == m_local_player->get_team( ) )
				continue;

		function( player );

	}

}