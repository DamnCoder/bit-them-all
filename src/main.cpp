/**
 * @file main.cpp
 *
 * Punto de entrada a la aplicacion.
 *
 *  @date 27/10/2009
 *  @author Jorge López González
 */

#include "app/BitThemAllApp.h"

int main(){

	app::CApplication* myGame = new app::CBitThemAllApp();

	// Iniciamos el juego//
	if(!myGame->init())
		return 0;

	// Damos paso al bucle principal del juego
	myGame->run();

	// Ejecutamos las rutinas para el fin del juego
	myGame->end();

	delete myGame;

#ifdef _DEBUG
	//std::cout<<"Aplicacion termina con exito!\n";
#endif
	return 0;

}
