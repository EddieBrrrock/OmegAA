/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"


#define	SCOREBOARD_X		(0)

#define SB_HEADER			86
#define SB_TOP				(SB_HEADER+32)

// Where the status bar starts, so we don't overwrite it
#define SB_STATUSBAR		420

#define SB_NORMAL_HEIGHT	40
#define SB_INTER_HEIGHT		26 // interleaved height

#define SB_MAXCLIENTS_NORMAL  ((SB_STATUSBAR - SB_TOP) / SB_NORMAL_HEIGHT)
#define SB_MAXCLIENTS_INTER   ((SB_STATUSBAR - SB_TOP) / SB_INTER_HEIGHT - 1)

// Used when interleaved



#define SB_LEFT_BOTICON_X	(SCOREBOARD_X-42)
#define SB_LEFT_HEAD_X		(SCOREBOARD_X-10)
#define SB_RIGHT_BOTICON_X	(SCOREBOARD_X-22)
#define SB_RIGHT_HEAD_X		(SCOREBOARD_X-52)
// Normal
#define SB_BOTICON_X		(SCOREBOARD_X-10)
#define SB_HEAD_X			(SCOREBOARD_X+12)

#define SB_SCORELINE_X		28

#define SB_RATING_WIDTH	    (6 * BIGCHAR_WIDTH) // width 6
#define SB_SCORE_X			(SB_SCORELINE_X + 7 * BIGCHAR_WIDTH) // width 5
#define SB_TIME_X			(SB_SCORELINE_X + 12 * BIGCHAR_WIDTH) // width 5
#define SB_NAME_X			(SB_SCORELINE_X + 17 * BIGCHAR_WIDTH) // width 12
#define SB_ACCURACY_X			(SB_SCORELINE_X + 29 * BIGCHAR_WIDTH) // width 4
#define SB_RATIO_X			(SB_SCORELINE_X + 33 * BIGCHAR_WIDTH) // width 4
#define SB_PING_X			(SB_SCORELINE_X + 37 * BIGCHAR_WIDTH) // width 4

// The new and improved score board
//
// In cases where the number of clients is high, the score board heads are interleaved
// here's the layout

//
//	0   32   80  112  144   240  320  400   <-- pixel position
//  bot head bot head score ping time name
//  
//  wins/losses are drawn on bot icon now

static qboolean localClient; // true if local client has been displayed


							 /*
=================
CG_DrawScoreboard
=================
*/
static void CG_DrawClientScore( int y, score_t *score, float *color, float fade, qboolean largeFormat ) {
	const char	*s;
	const char	*info;
	char	string[1024];
	vec3_t	headAngles;
	vec3_t	angles;
	vec3_t	origin;
	centity_t	*cent;
	clientInfo_t	*ci;
	int iconx, headx;
	int x;

	if ( score->client < 0 || score->client >= cgs.maxclients ) {
		Com_Printf( "Bad score->client: %i\n", score->client );
		return;
	}
	
	cent = &cg_entities[score->client];
	ci = &cgs.clientinfo[score->client];

	iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);
	headx = SB_HEAD_X + (SB_RATING_WIDTH / 2);

	// draw the handicap or bot skill marker (unless player has flag)
	if ( ci->powerups & ( 1 << PW_NEUTRALFLAG ) ) {
		CG_DrawFlagModel( iconx-3, y, 26, 26, TEAM_FREE, qfalse );
	} else if ( ci->powerups & ( 1 << PW_REDFLAG ) ) {
		CG_DrawFlagModel( iconx-3, y, 26, 26, TEAM_RED, qfalse );
	} else if ( ci->powerups & ( 1 << PW_BLUEFLAG ) ) {
		CG_DrawFlagModel( iconx-3, y, 26, 26, TEAM_BLUE, qfalse );
	} else {
		if ( ci->team != TEAM_SPECTATOR && cent->currentState.eFlags & EF_TALK ) {
				CG_DrawPic( iconx, y+3, 21, 21, cgs.media.balloonShader );
		} else if ( ci->botSkill > 0 && ci->botSkill <= 5 ) {
			if ( cg_drawIcons.integer ) {
				CG_DrawPic( iconx, y+3, 21, 21, cgs.media.botSkillShaders[ ci->botSkill - 1 ] );
			}
		} else if ( cg.warmup < 0 && ci->team != TEAM_SPECTATOR && cgs.startWhenReady ) {
			if ( cg.readyMask & ( 1 << score->client ) ) {
				CG_DrawSmallStringColor(iconx - BIGCHAR_WIDTH - 5, y+6, "READY", colorGreen);
			}
		} else if ( ci->handicap < 100 ) {
			Com_sprintf( string, sizeof( string ), "%i", ci->handicap );
			CG_DrawSmallStringColor( iconx+3, y+6, string, color );
		}

		// draw the wins / losses
		if ( cgs.gametype == GT_TOURNAMENT && !cg.warmup ) {
			Com_sprintf( string, sizeof( string ), "%i/%i", ci->wins, ci->losses );
			if( ci->handicap < 100 || ci->botSkill ) {
				CG_DrawSmallStringColor( iconx-28, y+6, string, color );
			}
			else {
				CG_DrawSmallStringColor( iconx-2, y+6, string, color );
			}
		}
	}

	// draw the face
	if ( cent->currentState.eFlags & EF_DEAD ) {
		CG_DrawPic( headx+2, y+2, BIGCHAR_HEIGHT+7, BIGCHAR_HEIGHT+7, cgs.media.skullShader );
	} else {
		VectorClear( headAngles );
		headAngles[YAW] = 180 + 60 * sin( cg.time / 2000.0 );
		CG_DrawHead( headx, y, BIGCHAR_HEIGHT+11, BIGCHAR_HEIGHT+11, score->client, headAngles );
	}

	if ( cg.snap->ps.stats[STAT_TEAM_LOCKED] & LOCKED ) {
		CG_DrawTinyString( 260, 92, "Teams are locked", 0.5 );
	}

	x = 5;

	// draw the server name
	info = CG_ConfigString( CS_SERVERINFO );
	s = Info_ValueForKey( info, "sv_hostname" );
	CG_DrawStringExt( x, 470, s, color, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );

	x += CG_DrawStrlen( s ) * TINYCHAR_WIDTH/1.33;

	// draw the slashs
	CG_DrawStringExt( x+2, 470, "/", colorCyan, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );
	CG_DrawStringExt( x+4, 470, "/", colorTtCyan, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );

	// draw gametype
	if ( cgs.gametype == GT_FFA ) {
		s = "Free For All";
	} else if ( cgs.gametype == GT_TOURNAMENT ) {
		s = "Tournament";
	} else if ( cgs.gametype == GT_TEAM ) {
		s = "Team Deathmatch";
	} else if ( cgs.gametype == GT_CTF ) {
		s = "Capture the Flag";
	} else if ( cgs.gametype == GT_ELIMINATION ) {
		s = "Elimination";
	} else if ( cgs.gametype == GT_CTF_ELIMINATION ) {
		s = "CTF Elimination";
	} else if ( cgs.gametype == GT_LMS ) {
		s = "Last Man Standing";
	} else if ( cgs.gametype == GT_DOUBLE_D ) {
		s = "Double Domination";
	} else if ( cgs.gametype == GT_1FCTF ) {
		s = "One Flag CTF";
	} else if ( cgs.gametype == GT_OBELISK ) {
		s = "Overload";
	} else if ( cgs.gametype == GT_HARVESTER ) {
		s = "Harvester";
	} else if ( cgs.gametype == GT_DOMINATION ) {
		s = "Domination";
	} else {
		s = "";
	}
	CG_DrawStringExt( x+12, 470, s, color, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );

	x += CG_DrawStrlen( s ) * TINYCHAR_WIDTH/1.33;

	// draw the slashs
	CG_DrawStringExt( x+15, 470, "/", colorGreen, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );
	CG_DrawStringExt( x+17, 470, "/", colorTtGreen, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );

	// draw the map name
	info = CG_ConfigString( CS_SERVERINFO );
	CG_DrawStringExt( x+25, 470, Info_ValueForKey( info, "mapname" ), color, qfalse, qfalse, TINYCHAR_WIDTH/1.25, TINYCHAR_HEIGHT/1.25, 0 );

	// draw omega logo
	VectorClear( angles );
	origin[0] = 90;
	origin[1] = 0;
	origin[2] = -10;
	angles[YAW] = cg.time * 360 / 2048.0;
	CG_Draw3DModel( 592, 432, ICON_SIZE, ICON_SIZE, cgs.media.omegaModel, 0, origin, angles );

#ifdef MISSIONPACK
	// draw the team task
	if ( ci->teamTask != TEAMTASK_NONE ) {
                if (ci->isDead) {
                    CG_DrawPic( headx + 48, y, 16, 16, cgs.media.deathShader );
                }
                else if ( ci->teamTask == TEAMTASK_OFFENSE ) {
			CG_DrawPic( headx + 48, y, 16, 16, cgs.media.assaultShader );
		}
		else if ( ci->teamTask == TEAMTASK_DEFENSE ) {
			CG_DrawPic( headx + 48, y, 16, 16, cgs.media.defendShader );
		}
	}
#endif

	// highlight your position
	if ( score->client == cg.snap->ps.clientNum ) {
		float	hcolor[4];
		int		rank;

		localClient = qtrue;

		if ( ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR ) ||
			( ( cgs.gametype >= GT_TEAM ) &&
			( cgs.ffa_gt != 1 ) ) ) {
			// Sago: I think this means that it doesn't matter if two players are tied in team game - only team score counts
			rank = -1;
		} else {
			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
		}
		if ( rank == 0 ) {
			hcolor[0] = 0;
			hcolor[1] = 0;
			hcolor[2] = 0.7f;
		} else if ( rank == 1 ) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0;
			hcolor[2] = 0;
		} else if ( rank == 2 ) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0;
		} else {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0.7f;
		}

		hcolor[3] = fade * 0.5;
		CG_FillRect( SB_SCORELINE_X + BIGCHAR_WIDTH + (SB_RATING_WIDTH / 2), y, 
			640 - SB_SCORELINE_X - BIGCHAR_WIDTH, BIGCHAR_HEIGHT+11, hcolor );
	}

	// draw score
	if ( score->ping == -1 ) {
		Com_sprintf(string, sizeof(string),
			"connecting");
	} else if ( ci->team == TEAM_SPECTATOR ) {
		Com_sprintf(string, sizeof(string),
			"SPEC");
	} else {
		Com_sprintf(string, sizeof(string),
			"%4i", score->score);
	}
	CG_DrawSmallString( SB_SCORE_X - 44, y+6, string, fade );

	// draw time
	if ( score->ping != -1 ) {
		Com_sprintf(string, sizeof(string),
			"%3i:%02i", score->time / 60, score->time - ( score->time / 60 ) * 60);
		CG_DrawStringExt( SB_TIME_X - 54, y+6, string, colorLtGrey, qtrue, qfalse, SMALLCHAR_WIDTH/1.25, SMALLCHAR_HEIGHT, 0 );
	}

	// draw name
	Com_sprintf(string, sizeof(string),
		"%s", ci->name);
	CG_DrawMediumString( SB_NAME_X - 49, y+6, string, fade );

	// draw accuracy
	if ( score->ping != -1 ) {
		Com_sprintf(string, sizeof(string),
			"%3i%%", score->accuracy);
		CG_DrawStringExt( SB_ACCURACY_X - 48, y+6, string, colorLtGrey, qtrue, qfalse, SMALLCHAR_WIDTH/1.25, SMALLCHAR_HEIGHT, 0 );
	}

	// draw ratio
	if ( !( score->ping == -1 || ci->team == TEAM_SPECTATOR ) ) {
		Com_sprintf(string, sizeof(string),
			"%3i/%-3i", score->kills, score->deaths);
		CG_DrawSmallString( SB_RATIO_X - 64, y+6, string, fade );
	}

	// draw ping
	if ( !( score->ping == -1 || ci->botSkill ) ) {
		Com_sprintf(string, sizeof(string),
			"%3i", score->ping);
		if ( score->ping >= 300 ) {
			CG_DrawSmallStringColor( SB_PING_X - 44, y+6, string, colorRed );
		} else if ( score->ping >= 200 ) {
			CG_DrawSmallStringColor( SB_PING_X - 44, y+6, string, colorOrange );
		} else if ( score->ping >= 100 ) {
			CG_DrawSmallStringColor( SB_PING_X - 44, y+6, string, colorYellow );
		} else {
			CG_DrawSmallStringColor( SB_PING_X - 44, y+6, string, colorGreen );
		}
	}

	// add the "ready" marker for intermission exiting
	if ( cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << score->client ) ) {
		CG_DrawSmallStringColor(iconx - BIGCHAR_WIDTH - 5, y+6, "READY", colorGreen);
	} else
        if(cgs.gametype == GT_LMS) {
            CG_DrawSmallStringColor( iconx-30, y+6, va("*%i*",ci->isDead), color );
        } else
        if(ci->isDead) {
            CG_DrawSmallStringColor( iconx-35, y+6, "DEAD", color );
        }
}

/*
=================
CG_TeamScoreboard
=================
*/
static int CG_TeamScoreboard( int y, team_t team, float fade, int maxClients, int lineHeight ) {
	int		i;
	score_t	*score;
	float	color[4];
	int		count;
	clientInfo_t	*ci;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = fade;

	count = 0;
	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ ) {
		score = &cg.scores[i];
		ci = &cgs.clientinfo[ score->client ];

		if ( team != ci->team ) {
			continue;
		}

		CG_DrawClientScore( y + lineHeight * count, score, color, fade, lineHeight == SB_NORMAL_HEIGHT );

		count++;
	}

	return count;
}

/*
=================
CG_DrawScoreboard

Draw the normal in-game scoreboard
=================
*/
qboolean CG_DrawOldScoreboard( void ) {
	int		x, x1, x2, y, w, w1, i, n1, n2;
	float	fade = 0.0;
	float	*fadeColor;
	float	color[4];
	char	*s;
	char	*s1;
	char	*s2;
	int maxClients;
	int lineHeight;
	int topBorderSize, bottomBorderSize;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = fade;

	// don't draw amuthing if the menu or console is up
	if ( cg_paused.integer ) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	if ( cgs.gametype == GT_SINGLE_PLAYER && cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	// don't draw scoreboard during death while warmup up
	if ( cg.warmup && !cg.showScores ) {
		return qfalse;
	}

	if ( cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD ||
		 cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		fade = 1.0;
		fadeColor = colorWhite;
	} else {
		fadeColor = CG_FadeColor( cg.scoreFadeTime, FADE_TIME );
		
		// next time scoreboard comes up, don't print killer
		cg.deferredPlayerLoading = 0;
		cg.killerName[0] = 0;
		return qfalse;

		fade = *fadeColor;
	}

	if ( cg.scoresRequestTime + 1000 < cg.time ) {
		// the scores are more than one second out of data,
		// so request new ones
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand( "score" );
	}

	// fragged by ... line
	if ( cg.killerName[0] ) {
		s = va("Fragged by %s", cg.killerName );
		w = CG_DrawStrlen( s ) * MEDIUMCHAR_WIDTH;
		x = ( SCREEN_WIDTH - w ) / 2;
		y = 28;
		CG_DrawMediumString( x, y, s, fade );
	}

	// current rank
	if ( cgs.gametype < GT_TEAM || cgs.ffa_gt == 1) {
		if (cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR ) {
			s = va("%s place with %i",
				CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),
				cg.snap->ps.persistant[PERS_SCORE] );
			w = CG_DrawStrlen( s ) * MEDIUMCHAR_WIDTH;
			x = ( SCREEN_WIDTH - w ) / 2;
			y = 60;
			CG_DrawMediumString( x, y+5, s, fade );
		}
	} else {
		x = ( SCREEN_WIDTH - 2 * MEDIUMCHAR_WIDTH ) / 2;
		CG_DrawMediumString( x, 67, "to", fade );

		s1 = va("%i", cg.teamScores[0] );
		w1 = CG_DrawStrlen( s1 ) * GIANTCHAR_WIDTH * 2;
		x1 = ( SCREEN_WIDTH - w1 ) / 2 - 21;
		CG_DrawStringExt( x1, 47, s1, colorRed, qtrue, qtrue, GIANTCHAR_WIDTH, GIANTCHAR_HEIGHT, 0 );

		s2 = va("%i", cg.teamScores[1] );
		x2 = SCREEN_WIDTH / 2 + 22;
		CG_DrawStringExt( x2, 47, s2, colorBlue, qtrue, qtrue, GIANTCHAR_WIDTH, GIANTCHAR_HEIGHT, 0 );
	}

	// scoreboard
	y = SB_HEADER;

	color[0] = 0.0;
	color[1] = 0.5;
	color[2] = 1.0;
	color[3] = 0.5;
	CG_DrawSmallStringColor( SB_SCORE_X - (SB_RATING_WIDTH / 2), y + 15, "Score", color );
	CG_DrawSmallStringColor( SB_PING_X - (SB_RATING_WIDTH / 2), y + 15, "Ping", color );
	CG_DrawSmallStringColor( SB_TIME_X - (SB_RATING_WIDTH / 2), y + 15, "Time", color );
	CG_DrawSmallStringColor( SB_NAME_X - (SB_RATING_WIDTH / 2), y + 15, "Name", color );
	CG_DrawSmallStringColor( SB_ACCURACY_X - (SB_RATING_WIDTH / 2), y + 15, "Acc", color );
	CG_DrawSmallStringColor( SB_RATIO_X - (SB_RATING_WIDTH / 2), y + 15, "K/D", color );

	y = SB_TOP;

	maxClients = SB_MAXCLIENTS_INTER;
	lineHeight = SB_INTER_HEIGHT;
	topBorderSize = 8;
	bottomBorderSize = 16;

	localClient = qfalse;

	if ( cgs.gametype >= GT_TEAM && cgs.ffa_gt!=1) {
		//
		// teamplay scoreboard
		//
		y += lineHeight/2;

		if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			n1 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n1 * lineHeight + bottomBorderSize, 0.33f, TEAM_RED );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n2 * lineHeight + bottomBorderSize, 0.33f, TEAM_BLUE );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		} else {
			n1 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n1 * lineHeight + bottomBorderSize, 0.33f, TEAM_BLUE );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n2 * lineHeight + bottomBorderSize, 0.33f, TEAM_RED );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		}
		n1 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;

	} else {
		//
		// free for all scoreboard
		//
		n1 = CG_TeamScoreboard( y, TEAM_FREE, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
		n2 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients - n1, lineHeight );
		y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
	}

	if (!localClient) {
		// draw local client at the bottom
		for ( i = 0 ; i < cg.numScores ; i++ ) {
			if ( cg.scores[i].client == cg.snap->ps.clientNum ) {
				CG_DrawClientScore( y, &cg.scores[i], fadeColor, fade, lineHeight == SB_NORMAL_HEIGHT );
				break;
			}
		}
	}

	// load any models that have been deferred
	if ( ++cg.deferredPlayerLoading > 10 ) {
		CG_LoadDeferredPlayers();
	}

	return qtrue;
}

//================================================================================

/*
================
CG_CenterGiantLine
================
*/
static void CG_CenterGiantLine( float y, const char *string ) {
	float		x;
	vec4_t		color;

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	x = 0.5 * ( 640 - GIANT_WIDTH * CG_DrawStrlen( string ) );

	CG_DrawStringExt( x, y, string, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
}

/*
=================
CG_DrawTourneyScoreboard

Draw the oversize scoreboard for tournements
=================
*/
void CG_DrawOldTourneyScoreboard( void ) {
	const char		*s;
	vec4_t			color;
	int				min, tens, ones;
	clientInfo_t	*ci;
	int				y;
	int				i;

	// request more scores regularly
	if ( cg.scoresRequestTime + 2000 < cg.time ) {
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand( "score" );
	}

	// draw the dialog background
	color[0] = color[1] = color[2] = 0;
	color[3] = 1;
	CG_FillRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color );

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	// print the mesage of the day
	s = CG_ConfigString( CS_MOTD );
	if ( !s[0] ) {
		s = "Scoreboard";
	}

	// print optional title
	CG_CenterGiantLine( 8, s );

	// print server time
	ones = cg.time / 1000;
	min = ones / 60;
	ones %= 60;
	tens = ones / 10;
	ones %= 10;
	s = va("%i:%i%i", min, tens, ones );

	CG_CenterGiantLine( 64, s );


	// print the two scores

	y = 160;
	if ( cgs.gametype >= GT_TEAM && cgs.ffa_gt!=1) {
		//
		// teamplay scoreboard
		//
		CG_DrawStringExt( 8, y, "Red Team", color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		s = va("%i", cg.teamScores[0] );
		CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		
		y += 64;

		CG_DrawStringExt( 8, y, "Blue Team", color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		s = va("%i", cg.teamScores[1] );
		CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
	} else {
		//
		// free for all scoreboard
		//
		for ( i = 0 ; i < MAX_CLIENTS ; i++ ) {
			ci = &cgs.clientinfo[i];
			if ( !ci->infoValid ) {
				continue;
			}
			if ( ci->team != TEAM_FREE ) {
				continue;
			}

			CG_DrawStringExt( 8, y, ci->name, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
			s = va("%i", ci->score );
			CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
			y += 64;
		}
	}


}

