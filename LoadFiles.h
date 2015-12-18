#pragma once

struct init_sounds {
	SoundBuffer machineBuffer;//������ ����� ��� �����
	SoundBuffer damageBuffer;//������ ����� ��� �����
	SoundBuffer shotgunBuffer;//������ ����� ��� �����
	SoundBuffer rifleBuffer;//������ ����� ��� �����
	SoundBuffer reloadBuffer;//������ ����� ��� �����
	Sound shootMachine;
	Sound damage;
	Sound shootShootGun;
	Sound shootRifle;
	Sound shootReload;
	Music music;
	init_sounds() {
		music.openFromFile("sounds/droideka.ogg");

		machineBuffer.loadFromFile("sounds/MachineGun.wav");//��������� � ���� ����
		shootMachine.setBuffer(machineBuffer);

		damageBuffer.loadFromFile("sounds/ZombieAttack1.wav");//��������� � ���� ����
		damage.setBuffer(damageBuffer);//������� ���� � ��������� � ���� ���� �� ������

		shotgunBuffer.loadFromFile("sounds/PumpShotgun.wav");//��������� � ���� ����
		shootShootGun.setBuffer(shotgunBuffer);//������� ���� � ��������� � ���� ���� �� ������

		rifleBuffer.loadFromFile("sounds/Rifle.wav");//��������� � ���� ����
		shootRifle.setBuffer(rifleBuffer);//������� ���� � ��������� � ���� ���� �� ������

		reloadBuffer.loadFromFile("sounds/Reload.wav");//��������� � ���� ����
		shootReload.setBuffer(reloadBuffer);//������� ���� � ��������� � ���� ���� �� ������	
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
	Level lvl;//������� ��������� ������ �������
	Object player;
	game_indicators() {
		lvl.LoadFromFile("map.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.
		player = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	}
};