#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;



class Collision {
public:
	bool check(Vector2f firstPos, Vector2f secondPos, Vector2f firstSize, Vector2f secondSize);
};



class Game {
private:

	unsigned int width, height;
	RenderWindow window;
	int frame = 0, jumpFrame = 0;

	Event event;

	Font font;

	Sprite gameOver;
	Texture texture_gameOver;



	Sprite heroMask;
	Texture texture_heroMask;

	RectangleShape heroHead;
	Vector2f heroHeadSize;
	Vector2f heroHeadPos;



	RectangleShape heroBody;
	Vector2f heroBodySize;
	Vector2f heroBodyPos;


	Vector2f heroTotalSize;
	Vector2f heroTotalPos;


	RectangleShape heroArm;

	Vector2f heroArmPos;
	Vector2f heroArmSize;

	double armRotation = 0;



	RectangleShape heroLegLeft;
	Vector2f heroLegLeftPos;

	RectangleShape heroLegRight;
	Vector2f heroLegRightPos;

	Vector2f heroLegSize;
	Vector2f heroLegSizeTotal;



	Vector2f heroPosTemp;

	int xVelo = 0, yVelo = 7;
	float speed = 7;



	Vector2f mousePos;
	bool mouseReleased = true;



	RectangleShape gun;
	Vector2f gunPos;
	Vector2f gunSize;

	double gunRotation = 0;
	bool emptyArms = false;



	RectangleShape bullet;
	Vector2f bulletPos;
	Vector2f bulletPosTemp;
	Vector2f bulletSize;
	Vector2f bulletVelo;

	int bulletFrame = 0;
	bool fired = false, bulletActive = false, bulletTime = false, cut = false;



	int heroEffectSize = 25, heroEffectCount = 52, effectStart = -1;
	int breathingOrder = 1;
	bool heroEnemyJump = false, enemyHitTop = false;



	RectangleShape health[3];
	int healthCount = 3;



	RectangleShape enemy[4];
	Vector2f enemyPos[4];
	Vector2f enemySize;

	int enemySpeed = 5, enemySpawnFrame[4] = { 60, 120, 0, 300 }, enemyRight[4] = { -1, -1, 1, -1 };
	int enemyHitNo = 0;
	bool enemyWait[4] = { false, true, false, true };



	RectangleShape ground[12];
	Vector2f groundPos[12];
	Vector2f groundSize[12];

	Text textGr[12];

	int groundPosArr[12][2] = {
		{ 1600, 180 }, { 450, 310 }, { 1260, 310 },
		{ 950, 400 }, { 160, 480 }, { 1460, 470 },
		{ 700, 480 }, { 410, 570  }, { 1200, 570 },
		{ 960, 640 }, { 670, 760 }, { 250, 760 }
	};

	int groundNo = 12;



	RectangleShape terrain;
	Vector2f terrainSize;



	int rainCount = 100;


	bool sit = false, InitSit = false, jump = false, jumpFall = false, safeFall = false, heroBlink = false, bottom = false,
		groundColX = false, releasedS = true, right = false, left = false, enableEffect = false;
	bool over = false, restart = true, pause = false,
		groundColHero = false, borderColHero = false, enemyColHero = false,
		groundColBullet = false, borderColBullet = false, enemyColBullet = false;

public:

	void setWindow();

	void fonts();

	void textures();

	void objects(RectangleShape heroEffect[52], RectangleShape rain[100], Vector2f rainPos[100]);

	void events();

	void Restart();

	void enemyPhysics();

	void enemyHit();

	void mousep();

	void findRotation(double& rotation, double x, double y);

	void armp(double& x, double& y);

	void gunp(double& x, double& y);

	void bulletp(double x, double y);

	void bulletReset();

	void Jump();

	void Sit();

	void moveHeroKey();

	void physics();

	void colMain();

	void colBase(int& i, bool heroTurn, bool& groundColBase, bool& enemyColBase, bool& borderColBase, Vector2f& firstPos, Vector2f& firstSize, Vector2f& firstPosTemp);

	void colGroundFix(int i, Vector2f& firstPos, Vector2f& firstSize, Vector2f& firstPosTemp);

	void effectUpdate(RectangleShape heroEffect[52], Vector2f heroEffectPos[52]);

	void raining(RectangleShape rain[100], Vector2f rainPos[100]);

	void breathing();

	void setPos();

	void frames();

	void displays(RectangleShape heroEffect[52], RectangleShape rain[100]);

	Game();

	~Game();
};



void Game::setWindow() {
	width = VideoMode::getDesktopMode().width - 100;
	height = VideoMode::getDesktopMode().height;

	window.create(VideoMode(width, height), "SFML Game", Style::None);
	window.setFramerateLimit(60);
}

void Game::fonts() {
	if (!font.loadFromFile("resources\\fonts\\Jersey Sharp.ttf")) {
		cout << "\n\t Font not found\n";
		return;
	}
}

void Game::textures() {

	if (!texture_gameOver.loadFromFile("resources\\images\\over.jpg")) {
		cout << "\n\t Texture game over not found\n";
		return;
	}
	gameOver.setTexture(texture_gameOver);

	if (!texture_heroMask.loadFromFile("resources\\images\\ninja head.png")) {
		cout << "\n\t Texture mask not found\n";
		return;
	}
	heroMask.setTexture(texture_heroMask);
}

void Game::objects(RectangleShape heroEffect[52], RectangleShape rain[100], Vector2f rainPos[100]) {
	// HERO

	// HERO HEAD
	heroHeadSize.x = 40; heroHeadSize.y = 40;
	heroHead.setSize(heroHeadSize);
	heroHead.setFillColor(Color::Black);
	heroHeadPos.x = 25; heroHeadPos.y = 25;
	heroHead.setPosition(heroHeadPos);



	// HERO BODY
	heroBodySize.x = 40; heroBodySize.y = 60;
	heroBody.setSize(heroBodySize);
	heroBody.setFillColor(Color::Black);
	heroBodyPos.x = 25; heroBodyPos.y = 25;
	heroBody.setPosition(heroBodyPos);
	heroBody.setOutlineThickness(-1);
	heroBody.setOutlineColor(Color::Cyan);

	//HERO ARMS
	heroArmSize.x = 50;
	heroArmSize.y = 20;

	heroArm.setFillColor(Color::Black);
	heroArm.setSize(heroArmSize);
	heroArm.setOrigin(0, heroArmSize.y / 2);
	heroArm.setOutlineThickness(-1);
	heroArm.setOutlineColor(Color::Cyan);


	// HERO LEGS
	heroLegSize.x = 20;
	heroLegSize.y = 50;

	heroLegSizeTotal.x = 40;
	heroLegSizeTotal.y = 50;

	heroLegLeft.setSize(heroLegSize);
	heroLegLeft.setFillColor(Color::Black);


	heroLegRight = heroLegLeft;

	// HERO MASK
	heroMask.setScale(0.08, 0.08);
	heroMask.setPosition(heroBodyPos);

	// HERO GUN
	gunSize.x = 100;
	gunSize.y = 20;
	gun.setSize(gunSize);
	gun.setFillColor(Color::Black);
	gun.setOrigin(0, gunSize.y / 2);

	gun.setOutlineThickness(-1);
	gun.setOutlineColor(Color::Red);


	// BULLET
	bulletSize.x = 20;
	bulletSize.y = 20;

	bullet.setSize(bulletSize);
	bullet.setFillColor(Color::Black);
	bullet.setOrigin(bulletSize.x / 2, bulletSize.y / 2);
	bullet.setOutlineThickness(-3);
	bullet.setOutlineColor(Color::Cyan);


	// HERO EFFECT
	int temp = 1;
	for (int i = 0; i < heroEffectCount; i++) {
		heroEffect[i].setOrigin(heroEffectSize / 2 - 2, heroEffectSize / 2 - 2);
		heroEffect[i].setSize(Vector2f(heroEffectSize, heroEffectSize));

		heroEffect[i].rotate(i * 15);
		if (i > 10 && (i % 4 == 0 || i % 3 == 0)) { temp = 0; }
		else { temp = 1; }
		heroEffect[i].setFillColor(Color(i * 4, 0, 150, (255 - i * 5) * temp));
	}


	//HEALTH
	health[0].setSize(Vector2f(25, 25));
	health[0].setFillColor(Color::Black);
	health[0].setRotation(45);

	for (int i = 0; i <= 2; i++) {
		health[i] = health[0];
		health[i].setPosition(1800 - i * 50, 50);
	}


	// ENEMY
	enemySize.x = 50;
	enemySize.y = 25;
	enemy[0].setSize(enemySize);
	enemy[0].setFillColor(Color::Red);
	enemyPos[0].x = 0 - enemySize.x;
	enemyPos[0].y = 200;
	enemy[0].setPosition(enemyPos[0]);

	for (int i = 1; i <= 3; i++) {
		enemy[i] = enemy[0];
		enemyPos[i].x = enemyPos[0].x;
		enemyPos[i].y = enemyPos[0].y + i * 150;
		enemy[i].setPosition(enemyPos[i]);
	}


	// GROUND
	string tempS;
	for (int i = 0; i < groundNo; i++) {
		groundSize[i].x = 130 + i % 4 * 20;
		groundSize[i].y = 45;
		ground[i].setSize(groundSize[i]);
		ground[i].setFillColor(Color::Black);

		groundPos[i].x = groundPosArr[i][0];
		groundPos[i].y = groundPosArr[i][1];
		ground[i].setPosition(groundPos[i]);

		textGr[i].setFont(font);
		textGr[i].setFillColor(Color::Black);
		tempS = to_string(i + 1);
		textGr[i].setString(tempS);
		textGr[i].setPosition(Vector2f(groundPos[i].x + 10, groundPos[i].y - 5));
	}

	// TERRAIN

	terrainSize.x = width;
	terrainSize.y = 50;
	terrain.setSize(terrainSize);
	terrain.setPosition(Vector2f(0, height - terrainSize.y));
	terrain.setFillColor(Color::Black);


	// RAIN
	for (int i = 0; i < rainCount; i++) {
		rain[i].setSize(Vector2f(1, 7 + rand() % 10));
		rain[i].setFillColor(Color::Black);

		rainPos[i].x = rand() % width;
		rainPos[i].y = rand() % height;
		rain[i].setPosition(rainPos[i]);
	}
}

void Game::events() {
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed) window.close();

		else if (event.type == Event::KeyPressed) {

			if (event.key.code == Keyboard::Escape) {
				window.close();
			}

			else if (event.key.code == Keyboard::R) {
				if (!pause) {
					restart = true;
				}

				else {
					pause = false;
				}
			}

			else if (event.key.code == Keyboard::P && !over) {
				pause = true;
			}

			else if (event.key.code == Keyboard::Q) {
				if (!emptyArms) { emptyArms = true; }
				else { emptyArms = false; }
			}
		}

		else if (event.type == Event::KeyReleased && event.key.code == Keyboard::S) {
			releasedS = true;
		}

		else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
			mouseReleased = true;
		}
	}
}

void Game::Restart() {
	if (over) {
		over = false;
		healthCount = 3;
	}
	restart = false;
	safeFall = true;

	//RESET HERO
	effectStart = -1;
	heroBodyPos.x = 300;
	heroBodyPos.y = 0;

	if (bulletActive) {
		bulletReset();
	}

	//CHECK OVER
	if (healthCount == 0) {
		over = true;
	}
}

void Game::enemyPhysics() {

	// UPDATE ENEMY POSITION
	for (int i = 0; i <= 3; i++) {

		if (!enemyWait[i]) {
			enemyPos[i].x += enemySpeed * enemyRight[i];

			if (enemyPos[i].x < 0 - enemySize.x || enemyPos[i].x > width) {
				enemySpawnFrame[i] = frame + 60 + rand() % 60;
				enemyWait[i] = true;
			}

			enemy[i].setPosition(enemyPos[i]);
		}

		else if (frame == enemySpawnFrame[i]) {
			enemyPos[i].y = groundPosArr[i * 3 + rand() % ((i + 1) * 3)][1] - (50 + enemySize.y + 15 * (rand() % 1));
			enemyRight[i] *= -1;
			enemyWait[i] = false;
		}
	}
}

void Game::enemyHit() {

	if (enemyColHero || enemyColBullet) {

		if (enemyColHero) {
			if (enemyHitTop) {
				enemyHitTop = false;
				heroEnemyJump = true;
			}

			else {
				healthCount--;
				restart = true;
			}
		}

		int temp = (enemyRight[enemyHitNo] + 1) / 2 * (width + 50) - 50;
		enemyPos[enemyHitNo].x = temp;
	}
}

void Game::mousep() {
	mousePos.x = Mouse::getPosition().x;
	mousePos.y = Mouse::getPosition().y;

	double x = mousePos.x - gunPos.x - heroBodySize.x;
	double y = mousePos.y - gunPos.y;

	//DETECT MOUSE PRESS
	if (Mouse::isButtonPressed(Mouse::Left)) {
		if (emptyArms) {
			cut = true;
		}

		else {
			if (mouseReleased) {
				if (!safeFall) {
					fired = true;
				}
			}
		}

		if (mouseReleased) {
			mouseReleased = false;
		}
	}


	// HERO SIDE
	if (x >= 0) {
		right = true;
	}
	else { left = true; }

	armp(x, y);

	gunp(x, y);

}

void Game::findRotation(double& rotation, double x, double y) {

	rotation = 0;

	if (x < 0) {
		rotation += 90;
		if (y < 0) { rotation += 90; }
	}
	else if (y < 0) { rotation += 270; }

	if ((x < 0 && y >= 0) || (x >= 0 && y < 0)) {
		int temp = x;
		x = y;
		y = temp;
	}

	rotation += atan(abs(y) / abs(x)) * 180 / 3.1415;
}

void Game::armp(double& x, double& y) {
	if (emptyArms) {
		armRotation = 90;

		if (cut) {
			findRotation(armRotation, x, y);
			cut = false;
		}

		heroArm.setRotation(armRotation);
	}
}

void Game::gunp(double& x, double& y) {

	// BULLET PHYSICS
	bulletp(x, y);


	if (!emptyArms) {

		findRotation(gunRotation, x, y);

		gun.setRotation(gunRotation);
	}
}

void Game::bulletp(double x, double y) {
	if (fired) {
		if (!emptyArms && !bulletTime && !bulletActive) {

			// FIND BULLET ROTATION
			double bulletRotation;
			findRotation(bulletRotation, x, y);
			bullet.setRotation(bulletRotation);

			// FIND BULLET START POSITION
			bulletPos = gunPos;

			double tempB = sqrt(pow(x, 2) + pow(y, 2));

			tempB = gunSize.x / tempB;

			bulletPos.x += x * tempB;
			bulletPos.y += y * tempB;

			bulletVelo.x = x * tempB / 5;
			bulletVelo.y = y * tempB / 5;

			// REST
			bulletTime = true;
			bulletActive = true;
			bulletFrame = frame + 120;
		}
		fired = false;
	}

	if (bulletTime) {

		if (frame == bulletFrame) {
			bulletReset();
			bulletTime = false;
		}

		if (bulletActive) {
			bulletPosTemp = bulletPos;

			bulletPos.x += bulletVelo.x;
			bulletPos.y += bulletVelo.y;

			if (groundColBullet || borderColBullet || enemyColBullet) {
				bulletReset();
			}

			bullet.setPosition(bulletPos);
		}
	}
}

void Game::bulletReset() {
	bulletActive = false;
	bulletPos.x = -100;
	bulletPos.y = -100;

	groundColBullet = false;
	borderColBullet = false;
	enemyColBullet = false;

}


void Game::Jump() {
	if (jump) {
		if (!jumpFall && frame == jumpFrame) {
			jumpFall = true;
		}

		if (!jumpFall) {
			yVelo = -7;
			speed += 0.1;
		}

		else if (speed > 5) {
			speed -= 0.1;
		}
	}

	else {
		speed = 7;
	}
}

void Game::Sit() {
	if (InitSit) {
		heroBodyPos.y += 20; heroBodySize.y -= 20; heroBody.setSize(heroBodySize);
		InitSit = false;
	}

	if (sit && releasedS) {
		sit = false;
		heroBodySize.y += 20;
		heroBodyPos.y -= 20;
		heroBody.setSize(heroBodySize);
		yVelo = 0;
	}
}

void Game::moveHeroKey() {
	xVelo = 0;


	if (Keyboard::isKeyPressed(Keyboard::D)) {
		xVelo = speed;

		enableEffect = true;
	}

	else if (Keyboard::isKeyPressed(Keyboard::A)) {
		xVelo = -speed;

		enableEffect = true;
	}

	if ((Keyboard::isKeyPressed(Keyboard::W) && !jump && !sit && bottom) || heroEnemyJump) {
		jump = true;
		jumpFall = false;
		jumpFrame = frame + 40;
		speed = 4;

		if (heroEnemyJump) { heroEnemyJump = false; }
	}

	else if (Keyboard::isKeyPressed(Keyboard::S) && bottom) {
		if (releasedS) {
			InitSit = true;
		}
		sit = true;
		releasedS = false;
	}
}

void Game::physics() {
	xVelo = 0; yVelo = 7;
	enableEffect = false;

	//ENEMY PHYSICS
	enemyPhysics();

	//MOVE KEY INPUT
	moveHeroKey();

	//JUMP
	Jump();

	//SIT
	Sit();

	//APPROVE MOVEMENT
	colMain();

	//BREATHE
	breathing();
}

void Game::colMain() {
	int i = 0;
	bottom = false;

	//HERO COL
	heroTotalPos.x = heroBodyPos.x;
	heroTotalPos.y = heroBodyPos.y - heroHeadSize.y;
	heroPosTemp = heroTotalPos;
	heroTotalPos.x += xVelo;
	heroTotalPos.y += yVelo;

	heroTotalSize.x = heroHeadSize.x;
	heroTotalSize.y = heroHeadSize.y + heroBodySize.y + heroLegSize.y;

	colBase(i, true, groundColHero, enemyColHero, borderColHero, heroTotalPos, heroTotalSize, heroPosTemp);

	double changeX = 0;
	changeX = heroTotalPos.x - heroPosTemp.x;
	double changeY = 0;
	changeY = heroTotalPos.y - heroPosTemp.y;

	heroHeadPos.x += changeX;
	heroHeadPos.y += changeY;

	heroBodyPos.x += changeX;
	heroBodyPos.y += changeY;

	heroLegLeftPos.x += changeX;
	heroLegLeftPos.y += changeY;


	//BULLET COL
	if (bulletActive) {
		Vector2f bulletPosOrigin(bulletPos.x - bulletSize.x / 2, bulletPos.y - bulletSize.y / 2);
		colBase(i, false, groundColBullet, enemyColBullet, borderColBullet, bulletPosOrigin, bulletSize, bulletPosTemp);
	}
}

void Game::colBase(int& i, bool heroTurn, bool& groundColBase, bool& enemyColBase, bool& borderColBase, Vector2f& firstPos, Vector2f& firstSize, Vector2f& firstPosTemp) {
	Collision collision;

	groundColBase = false;
	borderColBase = false;
	enemyColBase = false;


	//BORDER COL
	if (firstPos.x < 0 || firstPos.x + firstSize.x > width || firstPos.y < 0 || firstPos.y + firstSize.y > height - terrainSize.y) {
		borderColBase = true;

		if (heroTurn) {
			if (firstPos.y <= 0) { jumpFall = true; }

			else if (firstPos.y + firstSize.y > height - terrainSize.y) { healthCount--; restart = true; }

			if (firstPos.x + firstSize.x > width) { firstPos.x = width - firstSize.x; }

			else if (firstPos.x < 0) { firstPos.x = 0; }
		}
	}

	//GROUND COL
	for (i = 0; i < groundNo; i++) {
		if (collision.check(firstPos, groundPos[i], firstSize, groundSize[i])) {
			groundColBase = true;

			if (heroTurn) {
				colGroundFix(i, firstPos, firstSize, firstPosTemp);
			}
		}
	}



	//ENEMY COL
	if (!safeFall) {
		for (i = 0; i <= 3; i++) {
			if (collision.check(firstPos, enemyPos[i], firstSize, enemySize)) {
				enemyColBase = true;
				enemyHitNo = i;
				if (firstPosTemp.y + firstSize.y <= enemyPos[enemyHitNo].y) {
					enemyHitTop = true;
				}
				break;
			}
		}
	}
}

void Game::colGroundFix(int i, Vector2f& firstPos, Vector2f& firstSize, Vector2f& firstPosTemp) {

	// LEFT
	if (firstPosTemp.x + firstSize.x <= groundPos[i].x) {
		firstPos.x = groundPos[i].x - firstSize.x;
	}

	// RIGHT
	else if (firstPosTemp.x >= groundPos[i].x + groundSize[i].x) {
		firstPos.x = groundPos[i].x + groundSize[i].x;
	}

	// TOP
	else if (firstPosTemp.y + firstSize.y <= groundPos[i].y) {
		firstPos.y = groundPos[i].y - firstSize.y;

		bottom = true;
		if (safeFall) { safeFall = false; heroBlink = false; }
		if (jump) { jump = false; jumpFall = false; }
	}

	// BOTTOM
	else if (firstPosTemp.y >= groundPos[i].y + groundSize[i].y) {
		firstPos.y = groundPos[i].y + groundSize[i].y;

		if (jump) { jumpFall = true; }
	}

}

bool Collision::check(Vector2f firstPos, Vector2f secondPos, Vector2f firstSize, Vector2f secondSize) {
	bool collide = false;

	if ((firstPos.x + firstSize.x > secondPos.x && firstPos.x < secondPos.x + secondSize.x) &&
		(firstPos.y + firstSize.y > secondPos.y && firstPos.y < secondPos.y + secondSize.y)) {
		collide = true;
	}

	return collide;
}

void Game::effectUpdate(RectangleShape heroEffect[52], Vector2f heroEffectPos[52]) {
	int gap = 3;
	int side = -1;

	//MAKE EFFECT FOLLOW THE PREVIOUS
	if (!groundColHero) { enableEffect = true; }

	if (enableEffect) { if (effectStart < 51) { effectStart += 2; } }
	else if (effectStart > -1) { effectStart -= 2; }

	if (effectStart > -1) {

		for (int i = 0; i <= 1; i++) {
			heroEffectPos[i].x = heroBodyPos.x + heroBodySize.x / 2 + gap;
			heroEffectPos[i].y = heroBodyPos.y + heroBodySize.y / 2 - gap;
			heroEffect[i].setPosition(heroEffectPos[i]);
			gap *= -1;
		}

		if (effectStart > 1) {
			for (int i = effectStart; i > 1; i -= 2) {
				side *= -1;
				for (int j = 0; j <= 1; j++) {
					heroEffect[i - j].rotate(20 * side);
					heroEffectPos[i - j] = heroEffectPos[i - j - 2];
					heroEffect[i - j].setPosition(heroEffectPos[i - j]);
				}
			}
		}
	}

	// SAFE FALL
	if (safeFall && frame % 10 == 0) {
		if (!heroBlink) { heroBlink = true; }
		else { heroBlink = false; }
	}

	//SET MASK SIDE
	if (right) {
		heroMask.setScale(-0.08, 0.08);
		heroMask.setOrigin(500, 0);
		right = false;
	}

	else if (left) {
		heroMask.setScale(0.08, 0.08);
		heroMask.setOrigin(0, 0);
		left = false;
	}
}

void Game::breathing() {

	if (bottom || (breathingOrder == -1 && (jump || (!bottom && !jump)))) {
		if (jump || frame % 30 == 0) {
			heroBodySize.y -= 5 * breathingOrder;
			heroBodyPos.y += 5 * breathingOrder;
			breathingOrder *= -1;
			heroBody.setSize(heroBodySize);
		}
	}
}

void Game::setPos() {
	gunPos.x = heroBodyPos.x + heroBodySize.x / 2;
	gunPos.y = heroBodyPos.y + heroBodySize.y / 2;
	gun.setPosition(gunPos);

	heroHeadPos.x = heroBodyPos.x;
	heroHeadPos.y = heroBodyPos.y - heroHeadSize.y;
	heroHead.setPosition(heroHeadPos);
	heroMask.setPosition(heroHeadPos);

	heroArmPos.x = heroBodyPos.x + heroBodySize.x / 2;
	heroArmPos.y = heroBodyPos.y + heroBodySize.y * 0.25;
	heroArm.setPosition(heroArmPos);

	heroBody.setPosition(heroBodyPos);

	heroLegLeftPos.x = heroBodyPos.x + heroBodySize.x / 2 - heroLegSize.x;
	heroLegLeftPos.y = heroBodyPos.y + heroBodySize.y;
	heroLegLeft.setPosition(heroLegLeftPos);

	heroLegRightPos.x = heroLegLeftPos.x + heroLegSize.x;
	heroLegRightPos.y = heroLegLeftPos.y;
	heroLegRight.setPosition(heroLegRightPos);
}

void Game::frames() {
	frame++;

	//RESET FRAME
	if (frame == 99999) {
		jumpFrame -= frame;

		if (bulletActive) { bulletFrame -= frame; }

		for (int i = 0; i <= 3; i++) {
			if (enemyWait[i]) { enemySpawnFrame[i] -= frame; }
		}
		frame = 0;
	}
}


void Game::raining(RectangleShape rain[100], Vector2f rainPos[100]) {

	//UPDATE RAIN POSITION
	for (int i = 0; i < rainCount; i++) {
		rainPos[i].y += 5;
		if (rainPos[i].y >= height) { rainPos[i].y = 0; }

		rainPos[i].x += 2;
		if (rainPos[i].x >= width) { rainPos[i].x = 0; }

		rain[i].setPosition(rainPos[i]);
	}
}

Game::Game() {
	//LARGE VARS SET AS LOCAL
	RectangleShape heroEffect[52];
	Vector2f heroEffectPos[52];

	RectangleShape rain[100];
	Vector2f rainPos[100];

	//INITIALIZERS
	setWindow();
	fonts();
	textures();
	objects(heroEffect, rain, rainPos);


	// MAIN LOOP
	while (window.isOpen())
	{


		// EVENTS

		events();

		mousep();


		if (!over && !pause) {

			// PHYSICS

			physics();

			enemyHit();

			// EFFECTS

			effectUpdate(heroEffect, heroEffectPos);

			raining(rain, rainPos);

			setPos();

			frames();
		}


		// RESTART

		if (restart) {
			Restart();
		}


		// DISPLAY

		displays(heroEffect, rain);
	}
}

Game::~Game() {}

void Game::displays(RectangleShape heroEffect[52], RectangleShape rain[100]) {

	//BACKGROUND COLOR
	window.clear(Color::White);

	//DRAW OBJECTS
	if (!over) {

		for (int i = 0; i < rainCount; i++) {
			window.draw(rain[i]);
		}

		window.draw(terrain);

		for (int i = 0; i < healthCount; i++) {
			window.draw(health[i]);
		}

		for (int i = 0; i < 4; i++) {
			window.draw(enemy[i]);
		}

		for (int i = 0; i < groundNo; i++) {
			window.draw(ground[i]);
			window.draw(textGr[i]);
		}

		if (!heroBlink) {
			if (effectStart > -1) {
				for (int i = effectStart; i >= 0; i--) {
					window.draw(heroEffect[i]);
				}
			}

			window.draw(heroHead);
			window.draw(heroMask);

			window.draw(heroBody);

			window.draw(heroLegLeft);
			window.draw(heroLegRight);

			if (emptyArms) {
				window.draw(heroArm);
				window.draw(heroArm);
			}

			else {
				window.draw(gun);
			}
		}

		if (bulletActive) {
			window.draw(bullet);
		}
	}

	else {
		window.draw(gameOver);
	}

	//DISPLAY THE OBJECTS
	window.display();
}




int main()
{

	Game game;

	return 0;
}

/*
	TO DO:
		ADD FALLING OBJECTS WHICH YOU CAN SHOOT WITH YOUR WEAPON
		DROP HEALTH ITEM WITH LUCK IF ENEMY HIT
		FIX ENEMY SPAWN POSITIONS
		ADD BULLET FIRE ANIMATION
		CHANGE ENEMY SPRITE
		CHANGE GUN & BULLET SPRITE

		FIX CORNER COLLISION

*/