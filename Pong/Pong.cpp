#include "stdafx.h"
#include <SDL.h>
#include <string>
#include <iostream>

struct objectSDL
{
	SDL_Rect dst;
	SDL_Surface* surface;
};

class Balle
{
public:
	Balle(int coteBalle, int pX, int pY, int, int);
	~Balle();
	//Position
	void setPosX(int pos);
	void setPosY(int pos);
	int getPosX();
	int getPosY();

	//Velocité
	int getVelX();
	int getVelY();
	void setVelX(int vel);
	void setVelY(int vel);

	//Dimension
	int getDim();

	void rebondirPaddle();
	void rebondirTop();
	void chargerBalle();
	void blit(SDL_Surface * surface);
	void rebondirBot();
	void updatePos(double deltaTime);


private:
	int posX;
	int posY;
	int velX;
	int velY;
	int cote;
	objectSDL object;
};

Balle::Balle(int coteBalle, int pX, int pY, int vX, int vY)
{
	cote = object.dst.h = object.dst.w = coteBalle;
	posX = object.dst.x = pX;
	posY = object.dst.y = pY;
	velX = vX;
	velY = vY;
}

Balle::~Balle()
{

}

void Balle::setPosX(int pos) {
	posX = object.dst.x = pos;
}

void Balle::setPosY(int pos)
{
	posY = object.dst.y = pos;
}

int Balle::getPosX()
{
	return posX;
}

int Balle::getPosY()
{
	return posY;
}

int Balle::getVelX()
{
	return velX;
}

int Balle::getVelY()
{
	return velY;
}

void Balle::setVelX(int vel)
{
	velX = vel;
}

void Balle::setVelY(int vel)
{
	velY = vel;
}

int Balle::getDim()
{
	return cote;
}

void Balle::updatePos(double deltaTime)
{
	object.dst.x = posX = posX + velX * deltaTime;
	object.dst.y = posY =  posY + velY * deltaTime;
}

void Balle::rebondirPaddle()
{
	velX *= -1;
}

void Balle::rebondirTop()
{
	posY = object.dst.y = 16;
	velY *= -1;
}

void Balle::chargerBalle() {
	object.surface = SDL_LoadBMP("ball.bmp");
}

void Balle::blit(SDL_Surface* surface) {
	SDL_BlitSurface(object.surface, NULL, surface, &object.dst);
}

void Balle::rebondirBot() {
	posY = object.dst.y = 584;
	velY *= -1;
}




class Paddle
{
public:
	Paddle(int height, int width, int pX, int pY, int, char);
	~Paddle();
	//Position
	void setPosX(int pos);
	void setPosY(int pos);
	int getPosX();
	int getPosY();

	//Velocité
	int getVel();
	void setVel(int vel);

	//Mouvement
	bool moveUp(double deltaTime);
	bool moveDown(double deltaTime);

	//SDL Stuff
	void chargerPaddle();
	void blit(SDL_Surface *surface);

	int getHeight();

	char getSide();

private:
	int posX;
	int posY;
	int velDep; //Vitesse de déplacement en Y
	int w;     //Width (Largeur)
	int h;     //Height (Hauteur)
	char side; //L for left, R for right
	objectSDL object;
};

Paddle::Paddle(int height, int width, int pX, int pY, int vel, char cote)
{
	h = object.dst.h = height;
	w = object.dst.w = width;
	posX = object.dst.x = pX;
	posY = object.dst.y = pY;
	velDep = vel;
	side = cote;
}

Paddle::~Paddle()
{

}

void Paddle::setPosX(int pos)
{
	posX = pos;
}

void Paddle::setPosY(int pos)
{
	posY = pos;
}

int Paddle::getPosX()
{
	return posX;
}

int Paddle::getPosY()
{
	return posY;
}

int Paddle::getVel()
{
	return velDep;
}

void Paddle::setVel(int vel)
{
	velDep = vel;
}

bool Paddle::moveUp(double deltaTime)
{
	posY = object.dst.y = posY - velDep * deltaTime;
	if (posY <= 0)
		posY = object.dst.y = 0;

	return true;
}

bool Paddle::moveDown(double deltaTime)
{

	posY = object.dst.y = posY + velDep * deltaTime;
	if (posY >= 504)
		posY = object.dst.y = 504;
	return true;
}

void Paddle::chargerPaddle() {
	object.surface = SDL_LoadBMP("paddle.bmp");
}

void Paddle::blit(SDL_Surface* surface) {
	SDL_BlitSurface(object.surface, NULL, surface, &object.dst);
}

int Paddle::getHeight() { return h; }

char Paddle::getSide() { return side; }




class Window
{
public:
	Window(int, int, char[20]);
	~Window();

	void initWindow();

	void clean();

	void update();

	void destroy();

	SDL_Surface getScreenSurface();

	int getHeight();

	int getWidth();

private:
	int w;     //Width (Largeur)
	int h;     //Height (Hauteur)
	char name[20];
	SDL_Window* window;
	SDL_Surface* screenSurface;
};

Window::Window(int hauteur, int largeur, char nom[20])
{
	h = hauteur;
	w = largeur;
	strcpy_s(name, nom);
}

Window::~Window(){}

void Window::initWindow()
{
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	screenSurface = SDL_GetWindowSurface(window);
}

void Window::clean()
{
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 255, 255, 255));
}

void Window::update() {
	SDL_UpdateWindowSurface(window);
}

void Window::destroy() {
	SDL_DestroyWindow(window);
}

SDL_Surface Window::getScreenSurface(){	return *screenSurface; }

int Window::getHeight() { return h; }

int Window::getWidth() { return w; }



int _tmain(int argc, _TCHAR * argv[]);

//Proto
void manageEvent(SDL_Event, bool&, bool&);
//void manageKeyPress(Paddle, Paddle, double);
bool checkCollisionPlaf(Balle&, int);
bool checkCollisionPaddle(Paddle &paddle, Balle &balle);
bool checkBallOutofBounds(Balle&, Window&);
void resetBall(Balle&, Window&);


int _tmain(int argc, _TCHAR* argv[])
{
	//Initialize Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur d'initation de la vidéo..." << std::endl;
		system("pause");
		return 1;
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Erreur d'initation de l'audio..." << std::endl;
		system("pause");
		return 1;
	}

	







	//Création des objets
	Paddle pdlJoueur = Paddle(96, 16, 50, 252, 2, 'l'), pdlAI = Paddle(96, 16, 734, 252, 2, 'r');
	Window gameWindow = Window(600, 800, "Pong!");
	Balle ball = Balle(16, 392, 292, 2,0);

	//Initialisation 
	//TODO: Fonction
	pdlAI.chargerPaddle();
	pdlJoueur.chargerPaddle();
	ball.chargerBalle();
	gameWindow.initWindow();


	SDL_Surface* surface = &gameWindow.getScreenSurface();

	//Keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//Time stuff
	Uint64 NOW = SDL_GetTicks();
	Uint64 LAST = 0;
	double deltaTime = 0;

	bool quitter = false;
	bool gameGoing = false;

	while (!quitter) //Main loop
	{
		LAST = NOW;
		NOW = SDL_GetTicks();
		deltaTime = NOW - LAST;
		deltaTime /= 2; //plus bas ca le brise
		gameWindow.clean();

		// Lire événements
		SDL_Event evenement;
		while (SDL_PollEvent(&evenement) != 0)
		{
			manageEvent(evenement, quitter, gameGoing);
		}


		//Managing keypress
		//Left paddle
		if (state[SDL_SCANCODE_S]) { //Down
			pdlJoueur.moveDown(deltaTime);
		}
		if (state[SDL_SCANCODE_W]) { //UP
			pdlJoueur.moveUp(deltaTime);
		}

		//Right paddle
		if (state[SDL_SCANCODE_DOWN]) { //Down
			pdlAI.moveDown(deltaTime);
		}
		if (state[SDL_SCANCODE_UP]) { //UP
			pdlAI.moveUp(deltaTime);
		}


		if (gameGoing) //Update la position de la balle
		{
			checkCollisionPlaf(ball, gameWindow.getHeight());
			
			//Evite de faire des checks inutiles
			if (ball.getPosX() > gameWindow.getWidth() / 2) //Côté droit
				checkCollisionPaddle(pdlAI, ball);
			else
				checkCollisionPaddle(pdlJoueur, ball); //Côté gauche

			if (checkBallOutofBounds(ball, gameWindow)) {
				gameGoing = false;
				resetBall(ball, gameWindow);
			}
			ball.updatePos(deltaTime);

		}
	

		
		ball.blit(surface);
		pdlAI.blit(surface);
		pdlJoueur.blit(surface);
		
		//Update the surface 
		gameWindow.update();
	}

	gameWindow.destroy();//TODO déconstructeur?
	system("pause");
	SDL_Quit();
	return 0;
}

void manageEvent(SDL_Event evenement, bool &quitter, bool &gameGoing)
{		//User requests quit 
	if (evenement.type == SDL_QUIT)
	{
		quitter = true;
	}
	if (evenement.type == SDL_KEYDOWN) {
		if (evenement.key.keysym.sym == SDLK_SPACE && !gameGoing) {
			gameGoing = true;
		}
	}
}

/*void manageKeyPress(Paddle pdlJoueur, Paddle pdlAI, double deltaTime)
{
	
}*/


//Check for collision with top/bottom and makes the ball bounce
bool checkCollisionPlaf(Balle &ball, int h)
{
	if (ball.getPosY() < 0)
		ball.rebondirTop();
	else if (ball.getPosY() > h - 16)
		ball.rebondirBot();
	return false;
}

/*Check for collisions between a paddle and a ball
  makes the ball bounce if there's one*/
bool checkCollisionPaddle(Paddle &paddle,Balle &balle)
{
	//DON'T FORGET: IF THE Y AND X IS OK (SO THERE'S A COLLISION) REPLACE THE BALL SO IT DOESNT FUCK UP
	if (balle.getPosY() >= paddle.getPosY() && balle.getPosY() <= (paddle.getPosY() + paddle.getHeight())) //Check en Y
	{
		//Check en X
		if (paddle.getSide() == 'r') //Right paddle
		{
			if (balle.getPosX() >= paddle.getPosX() && balle.getPosX() <= paddle.getPosX() + 16) //Deuxieme condition pour éviter les trucs fucked
			{
				balle.setPosX(paddle.getPosX()); //Replace la balle pour éviter de tout briser
				balle.rebondirPaddle();
			}
		}
		else
		{
			if (balle.getPosX() <= paddle.getPosX() && balle.getPosX() >= paddle.getPosX() - 16)
			{
				balle.setPosX(paddle.getPosX()); //Replace la balle pour éviter de tout briser
				balle.rebondirPaddle();
			}
		}
	}
	return false;
}

//Return True if ball is out of screen, false if not
bool checkBallOutofBounds(Balle &balle, Window &window)
{
	return (balle.getPosX() < 0) || (balle.getPosX() > window.getWidth());
}

void resetBall(Balle &ball, Window &window) 
{
	ball.setPosX(window.getWidth()  /2 - ball.getDim());
	ball.setPosY(window.getHeight() / 2 - ball.getDim());
}



