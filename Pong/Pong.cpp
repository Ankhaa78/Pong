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

	//Velocit�
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
	Paddle(int height, int width, int pX, int pY, int);
	~Paddle();
	//Position
	void setPosX(int pos);
	void setPosY(int pos);
	int getPosX();
	int getPosY();

	//Velocit�
	int getVel();
	void setVel(int vel);

	//Mouvement
	bool moveUp(double deltaTime);
	bool moveDown(double deltaTime);

	//SDL Stuff
	void chargerPaddle();
	void blit(SDL_Surface *surface);

private:
	int posX;
	int posY;
	int velDep; //Vitesse de d�placement en Y
	int w;     //Width (Largeur)
	int h;     //Height (Hauteur)
	char side; //L for left, R for right
	objectSDL object;
};

Paddle::Paddle(int height, int width, int pX, int pY, int vel)
{
	h = object.dst.h = height;
	w = object.dst.w = width;
	posX = object.dst.x = pX;
	posY = object.dst.y = pY;
	velDep = vel;
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

Window::~Window()
{
}

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

void Window::destroy() //TODO mettre dans le destructeur
{
	SDL_DestroyWindow(window);
}

SDL_Surface Window::getScreenSurface()
{
	return *screenSurface;
}

int Window::getHeight() {
	return h;
}

//Proto
void manageEvent(SDL_Event, bool&, bool&);
//void manageKeyPress(Paddle, Paddle, double);
bool checkCollisionPlaf(Balle&, int);


int _tmain(int argc, _TCHAR* argv[])
{
	//Initialize Video
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur d'initation de la vid�o..." << std::endl;
		system("pause");
		return 1;
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Erreur d'initation de l'audio..." << std::endl;
		system("pause");
		return 1;
	}







	//Cr�ation des objets
	Paddle pdlJoueur = Paddle(96, 16, 50, 252, 2), pdlAI = Paddle(96, 16, 734, 252, 2);
	Window gameWindow = Window(600, 800, "Pong!");
	Balle ball = Balle(16, 392, 292, 2,2);

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
		deltaTime *= 0.5;
		gameWindow.clean();

		// Lire �v�nements
		SDL_Event evenement;
		while (SDL_PollEvent(&evenement) != 0)
		{
			manageEvent(evenement, quitter, gameGoing);
		}
		//manageKeyPress(pdlJoueur, pdlAI, deltaTime);

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


		if (gameGoing)
		{
			checkCollisionPlaf(ball, gameWindow.getHeight());
			ball.updatePos(deltaTime);
		}
	

		ball.blit(surface);
		pdlAI.blit(surface);
		pdlJoueur.blit(surface);


		//Update the surface 
		gameWindow.update();
	}

	gameWindow.destroy();//TODO d�constructeur
	system("pause");
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

bool checkCollisionPlaf(Balle &ball, int h)
{
	if (ball.getPosY() < 0)
		ball.rebondirTop();
	else if (ball.getPosY() > h - 16)
		ball.rebondirBot();

	return false;
}



