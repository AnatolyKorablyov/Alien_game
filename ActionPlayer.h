#pragma once
#include "Classes.h"
#include "LoadFiles.h"

void damage_player_to_enemys(Entity &one, Entity &two);
void pick_up_weapon(Entity &one, Player &p);
void pick_up_bonuses_and_exit(Entity &one, Player &p);
void player_shotting(Event event, std::vector<Entity*>  &entities, Player &p, game_indicators &GI);