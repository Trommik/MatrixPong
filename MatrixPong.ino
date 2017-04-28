#include "PongGame.h"
#include "Puck.h"
#include "Paddel.h"

#include <Adafruit_NeoPixel.h>

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

	renderStrip();

	//int leftLenght = map(scoreLeft, 0, 17, 0, 4);
	//int rightLenght = map(scoreRight, 0, 17, 0, 4);

	//leftPlayer.setHeight(START_PADDEL_HEIGHT-leftLenght);
	//rightPlayer.setHeight(START_PADDEL_HEIGHT-rightLenght);

	/*
	int waitTime;
	if(scoreLeft < scoreRight){
		waitTime = scoreRight;
	}else{
		waitTime = scoreLeft;
	}
	waitTime = WIDTH - waitTime;
	waitTime = map(waitTime, 0, 17, 10, 50);
	//Serial.println(waitTime);
	delay(waitTime);*/

	delay(100);
}

void paddelsMove()
{
	uint8_t left = map(analogRead(A1), 0, 1023, 1, HEIGHT - leftPlayer.getHeight());

	leftPlayer.move(left);



	if(puck.vel.x > 0){
		rightPlayer.move(puck.pos.y - 2);
	}else{
		if (rightPlayer.getPosY() + rightPlayer.getHeight() / 2 > HEIGHT / 2){
			rightPlayer.moveUp();
		}else if (rightPlayer.getPosY() - rightPlayer.getHeight() / 2 < HEIGHT / 2+1){
			rightPlayer.moveDown();
		}
	}

	
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