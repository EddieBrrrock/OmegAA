/*
===========================================================================
Copyright (C) 2006 Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com))

This file is part of Open Arena and is based upon Mr. Elusive's fuzzy logic
system found in Quake 3 Arena.

Open Arena is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Open Arena is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

chat "alpha"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"I'm gonna whip your ass.";
		"You are going to die.";
	}

	type "game_exit"
	{
		"Bye!";
		"FUCK LITTLEBIGPLANET!";
	}

	type "level_start"
	{
		"I hate ", 4, ".";
	}

	type "level_end"
	{
		"It was fun.";
	}

	type "level_end_victory"
	{
		"I'm happier that you lost than that I won :)";
	}

	type "level_end_lose"
	{
		"Next time i play with the right hand.";
	}

	type "hit_talking"
	{
		"So that's how you get kills " 0, " huh?";
	}

	type "hit_nodeath"
	{
		"No pain.";
	}

	type "hit_nokill"
	{
		"You better run.";
	}

	type "death_telefrag"
	{
		DEATH_TELEFRAGGED0;
		DEATH_TELEFRAGGED0;
		DEATH_TELEFRAGGED0;
	}

	type "death_cratered"
	{
		DEATH_FALLING0;
		DEATH_FALLING0;
		DEATH_FALLING0;
	}

	type "death_lava"
	{
		"...";
	}

	type "death_slime"
	{
		"...";
	}

	type "death_drown"
	{
		"...";
	}

	type "death_suicide"
	{
		"Gave you a small head start.";
	}

	type "death_gauntlet"
	{
		"You use this because you don't know how to aim?";
		"Everyone look at " 0, ", he is so funny!";
	}

	type "death_rail"
	{
		"Did you know that railgun is the easiest weapon to use?";
		DEATH_RAIL1;
		DEATH_RAIL0;
	}

	type "death_bfg"
	{
		"Aim for " 0, ", he got the big fucking gun.;
		"Bfg is not as big as ~your mom."; 
	}

	type "death_insult"
	{
		"Fuck you!";
		"Nice.";
	}

	type "death_praise"
	{
		0, " is good at OA but is probably smelly.";
	}

	type "kill_rail"
	{
		"I love the sound it does when you explode";
		"Where are you?";
	}

	type "kill_gauntlet"
	{
		"VRZZZZZ";		
	}

	type "kill_telefrag"
	{
		"You take too much space, go for a jog with ~your mom.";
		"This wouldn't have happened if you weren't camping.";
	}

	type "kill_suicide"
	{
		TAUNT0;
		TAUNT1;
	}

	type "kill_insult"
	{
		"Pathetic.";
		"Mtss";
		"My ", family_member, " could do better.";
	}

	type "kill_praise"
	{
		"Nice one, ", 0, ".";
		"Wow, ~you never saw that one coming, ", 0, ".";
	}

	type "random_insult"
	{
		 0, ", ~you suck!";
		 0, " needs to stop calling my mom.";
	}

	type "random_misc"
	{
		0, " gave my ~mom ", disease, ".";
		"Is it true that FatSakura is heavier than a tank?";
		
	}
}
