#pragma once

struct init_sounds {
	SoundBuffer machineBuffer;//создаЄм буфер дл€ звука
	SoundBuffer damageBuffer;//создаЄм буфер дл€ звука
	SoundBuffer shotgunBuffer;//создаЄм буфер дл€ звука
	SoundBuffer rifleBuffer;//создаЄм буфер дл€ звука
	SoundBuffer reloadBuffer;//создаЄм буфер дл€ звука
	Sound shootMachine;
	Sound damage;
	Sound shootShootGun;
	Sound shootRifle;
	Sound shootReload;
	Music music;
	init_sounds() {
		music.openFromFile("sounds/droideka.ogg");

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
	Image bonusImage;
	Image exitImage;
	Image solidImage;
	Texture meetTexture;
	Sprite meetSprite;
	init_picture() {
		meetTexture.loadFromFile("images/meet.png");
		meetSprite.setTexture(meetTexture);
		bulletImage.loadFromFile("images/shot2.png");
		heroImage.loadFromFile("images/players_anim.png");
		gunImage.loadFromFile("images/weapons.png");
		equipImage.loadFromFile("images/equipment2.png");
		easyEnemyImage.loadFromFile("images/easy_enemy.png");
		banditImage.loadFromFile("images/bandit.png");
		mediumEnemyImage.loadFromFile("images/MediumEnemy.png");
		bonusImage.loadFromFile("images/bonus.png");
		exitImage.loadFromFile("images/exitImage.png");
		solidImage.loadFromFile("images/solidImage.png");
	}
};

struct init_fonts {
	Font albionic;
	Text text_albion;

	init_fonts(){
		albionic.loadFromFile("images/Albionic.ttf");
		text_albion.setCharacterSize(40);
		text_albion.setFont(albionic);
		text_albion.setColor(Color::Red);
	}
};

struct game_indicators {
	int countEnemy = 0;
	bool areaClean = false;
	float timeGame;
	Vector2f posView;
	Vector2f pos;
	init_sounds sounds;
	init_picture pict;
	init_fonts font;
	Level lvl;//создали экземпл€р класса уровень
	Object player;
	game_indicators() {
		lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.
		player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	}
};