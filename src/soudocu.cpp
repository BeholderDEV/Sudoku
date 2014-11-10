//============================================================================
// Name        : soudocu.cpp
// Author      : Alisson
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define TEXTOMENU 1
#define TEXTODIFICULDADE 2
#define MENU 1
#define DIFICULDADE 2

#include <time.h>
using namespace std;
using namespace sf;

struct ImageData{
	Sprite fundo;
	Sprite botao;
	Sprite botaoh;
};

struct SoundData{
	Music bach;
	Music ravel;
};

struct TextData{
	string modos[5];
};

struct Data{
	ImageData imagens;
	SoundData musicas;
	TextData textos;
};

void carregarImagens(ImageData &imagens)
{
	Texture *backgroundTexture = new Texture;
	backgroundTexture->loadFromFile("img/fundo-inicio.jpg");
	imagens.fundo.setTexture(*backgroundTexture);

	Texture *botaoTexture = new Texture;
	botaoTexture->loadFromFile("img/botao.png");
	imagens.botao.setTexture(*botaoTexture);

	Texture *botaohTexture = new Texture;
	botaohTexture->loadFromFile("img/botaoh.png");
	imagens.botaoh.setTexture(*botaohTexture);
}

void carregarSons(SoundData &musicas)
{
	musicas.bach.openFromFile("som/bach.ogg");
	musicas.ravel.openFromFile("som/ravel.ogg");
}

void carregarTextos(TextData &texto, int tipo)
{
	if(tipo == TEXTOMENU)
	{
		texto.modos[0]=" 6x6 ";
		texto.modos[1]=" 9x9 ";
		texto.modos[2]="12x12";
		texto.modos[3]="16x16";
		texto.modos[4]="Diag";
	}
	if(tipo == TEXTODIFICULDADE)
	{
		texto.modos[0]=" Fácil ";
		texto.modos[1]="Médio ";
		texto.modos[2]="Difícil";
	}

}

void principal(RenderWindow &window);
int tratamentoTeclas(RenderWindow &window, int &j, Data &media, int tipo);
void desenharBotoes(RenderWindow &window, Data &media, int j, int indice);

int escolherDificuldade(RenderWindow &window, Data &media)
{
	int j=0;

	carregarTextos(media.textos,TEXTODIFICULDADE);

	window.clear();

	window.draw(media.imagens.fundo);

	window.display();

	while(window.isOpen())
	{

		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					tratamentoTeclas(window, j, media, DIFICULDADE);
					if(Keyboard::isKeyPressed(Keyboard::Return))
					{
						return j;
					}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}

		window.clear();

		window.draw(media.imagens.fundo);

		desenharBotoes(window, media , j, 3);

		window.display();

	}
}

void telaSeis(RenderWindow &window, Data &media, int dificuldade)
{
	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
						{
							media.musicas.ravel.stop();
							principal(window);
						}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}



		window.clear();

		window.draw(media.imagens.fundo);

		window.display();


	}
}

void telaNove(RenderWindow &window, Data &media, int dificuldade)
{
	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
						{
							media.musicas.ravel.stop();
							principal(window);
						}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}



		window.clear();

		window.draw(media.imagens.fundo);

		window.display();


	}
}

void telaDoze(RenderWindow &window, Data &media, int dificuldade)
{
	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
						{
							media.musicas.ravel.stop();
							principal(window);
						}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}



		window.clear();

		window.draw(media.imagens.fundo);

		window.display();


	}
}

void telaDeze(RenderWindow &window, Data &media, int dificuldade)
{
	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
						{
							media.musicas.ravel.stop();
							principal(window);
						}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}



		window.clear();

		window.draw(media.imagens.fundo);

		window.display();


	}
}

void telaDiag(RenderWindow &window, Data &media, int dificuldade)
{
	while(window.isOpen())
	{
		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
						{
							media.musicas.ravel.stop();
							principal(window);
						}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}



		window.clear();

		window.draw(media.imagens.fundo);

		window.display();


	}
}

int tratamentoTeclas(RenderWindow &window, int &j, Data &media, int tipo)
{
	int indice=0;

	if(tipo==MENU)
	{
		indice=4;
	}

	if(tipo==DIFICULDADE)
	{
		indice=2;
	}


	if( (Keyboard::isKeyPressed(Keyboard::RAlt))&& (Keyboard::isKeyPressed(Keyboard::F4)))
	{
		window.close();
	}

	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
		j--;
		if(j<0)
		{
			j=indice;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		j++;
		if(j>indice)
		{
			j=0;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Escape))
	{
		media.musicas.ravel.stop();
		principal(window);
	}

	if(Keyboard::isKeyPressed(Keyboard::Return))
	{

			if(tipo==MENU)
			{
				switch(j)
				{
					case 0:
						telaSeis(window, media, escolherDificuldade(window, media));
					break;
					case 1:
						telaNove(window, media, escolherDificuldade(window, media));
					break;
					case 2:
						telaDoze(window, media, escolherDificuldade(window, media));
					break;
					case 3:
						telaDeze(window, media, escolherDificuldade(window, media));
					break;
					case 4:
						telaDiag(window, media, escolherDificuldade(window, media));
					break;
				}
			}
			if(tipo==DIFICULDADE)
			{
				return j;
			}
	}
}


void desenharTexto(RenderWindow &window, TextData texto, int indice)
{
	Font font;
	font.loadFromFile("font/sansation.ttf");

		int x=375, y=185;

		for(int i=0; i<indice; i++)
		{
			Text temp(texto.modos[i], font, 30);
			temp.setPosition(x, y);
			temp.setColor(Color(80, 80, 80));

			window.draw(temp);

			y+=80;
		}

}

void desenharBotoes(RenderWindow &window, Data &media, int j, int indice)
{

	int x=350, y=175;

	for(int i=0; i<indice; i++)
	{
		if(i!=j)
		{
			media.imagens.botao.setPosition(x,y);
			window.draw(media.imagens.botao);
		}
		else
		{
			media.imagens.botaoh.setPosition(x,y);
			window.draw(media.imagens.botaoh);
		}

		y+=80;
	}

	desenharTexto(window, media.textos, indice);


}

void principal(RenderWindow &window)
{
	int j=0;

	Data media;

	carregarImagens(media.imagens);
	carregarSons(media.musicas);
	carregarTextos(media.textos,TEXTOMENU);

	window.clear();

	window.draw(media.imagens.fundo);

	window.display();

	media.musicas.ravel.play();

	while(window.isOpen())
	{

		Event event;
		// while there are pending events...

		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					tratamentoTeclas(window, j, media, MENU);
				break;

				// we don't process other types of events
				default:
					break;
			}

		}

		window.clear();

		window.draw(media.imagens.fundo);

		desenharBotoes(window, media , j,5);

		window.display();

	}
}

int main()
{
	srand(time(NULL));

	// Create the main window
	RenderWindow window(VideoMode(800, 600), "Sudoku", Style::Default);
	window.setVerticalSyncEnabled(true);

	principal(window);


	return 0;
}
