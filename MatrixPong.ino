#include "PongGame.h"
#include "Puck.h"
#include "Paddel.h"

#include <Adafruit_NeoPixel.h>

struct renderData red = {0, 0, 4 };
struct renderData green = { 2, 0, 2 };
struct renderData blue = { 0, 2, 5 };

Adafruit_NeoPixel strip = Adafruit_NeoPixel(289, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
	randomSeed(A5);

	Serial.begin(9600);
	Serial.println("Pong Game Serial Console");

	strip.begin();
	strip.show();

	puck.init();
	leftPlayer.init(LEFT_SIDE);
	rightPlayer.init(RIGHT_SIDE);
}

void loop()
{
	paddelsMove();

	puck.update();
	
	for(int off = 0; off < leftPlayer.getHeight(); off++){
		pong.set(leftPlayer.render(off));
	}
	for (int off = 0; off < rightPlayer.getHeight(); off++) {
		pong.set(rightPlayer.render(off));
	}

	pong.setScore(scoreRight, RIGHT_SIDE); // right side = top side
	pong.setScore(scoreLeft, LEFT_SIDE); // left side = bottom side
	

	pong.set(puck.render()); // render Puck
	pong.set(puck.renderLast()); // render last Puck 
	pong.set(puck.renderLastLast()); // render lastlast Puck 

	//pong.set(red);
	//pong.set(green);
	//pong.set(blue);

	renderStrip();

	int leftLenght = map(scoreLeft, 0, 17, 0, 4);
	int rightLenght = map(scoreRight, 0, 17, 0, 4);

	leftPlayer.setHeight(START_PADDEL_HEIGHT-leftLenght);
	rightPlayer.setHeight(START_PADDEL_HEIGHT-rightLenght);

	int waitTime;
	if(scoreLeft < scoreRight){
		waitTime = scoreRight;
	}else{
		waitTime = scoreLeft;
	}
	waitTime = WIDTH - waitTime;
	waitTime = map(waitTime, 0, 17, 10, 50);
	//Serial.println(waitTime);
	delay(waitTime);
}

void paddelsMove()
{
	uint8_t left = map(analogRead(A1), 0, 1023, 1, HEIGHT - 1 - leftPlayer.getHeight());
	uint8_t right = map(analogRead(A0), 0, 1023, 1, HEIGHT - 1 - rightPlayer.getHeight());

	leftPlayer.move(left);
	rightPlayer.move(right);
}

void renderStrip()
{
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			color c = pong.getColor(x, y);

			int index;
			if (y%2){
				index = (y * HEIGHT) + (WIDTH-1) - x;
			}else{
				index = y * HEIGHT + x;
			}

			strip.setPixelColor(index, strip.Color(c.r, c.g, c.b));
		}
	}
	strip.show();
	pong.reset();
}