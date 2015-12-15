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

	init_sounds() {
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