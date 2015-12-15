#pragma once

struct init_sounds {
	SoundBuffer machineBuffer;//создаём буфер для звука
	SoundBuffer damageBuffer;//создаём буфер для звука
	SoundBuffer shotgunBuffer;//создаём буфер для звука
	SoundBuffer rifleBuffer;//создаём буфер для звука
	SoundBuffer reloadBuffer;//создаём буфер для звука
	Sound shootMachine;
	Sound damage;
	Sound shootShootGun;
	Sound shootRifle;
	Sound shootReload;

	init_sounds() {
		machineBuffer.loadFromFile("sounds/MachineGun.wav");//загружаем в него звук
		shootMachine.setBuffer(machineBuffer);

		damageBuffer.loadFromFile("sounds/ZombieAttack1.wav");//загружаем в него звук
		damage.setBuffer(damageBuffer);//создаем звук и загружаем в него звук из буфера

		shotgunBuffer.loadFromFile("sounds/PumpShotgun.wav");//загружаем в него звук
		shootShootGun.setBuffer(shotgunBuffer);//создаем звук и загружаем в него звук из буфера

		rifleBuffer.loadFromFile("sounds/Rifle.wav");//загружаем в него звук
		shootRifle.setBuffer(rifleBuffer);//создаем звук и загружаем в него звук из буфера

		reloadBuffer.loadFromFile("sounds/Reload.wav");//загружаем в него звук
		shootReload.setBuffer(reloadBuffer);//создаем звук и загружаем в него звук из буфера	
	}
};
struct init_picture {
	Image heroImage;
	Image gunImage;
	Image easyEnemyImage;
	Image bulletImage;
	Image mediumEnemyImage;
	Image equipImage;
	Image banditImage;
	Image meetImage;
	Image bonusImage;
	Image exitImage;
	Image solidImage;
	init_picture() {
		bulletImage.loadFromFile("images/shot2.png");
		heroImage.loadFromFile("images/players_anim.png");
		gunImage.loadFromFile("images/weapons.png");
		equipImage.loadFromFile("images/equipment2.png");
		easyEnemyImage.loadFromFile("images/easy_enemy.png");
		banditImage.loadFromFile("images/bandit.png");
		mediumEnemyImage.loadFromFile("images/MediumEnemy.png");
		meetImage.loadFromFile("images/meet.png");
		bonusImage.loadFromFile("images/bonus.png");
		exitImage.loadFromFile("images/exitImage.png");
		solidImage.loadFromFile("images/solidImage.png");
	}
};