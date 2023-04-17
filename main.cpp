#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

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
	int frame = 0, tempFrame = 0;

	Event event;

	Font font;

	Sprite gameOver;
	Texture texture_gameOver;



	Sprite heroMask;
	Texture texture_heroMask;

	RectangleShape hero;
	Vector2f heroSize;
	Vector2f heroPos;
	Vector2f heroPosTemp;

	int xVelo = 0, yVelo = 7;
	float speed = 7;



	int heroEffectSize = 25, heroEffectCount = 52, effectStart = -1;
	int breathingOrder = 1;
	bool heroEnemyJump = false, heroLoseHealth = false;


	RectangleShape enemy[4];
	Vector2f enemyPos[4];
	Vector2f enemySize;

	int enemySpeed = 5, enemyFrameTemp[4] = { 0, 120, 0, 300 }, enemyRight[4] = { 1, -1, 1, -1 };
	int enemyHitNo = 0;
	bool enemyCollide = false;
	bool enemyRemove[4] = { false };
	bool enemyWait[4] = { false, true, false, true };

	RectangleShape ground[12];
	Vector2f groundPos[12];
	Vector2f groundSize[12];

	Text textGr[12];

	int groundPosArr[12][2] = {
		{ 450, 600 }, { 650, 760 }, { 160, 490 },
		{ 1280, 300 }, { 250, 730 }, { 1500, 470 },
		{ 950, 640 }, { 950, 400 }, { 700, 490 },
		{ 1230, 550 }, { 450, 330 }, { 1600, 180 }
	};

	int groundNo = 12;




	bool sit = false, jump = false, jumpFall = false, bottom = false,
		groundColX = false, releasedS = true, right = false, left = false, enableEffect = false;
	bool over = false, restart = true, pause = false, groundCollide = false, borderCollide = false;

public:

	void setWindow();

	void fonts();

	void textures();

	void objects(RectangleShape heroEffect[52]);

	void events();

	void Restart();

	void enemyPhysics();

	void moveHeroKey();

	void physics();

	void jumping();

	void askMove(int xVelo, int yVelo);

	void collision(int& i);

	void effectUpdate(RectangleShape heroEffect[52], Vector2f heroEffectPos[52]);

	void breathing();

	void enemyHit();

	void setPositions();

	void frames();

	void displays(RectangleShape heroEffect[52]);

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
	gameOver.setPosition(Vector2f(-450, 0));

	if (!texture_heroMask.loadFromFile("resources\\images\\ninja head.png")) {
		cout << "\n\t Texture mask not found\n";
		return;
	}
	heroMask.setTexture(texture_heroMask);
}

void Game::objects(RectangleShape heroEffect[52]) {

	// HERO
	heroSize.x = 50; heroSize.y = 80;
	hero.setSize(heroSize);
	hero.setFillColor(Color::Cyan);
	heroPos.x = 25; heroPos.y = 25;
	hero.setPosition(heroPos);

	// HERO MASK
	heroMask.setScale(0.1, 0.1);
	heroMask.setPosition(heroPos);


	// HERO EFFECT
	int m = 1;
	for (int i = 0; i < heroEffectCount; i++) {
		heroEffect[i].setOrigin(heroEffectSize / 2 - 2, heroEffectSize / 2 - 2);
		heroEffect[i].setSize(Vector2f(heroEffectSize, heroEffectSize));

		heroEffect[i].rotate(i * 15);
		if (i > 10 && (i % 4 == 0 || i % 3 == 0)) { m = 0; }
		else { m = 1; }
		heroEffect[i].setFillColor(Color(i * 4, 0, 150, (255 - i * 5) * m));
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
		ground[i].setFillColor(Color::White);

		groundSize[i].x = 130 + i % 4 * 20;
		groundSize[i].y = 45;
		ground[i].setSize(groundSize[i]);

		groundPos[i].x = groundPosArr[i][0];
		groundPos[i].y = groundPosArr[i][1];
		ground[i].setPosition(groundPos[i]);

		textGr[i].setFont(font);
		textGr[i].setFillColor(Color::Black);
		tempS = to_string(i + 1);
		textGr[i].setString(tempS);
		textGr[i].setPosition(Vector2f(groundPos[i].x + 10, groundPos[i].y - 5));
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
		}

		else if (event.type == Event::KeyReleased && event.key.code == Keyboard::S) {
			releasedS = true;
		}
	}
}

void Game::Restart() {
	over = false;
	restart = false;

	if (jump) {
		jump = false; jumpFall = false;
		frame = 0; tempFrame = 0;
	}


	int baseLength = 200;

	effectStart = -1;
	heroPos.x = 300;
	heroPos.y = 0;
}

void Game::enemyPhysics() {
	int temp = 200;

	for (int i = 0; i < 4; i++) {

		if (!enemyWait[i]) {
			enemyPos[i].x += enemySpeed * enemyRight[i];

			if (enemyPos[i].x < 0 - enemySize.x || enemyPos[i].x > width) {
				enemyFrameTemp[i] = frame + 60 + rand() % 300;;
				enemyWait[i] = true;
			}

			enemy[i].setPosition(enemyPos[i]);
		}

		else if (frame == enemyFrameTemp[i]) {
			enemyPos[i].x = (enemyRight[i] + 1) / 2 * width;
			enemyPos[i].y = temp + i * 135 + rand() % 80;
			temp = 0;
			enemyRight[i] *= -1;
			enemyWait[i] = false;
		}


	}
}

void Game::moveHeroKey() {
	xVelo = 0;


	if (Keyboard::isKeyPressed(Keyboard::D)) {
		xVelo = speed;

		enableEffect = true;
		right = true;
	}

	else if (Keyboard::isKeyPressed(Keyboard::A)) {
		xVelo = -speed;

		enableEffect = true;
		left = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::W) && !jump && !sit && bottom) {
		jump = true;
		tempFrame = frame;
		speed = 4;
	}

	else if (Keyboard::isKeyPressed(Keyboard::S) && bottom) {
		if (releasedS) {
			heroPos.y += 20; heroSize.y -= 20; hero.setSize(heroSize);
		}
		sit = true;

		releasedS = false;
	}
}

void Game::jumping() {
	if (!jumpFall && (frame - tempFrame) == 40) {
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

void Game::physics() {
	xVelo = 0; yVelo = 7;

	enableEffect = false;


	//DISABLE SIT
	if (sit && releasedS) {
		sit = false;
		heroSize.y += 20;
		heroPos.y -= 20;
		hero.setSize(heroSize);
		yVelo = 0;
	}

	moveHeroKey();

	//SET MASK SIDE
	if (right) {
		heroMask.setScale(-0.1, 0.1);
		heroMask.setOrigin(500, 0);
		right = false;
	}

	else if (left) {
		heroMask.setScale(0.1, 0.1);
		heroMask.setOrigin(0, 0);
		left = false;
	}


	//JUMP
	if (jump) {
		jumping();
	}
	else { speed = 7; }


	//check collision
	askMove(xVelo, yVelo);


	breathing();
}

void Game::askMove(int xVelo, int yVelo) {
	int i = 0;
	bottom = false;


	heroPosTemp = heroPos;

	heroPos.x += xVelo;
	heroPos.y += yVelo;

	collision(i);


	// DETECT GROUND COL SIDE
	if (groundCollide) {

		// LEFT
		if (heroPosTemp.x + heroSize.x <= groundPos[i].x) {
			heroPos.x = groundPos[i].x - heroSize.x;
			groundColX = true;
		}

		// RIGHT
		else if (heroPosTemp.x >= groundPos[i].x + groundSize[i].x) {
			heroPos.x = groundPos[i].x + groundSize[i].x;
			groundColX = true;
		}

		// TOP
		else if (heroPosTemp.y + heroSize.y <= groundPos[i].y) {
			heroPos.y = groundPos[i].y - heroSize.y;

			bottom = true;
			if (jump) { jump = false; jumpFall = false; }
		}

		// BOTTOM
		else if (heroPosTemp.y >= groundPos[i].y + groundSize[i].y) {
			heroPos.y = groundPos[i].y + groundSize[i].y;

			if (jump) { jumpFall = true; }
		}
	}

	else {
		enableEffect = true;
	}



	if (borderCollide) {
		heroPos.x = heroPosTemp.x;
	}
}

void Game::collision(int& i) {
	Collision collision;

	groundCollide = false;
	borderCollide = false;
	enemyCollide = false;


	//BORDER COL
	if (heroPos.x < 0 || heroPos.x + heroSize.x > width || heroPos.y < 0) {
		borderCollide = true;

		if (heroPos.y < 0) { jumpFall = true; }
	}

	if (heroPos.y + heroSize.y > height) {
		restart = true;
	}


	//ENEMY COL
	for (i = 0; i <= 3; i++) {
		enemyCollide = collision.check(heroPos, enemyPos[i], heroSize, enemySize);
		if (enemyCollide) { enemyHitNo = i; break; }
	}

	//GROUND COL
	for (i = 0; i < groundNo; i++) {
		groundCollide = collision.check(heroPos, groundPos[i], heroSize, groundSize[i]);
		if (groundCollide) { break; }
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
	int gapx = 0;
	int n = -1;

	//MAKE EFFECT FOLLOW THE PREVIOUS

	if (enableEffect) { if (effectStart < 51) { effectStart += 2; } }
	else if (effectStart > -1) { effectStart -= 2; }

	if (effectStart > -1) {

		for (int i = 0; i <= 1; i++) {
			heroEffectPos[i].x = heroPos.x + heroSize.x / 2 + gapx;
			heroEffectPos[i].y = heroPos.y + heroSize.y / 2 - gapx;
			heroEffect[i].setPosition(heroEffectPos[i]);
			gapx *= -1;
		}

		if (effectStart > 1) {
			for (int i = effectStart; i > 1; i -= 2) {
				n *= -1;
				for (int j = 0; j <= 1; j++) {
					heroEffect[i - j].rotate(20 * n);
					heroEffectPos[i - j] = heroEffectPos[i - j - 2];
					heroEffect[i - j].setPosition(heroEffectPos[i - j]);
				}
			}
		}
	}
}

void Game::breathing() {

	if (bottom || (breathingOrder == -1 && (jump || (!bottom && !jump)))) {
		if (jump || frame % 30 == 0) {
			heroSize.y -= 5 * breathingOrder;
			heroPos.y += 5 * breathingOrder;
			breathingOrder *= -1;
			hero.setSize(heroSize);
		}
	}
}

void Game::enemyHit() {

	if (enemyCollide) {
		if (heroPosTemp.y > enemyPos[enemyHitNo].y) {
			heroEnemyJump = true;
		}

		else {
			heroLoseHealth = true;
		}

		enemyRemove[enemyHitNo] = true;
	}

	if (enemyCollide) {
	}
}

void Game::setPositions() {
	heroMask.setPosition(heroPos);
	hero.setPosition(heroPos);
}

void Game::frames() {
	frame++;

	//RESET FRAME
	if (frame == 99999) {
		tempFrame = 0 - frame - tempFrame;
		for (int i = 0; i <= 3; i++) {
			if (enemyWait[i]) { enemyFrameTemp[i] -= frame; }
		}
		frame = 0;
	}
}

Game::Game() {
	RectangleShape heroEffect[52];
	Vector2f heroEffectPos[52];

	fonts();
	textures();
	objects(heroEffect);
	setWindow();



	while (window.isOpen())
	{


		// EVENTS

		events();


		// PHYSICS

		if (!over && !pause) {

			enemyPhysics();

			physics();

			effectUpdate(heroEffect, heroEffectPos);

			enemyHit();

			setPositions();

			frames();
		}


		// RESTART

		if (restart) {
			Restart();
		}


		// DISPLAY

		displays(heroEffect);
	}
}

Game::~Game() {}

void Game::displays(RectangleShape heroEffect[52]) {

	window.clear();

	if (!over) {
		if (effectStart > -1) {
			for (int i = effectStart; i >= 0; i--) {
				window.draw(heroEffect[i]);
			}
		}

		for (int i = 0; i < groundNo; i++) {
			window.draw(ground[i]);
			window.draw(textGr[i]);
		}
		window.draw(hero);
		window.draw(heroMask);

		for (int i = 0; i < 4; i++) {
			window.draw(enemy[i]);
		}
	}

	else {
		window.draw(gameOver);
	}


	window.display();
}




int main()
{

	Game game;

	return 0;
}

/*
TO DO LIST :
	ADD ENEMIES
	ADJUST JUMPING PHYSICS

WORKING ORDER:
	CHECK KEY INPUT: OTHERS, MOVEMENT

	ENEMY PHYSICS
	ADD MOVEMENT/JUMP/FALL

	CHECK COLLISION: X, Y
	APPLY MOVEMENT

	ENEMY HIT
	BREATH
	UPDATE EFFECT

	SET POSITION

	DRAW
*/