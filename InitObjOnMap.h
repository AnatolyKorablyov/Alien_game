#pragma once

void init_objects_in_map(Level lvl, std::vector<Entity*>  &entities, init_picture pict) {
	std::vector<Object> easyE = lvl.GetObjects("easyEnemy");
	std::vector<Object> medE = lvl.GetObjects("mediumEnemy");
	std::vector<Object> bandE = lvl.GetObjects("bandit");
	std::vector<Object> shotgun = lvl.GetObjects("shotgun");
	std::vector<Object> machineGun = lvl.GetObjects("machineGun");
	std::vector<Object> rifle = lvl.GetObjects("rifle");
	std::vector<Object> automatical = lvl.GetObjects("automatical");
	std::vector<Object> exit = lvl.GetObjects("exit");
	std::vector<Object> solidExit = lvl.GetObjects("solidExit");
	Vector2f sizeGu = { 0, 0 };
	for (int i = 0; i < int(easyE.size()); i++) {
		Vector2f coordin = { easyE[i].rect.left, easyE[i].rect.top };
		Vector2f sizeHer = { easyE[i].rect.height, easyE[i].rect.width };
		entities.push_back(new Enemy(pict.easyEnemyImage, pict.gunImage, lvl, coordin, sizeHer, sizeGu, "easyEnemy"));//и закидываем в список всех наших врагов с карты
	}
	for (int i = 0; i < int(medE.size()); i++) {
		Vector2f coordin = { medE[i].rect.left, medE[i].rect.top };
		Vector2f sizeHer = { medE[i].rect.height, medE[i].rect.width };
		entities.push_back(new Enemy(pict.mediumEnemyImage, pict.gunImage, lvl, coordin, sizeHer, sizeGu, "mediumEnemy"));
	}
	for (int i = 0; i < int(bandE.size()); i++) {
		Vector2f coordin = { bandE[i].rect.left, bandE[i].rect.top };
		Vector2f sizeHer = { bandE[i].rect.height, bandE[i].rect.width };
		Vector2f sizeGu = { 15, 72 };
		entities.push_back(new Enemy(pict.banditImage, pict.gunImage, lvl, coordin, sizeHer, sizeGu, "bandit"));//и закидываем в список всех наших врагов с карты
	}
	for (int i = 0; i < int(machineGun.size()); i++) {
		Vector2f coordin = { machineGun[i].rect.left, machineGun[i].rect.top };
		Vector2f sizeHer = { 20, 80 };
		entities.push_back(new Equip(pict.equipImage, coordin, sizeHer, "machineGun"));
	}
	for (int i = 0; i < int(shotgun.size()); i++) {
		Vector2f coordin = { shotgun[i].rect.left, shotgun[i].rect.top };
		Vector2f sizeHer = { 15, 80 };
		entities.push_back(new Equip(pict.equipImage, coordin, sizeHer, "shotgun"));
	}
	for (int i = 0; i < int(automatical.size()); i++) {
		Vector2f coordin = { automatical[i].rect.left, automatical[i].rect.top };
		Vector2f sizeHer = { 20, 80 };
		entities.push_back(new Equip(pict.equipImage, coordin, sizeHer, "automatical"));
	}
	for (int i = 0; i < int(rifle.size()); i++) {
		Vector2f coordin = { rifle[i].rect.left, rifle[i].rect.top };
		Vector2f sizeHer = { 13, 80 };
		entities.push_back(new Equip(pict.equipImage, coordin, sizeHer, "rifle"));
	}
	for (int i = 0; i < int(exit.size()); i++) {
		Vector2f coordin = { exit[i].rect.left, exit[i].rect.top };
		Vector2f sizeHer = { 147, 138 };
		entities.push_back(new Exit(pict.exitImage, coordin, sizeHer, "exit"));
	}
	for (int i = 0; i < int(solidExit.size()); i++) {
		Vector2f coordin = { solidExit[i].rect.left, solidExit[i].rect.top };
		Vector2f sizeHer = { 147, 138 };
		entities.push_back(new Exit(pict.solidImage, coordin, sizeHer, "solidExit"));
	}
}