#pragma once

void init_enemys_in_map(std::vector<Entity*>  &entities, game_indicators &GI, int numPortal) {
	Level lvl = GI.lvl;
	int randEnemy = rand() % 3;
	String numPort = std::to_string(numPortal);
	std::vector<Object> easyE = lvl.GetObjects("easyEnemy" + numPort);
	std::vector<Object> medE = lvl.GetObjects("mediumEnemy" + numPort);
	std::vector<Object> bandE = lvl.GetObjects("bandit" + numPort);
	Vector2f sizeGu = { 0, 0 };
	if (randEnemy == 0)
		for (int i = 0; i < int(easyE.size()); i++) {
			Vector2f coordin = { easyE[i].rect.left, easyE[i].rect.top };
			Vector2f sizeHer = { easyE[i].rect.height, easyE[i].rect.width };
			entities.push_back(new Enemy(GI.pict.easyEnemyImage, GI.pict.gunImage, lvl, coordin, sizeHer, sizeGu, "easyEnemy"));//и закидываем в список всех наших врагов с карты
		}
	if (randEnemy == 1)
		for (int i = 0; i < int(medE.size()); i++) {
			Vector2f coordin = { medE[i].rect.left, medE[i].rect.top };
			Vector2f sizeHer = { medE[i].rect.height, medE[i].rect.width };
			entities.push_back(new Enemy(GI.pict.mediumEnemyImage, GI.pict.gunImage, lvl, coordin, sizeHer, sizeGu, "mediumEnemy"));
		}
	if (randEnemy == 2)
		for (int i = 0; i < int(bandE.size()); i++) {
			Vector2f coordin = { bandE[i].rect.left, bandE[i].rect.top };
			Vector2f sizeHer = { bandE[i].rect.height, bandE[i].rect.width };
			Vector2f sizeGu = { 15, 72 };
			entities.push_back(new Enemy(GI.pict.banditImage, GI.pict.gunImage, lvl, coordin, sizeHer, sizeGu, "bandit"));//и закидываем в список всех наших врагов с карты
		}
	
}

void init_weapons_in_map(std::vector<Entity*>  &entities, game_indicators &GI) {
	Level lvl = GI.lvl;
	std::vector<Object> shotgun = lvl.GetObjects("shotgun");
	std::vector<Object> machineGun = lvl.GetObjects("machineGun");
	std::vector<Object> rifle = lvl.GetObjects("rifle");
	std::vector<Object> automatical = lvl.GetObjects("automatical");
	for (int i = 0; i < int(machineGun.size()); i++) {
		Vector2f coordin = { machineGun[i].rect.left, machineGun[i].rect.top };
		Vector2f sizeHer = { 20, 80 };
		entities.push_back(new Equip(GI.pict.equipImage, coordin, sizeHer, "machineGun"));
	}
	for (int i = 0; i < int(shotgun.size()); i++) {
		Vector2f coordin = { shotgun[i].rect.left, shotgun[i].rect.top };
		Vector2f sizeHer = { 15, 80 };
		entities.push_back(new Equip(GI.pict.equipImage, coordin, sizeHer, "shotgun"));
	}
	for (int i = 0; i < int(automatical.size()); i++) {
		Vector2f coordin = { automatical[i].rect.left, automatical[i].rect.top };
		Vector2f sizeHer = { 20, 80 };
		entities.push_back(new Equip(GI.pict.equipImage, coordin, sizeHer, "automatical"));
	}
	for (int i = 0; i < int(rifle.size()); i++) {
		Vector2f coordin = { rifle[i].rect.left, rifle[i].rect.top };
		Vector2f sizeHer = { 13, 80 };
		entities.push_back(new Equip(GI.pict.equipImage, coordin, sizeHer, "rifle"));
	}
}

void init_exit_in_map(std::vector<Entity*>  &entities, game_indicators &GI) {
	Level lvl = GI.lvl;
	std::vector<Object> exit = lvl.GetObjects("exit");
	std::vector<Object> solidExit = lvl.GetObjects("solidExit");
	for (int i = 0; i < int(exit.size()); i++) {
		Vector2f coordin = { exit[i].rect.left, exit[i].rect.top };
		Vector2f sizeHer = { 147, 138 };
		entities.push_back(new Exit(GI.pict.exitImage, coordin, sizeHer, "exit"));
	}
	for (int i = 0; i < int(solidExit.size()); i++) {
		Vector2f coordin = { solidExit[i].rect.left, solidExit[i].rect.top };
		Vector2f sizeHer = { 147, 138 };
		entities.push_back(new Exit(GI.pict.solidImage, coordin, sizeHer, "solidExit"));
	}
}
void init_portal_in_map(std::vector<Entity*>  &entities, game_indicators &GI) {
	Level lvl = GI.lvl;
	String numPort;
	std::vector<Object> port = lvl.GetObjects("portal");
	for (int i = 0; i < int(port.size()); i++) {
		Vector2f coordin = { port[i].rect.left, port[i].rect.top };
		Vector2f sizeHer = { port[i].rect.height, port[i].rect.width };
		entities.push_back(new Portal(GI.pict.portalImage, coordin, sizeHer, "portal", i));
		entities[entities.size() - 1]->timeBurn = 5;
		init_enemys_in_map(entities, GI, i);
	}
	init_weapons_in_map(entities, GI);
	init_exit_in_map(entities, GI);
}