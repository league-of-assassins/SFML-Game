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

	int unsigned width = 1820, height = 1080;
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

	int xVelo = 0, yVelo = 0;
	float speed = 7;


	int heroEffectSize = 25, heroEffectCount = 52, effectStart = -1;

	int breathingOrder = 1;



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


	bool sit = false, jump = false, jumpFall = false, fall = false, bottom = false, releasedS = true, right = false, left = false, enableEffect = false;
	bool over = false, restart = true, pause = false, groundCollide = false, borderCollide = false;

public:

	void setWindow();

	void fonts();

	void textures();

	void objects(RectangleShape heroEffect[52]);

	void events();

	void Restart();

	void moveHeroKey();

	void physics();

	void jumping();

	void askMove(int xVelo, int yVelo, bool x);

	void collision(int& i);

	void effectUpdate(RectangleShape heroEffect[52], Vector2f heroEffectPos[52]);

	void breathing();

	void setPositions();

	void displays(RectangleShape heroEffect[52]);

	Game();

	~Game();
};


void Game::setWindow() {
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

void Game::moveHeroKey() {
	xVelo = 0;
	yVelo = 7;


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
		releasedS = false;
		sit = true;
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


	//check X collision
	askMove(xVelo, 0, true);

	//check Y collision
	askMove(0, yVelo, false);


	breathing();
}

void Game::askMove(int xVelo, int yVelo, bool x) {
	int i = 0;
	bottom = false;

	heroPosTemp = heroPos;

	heroPos.x += xVelo;
	heroPos.y += yVelo;

	collision(i);



	//REJECT MOVE/JUMP/FALL IF COLLIDING
	if (groundCollide || borderCollide) {
		heroPos = heroPosTemp;
	}

	//CHECK IF STANDING/FALLING
	fall = false;

	if (!x) {
		if (groundCollide) {
			bottom = true;
		}
		else {
			enableEffect = true;
			if (!jump) {
				fall = true;
			}
		}
	}

	if (borderCollide && !x && jump) {
		jumpFall = true;
	}


	if (groundCollide) {
		if (x) {
			//REMOVE LEFT SIDE GAP
			if (heroPos.x + heroSize.x <= groundPos[i].x) {
				heroPos.x = groundPos[i].x - heroSize.x;
			}

			//REMOVE RIGHT SIDE GAP
			else if (heroPos.x >= groundPos[i].x + groundSize[i].x) {
				heroPos.x = groundPos[i].x + groundSize[i].x;
			}
		}

		else {
			//REMOVE BOTTOM GAP
			if (heroPos.y + heroSize.y <= groundPos[i].y) {
				heroPos.y = groundPos[i].y - heroSize.y;
				if (jump) { jump = false; jumpFall = false; }
			}

			//REMOVE TOP GAP AND STOP JUMP
			else if (heroPos.y >= groundPos[i].y + groundSize[i].y) {
				heroPos.y = groundPos[i].y + groundSize[i].y;
				if (jump) { jumpFall = true; }
			}
		}
	}
}

void Game::collision(int& i) {
	Collision collision;

	groundCollide = false;
	borderCollide = false;

	//BORDER HIT
	if (heroPos.x < 0 || heroPos.x + heroSize.x > width || heroPos.y < 0) {
		borderCollide = true;
	}

	if (heroPos.y + heroSize.y > height) {
		restart = true;
	}

	//GROUND HIT
	for (i = 0; i < groundNo; i++) {
		if (collision.check(heroPos, groundPos[i], heroSize, groundSize[i])) {
			groundCollide = true;
		}
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

	//BREATH IF STANDING ELSE REVERT SIZE TO ORIGIN IF JUMPING OR FALLING
	if (bottom || (breathingOrder == -1 && (jump || fall))) {
		if (jump || frame % 30 == 0) {
			heroSize.y -= 5 * breathingOrder;
			heroPos.y += 5 * breathingOrder;
			breathingOrder *= -1;
			hero.setSize(heroSize);
		}
	}
}

void Game::setPositions() {
	heroMask.setPosition(heroPos);
	hero.setPosition(heroPos);
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


		if (restart) {
			Restart();
		}


		// PHYSICS

		if (!over && !pause) {

			physics();

			effectUpdate(heroEffect, heroEffectPos);

			setPositions();


			frame++;
			if (frame == 99999) {
				tempFrame = 0 - frame - tempFrame; frame = 0;
			}
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
	ADD MOVEMENT/JUMP/FALL
	CHECK COLLISION: X, Y
	APPLY MOVEMENT
	BREATH
	UPDATE EFFECT
	SET POSITION
	DRAW
*/