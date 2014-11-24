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
#include <fstream>
#include <string>


#define MODOS 1
#define DIFICULDADE 2
#define MENU 3

#include <time.h>
using namespace std;
using namespace sf;

struct RankUser{
	string nome;
	int tempo;
};

struct RankData{
	RankUser usuario[5];
};

struct ImageData{
	Sprite fundo;
	Sprite fundo_jogo;
	Sprite botao;
	Sprite botaoh;
};
struct UserData{
	string nome;
};

struct SoundData{
	Music bach;
	Music ravel;
	Music botao;
	Music enter;
};

struct TextData{
	string modos[5];
};

struct Data{
	ImageData imagens;
	SoundData musicas;
	TextData textos;
	UserData usuario;
	RankData rank;
};

void carregarImagens(ImageData &imagens)
{
	Texture *backgroundTexture = new Texture;
	backgroundTexture->loadFromFile("img/fundo-inicio.jpg");
	imagens.fundo.setTexture(*backgroundTexture);

	Texture *backgroundTexture02 = new Texture;
	backgroundTexture02->loadFromFile("img/fundo-game.jpg");
	imagens.fundo_jogo.setTexture(*backgroundTexture02);

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
	musicas.botao.openFromFile("som/botao.ogg");
	musicas.enter.openFromFile("som/metal.ogg");
}

void carregarTextos(TextData &texto, int tipo)
{
	if(tipo == MENU)
	{
		texto.modos[0]="Jogo";
		texto.modos[1]="Rank";
	}
	if(tipo == MODOS)
	{
		texto.modos[0]=" 6x6 ";
		texto.modos[1]=" 9x9 ";
		texto.modos[2]="12x12";
		texto.modos[3]="16x16";
		texto.modos[4]="Diag";
	}
	if(tipo == DIFICULDADE)
	{
		texto.modos[0]=" Fácil ";
		texto.modos[1]="Médio ";
		texto.modos[2]="Difícil";
	}

}
void telaMenu(RenderWindow &window, Data &media);
void telaModos(RenderWindow &window, Data &media);
int tratamentoTeclas(RenderWindow &window, int &j, Data &media, int tipo);
void desenharBotoes(RenderWindow &window, Data &media, int j, int indice);

void lerArquivo(int modo, Data &media)
{
	ifstream leitura;
	leitura.open("rank/6x6.rank");

	char *temp;

	temp = new char [35];


	for(int i=0; i<5; i++)
	{
		leitura.getline(temp, 35);
		media.rank.usuario[i].nome = temp;

		leitura.getline(temp, 35);
		media.rank.usuario[i].tempo = atoi(temp);
	}

	delete temp;
	leitura.close();
}

void escreverArquivo(int modo, Data &media)
{
	ofstream escrita;
	escrita.open("rank/6x62.rank");

	for(int i=0; i<5; i++)
	{
		escrita << media.rank.usuario[i].nome << endl;
		escrita << media.rank.usuario[i].tempo << endl;
	}

	escrita.close();
}

bool validarLinha(int **m, int tamanho, int linha, int valor)
{
	for(int i=0; i<tamanho; i++)
	{
		if(m[linha][i]==valor)
		{
			return false;
		}
	}
	return true;
}

bool validarColuna(int **m, int tamanho, int coluna, int valor)
{
	for(int i=0; i<tamanho; i++)
	{
		if(m[i][coluna]==valor)
		{
			return false;
		}
	}
	return true;
}

bool validarQuadrado(int **m, int tamanho,int linha, int coluna, int valor)
{
	int quadx, quady;

	switch(tamanho)
	{
	case 6:
		quadx=3;
		quady=2;
		break;
	case 9:
		quadx=3;
		quady=3;
		break;
	case 12:
		quadx=4;
		quady=3;
		break;
	case 16:
		quadx=4;
		quady=4;
		break;
	default:
		quadx=3;
		quady=2;
		break;

	}

	int yini=linha/quady;
	int xini=coluna/quadx;


	yini=(yini*quady);
	xini=(xini*quadx);

	for(int i=yini; i<yini+quady; i++)
	{
		for(int j=xini; j<xini+quadx;j++)
		{
			if(m[i][j]==valor)
			{
				return false;
			}
		}
	}
	return true;

}

bool redefinir(bool bol[16], int t)
{
	for(int i=0; i<t; i++)
	{
		if(bol[i])
		{
			return false;
		}
	}
	return true;
}

int escolherDificuldade(RenderWindow &window, Data &media)
{
	int j=0;

	carregarTextos(media.textos,DIFICULDADE);

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
						media.musicas.enter.play();
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

int lerTile(RenderWindow &window, int tamanho,int indice, int **mapa)
{
	Event event;
	string valor="";

	while(window.isOpen())
	{
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
					if( (Keyboard::isKeyPressed(Keyboard::Return)) )
					{
						int val=atoi(valor.c_str());
						if(validarColuna(mapa,tamanho,indice%tamanho,val) && validarLinha(mapa,tamanho,indice/tamanho,val) && validarQuadrado(mapa,tamanho,indice/tamanho,indice%tamanho, val))
						{
							return val;
						}
						else
						{
							return 0;
						}

					}
				break;
				// In event loop...
				case Event::TextEntered:
					// Handle ASCII characters only
					if(event.text.unicode=='\b' && valor.size()>0)
					{
						valor.erase(valor.size()-1,1);
					}
					else if(event.text.unicode < 128 && valor.size()<3)
					{
						valor+= static_cast<char>(event.text.unicode);
					}
				break;
				// we don't process other types of events
				default:
					break;
			}
		}
	}
}

void selecionarTile(RenderWindow &window, int tamanho,int &indice, int **mapa,Data &media, int &erros)
{
	if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		indice--;
		if(indice<0)
		{
			indice=tamanho*tamanho-1;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
		indice++;
		if(indice>tamanho*tamanho-1)
		{
			indice=0;
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
		indice-=tamanho;
		if(indice<0)
		{
			indice=((tamanho*tamanho))-tamanho+((indice+tamanho)%tamanho);
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		indice+=tamanho;
		if(indice>tamanho*tamanho-1)
		{
			indice=indice%tamanho;
		}
	}
	if(Keyboard::isKeyPressed(Keyboard::Return))
	{
		int val=lerTile(window, tamanho, indice, mapa);
		if(val>0)
		{
			mapa[indice/tamanho][indice%tamanho]=val;
		}
		else
		{
			erros++;
			media.musicas.enter.play();
		}
	}
}

string intTOstring(int number)
{
	if (number == 0)
	        return "0";
	    string temp="";
	    string returnvalue="";
	    while (number>0)
	    {
	        temp+=number%10+48;
	        number/=10;
	    }
	    for (int i=0;i<temp.length();i++)
	        returnvalue+=temp[temp.length()-i-1];
	    return returnvalue;
}

void desenharMapa(RenderWindow &window,int **mapa, int tamanho, int indice, int tempo, int errados)
{
	int TAMANHOTILE, TAMANHOFONTE;

	if(tamanho==16)
	{
		TAMANHOTILE=20;
		TAMANHOFONTE=10;
	}
	else
	{
		TAMANHOTILE=40;
		TAMANHOFONTE=20;
	}

	Font font;
	font.loadFromFile("font/sansation.ttf");


	Text relogio(intTOstring(tempo), font, 20);
	relogio.setPosition(700, 40);
	relogio.setColor(Color(80, 80, 80));

	window.draw(relogio);

	Text erros(intTOstring(errados), font, 20);
	erros.setPosition(700, 550);
	erros.setColor(Color::Red);

	window.draw(erros);

	RectangleShape quadrado;
	quadrado.setSize(Vector2f(TAMANHOTILE, TAMANHOTILE));
	quadrado.setOutlineThickness(1);
	quadrado.setOutlineColor(sf::Color::Black);
	quadrado.setFillColor(sf::Color::Transparent);

	for(int i=0; i<tamanho; i++)
	{
		for(int j=0; j<tamanho; j++)
		{
			quadrado.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE,(300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE);
			window.draw(quadrado);

			Text tile(intTOstring(mapa[i][j]), font, TAMANHOFONTE);
			tile.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE+TAMANHOFONTE/2, (300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE+TAMANHOFONTE/2);
			tile.setColor(Color(80, 80, 80));

			window.draw(tile);

			if(indice%tamanho==j && indice/tamanho==i)
			{
				quadrado.setFillColor(Color(255,0,0,100));
				quadrado.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE,(300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE);
				window.draw(quadrado);
				quadrado.setFillColor(sf::Color::Transparent);
			}
		}

	}

}

void preencherMapa(int **m, int tamanho)
{
	int val[16];
	bool valbol[16];

	for(int i=0; i<16; i++)
	{
		val[i]=i+1;
		valbol[i]=true;
	}

	int aux=(rand()%tamanho);

	for(int i = 0; i < tamanho; i++)
	{
		for(int j = 0; j < tamanho; j++)
		{
			if(valbol[aux])
			{
				if(validarLinha(m,tamanho,i,val[aux])&& validarColuna(m,tamanho,j,val[aux]) && validarQuadrado(m,tamanho,i,j,val[aux]))
				{
					m[i][j]=val[aux];
					for(int i=0; i<16; i++)
					{
						valbol[i]=true;
					}
					aux=(rand()%tamanho);
				}
				else
				{
					valbol[aux]=false;
					j--;
					aux=(aux+1)%tamanho;
				}
			}
			else
			{
				aux=(aux+1)%tamanho;
				j--;
				if(redefinir(valbol,tamanho))
				{
					for(int x=0; x<16; x++)
					{
						valbol[x]=true;
						m[i][x]=0;
					}
					j=-1;
				}
			}
		}
	}
}

void adicionarDificuldade(int **mapa, int tamanho, int dificuldade)
{
	int porcentagem;
	switch (dificuldade)
	{
	case 0:
		porcentagem=2;
		break;
	case 1:
		porcentagem=3;
		break;
	case 2:
		porcentagem=10;
		break;
	default:
		porcentagem=2;
		break;
	}
	int itens =(tamanho*tamanho);
	int permanecer=itens/porcentagem;
	int apagar = itens-permanecer;

	int linha=rand()%tamanho;
	int coluna=rand()%tamanho;
	for(int i=0; i<apagar; i++)
	{
		if(mapa[linha][coluna] > 0)
		{
			mapa[linha][coluna]=0;
		}
		else
		{
			linha=rand()%tamanho;
			coluna=rand()%tamanho;
			i--;
		}
	}


}

void telaTamanho(RenderWindow &window, Data &media, int dificuldade, int tamanho)
{

	if(tamanho==10)
	{
		tamanho=9;
	}
	int **m = new int*[256];
	m[0] = new int[256*256];

	for (int i=1; i<256; i++)
	{
		m[i] = m[i-1]+256;
	}

	preencherMapa(m,tamanho);
	adicionarDificuldade(m,tamanho,dificuldade);

	int erros=0;

	time_t tempo_inicial, tempo_atual, tempo_decorrido;

	time(&tempo_inicial);

	int indice = 0;
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
						telaMenu(window,media);
					}
					selecionarTile(window, tamanho,indice, m, media, erros);
				break;

				// we don't process other types of events
				default:
					break;
			}

		}
		time(&tempo_atual);

		tempo_decorrido=(tempo_atual-tempo_inicial);
		window.clear();

		window.draw(media.imagens.fundo_jogo);
		desenharMapa(window, m , tamanho, indice, tempo_decorrido, erros);

		window.display();


	}
}

void telaRank(RenderWindow &window, Data &media)
{
	lerArquivo(1,media);
	escreverArquivo(1,media);
}

int tratamentoTeclas(RenderWindow &window, int &j, Data &media, int tipo)
{
	int indice=0;

	if(tipo==MENU)
	{
		indice=1;
	}

	if(tipo==MODOS)
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
		media.musicas.botao.play();
		j--;
		if(j<0)
		{
			j=indice;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		media.musicas.botao.play();
		j++;
		if(j>indice)
		{
			j=0;
		}
	}

	if(Keyboard::isKeyPressed(Keyboard::Escape))
	{
		media.musicas.ravel.stop();
		telaMenu(window, media);
	}

	if(Keyboard::isKeyPressed(Keyboard::Return))
	{
		media.musicas.enter.play();
		if(tipo==MENU)
		{
			switch(j)
			{
				case 0:
					telaModos(window,media);
				break;
				case 1:
					telaRank(window, media);
				break;
			}
		}

		if(tipo==MODOS)
		{
			switch(j)
			{
				case 0:
					telaTamanho(window, media, escolherDificuldade(window, media),6);
				break;
				case 1:
					telaTamanho(window, media, escolherDificuldade(window, media),9);
				break;
				case 2:
					telaTamanho(window, media, escolherDificuldade(window, media),12);
				break;
				case 3:
					telaTamanho(window, media, escolherDificuldade(window, media),16);
				break;
				case 4:
					telaTamanho(window, media, escolherDificuldade(window, media),10);
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

void telaModos(RenderWindow &window, Data &media)
{
	carregarTextos(media.textos,MODOS);
	int j=0;

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
					tratamentoTeclas(window, j, media, MODOS);
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

void telaMenu(RenderWindow &window, Data &media)
{
	carregarTextos(media.textos,MENU);
	int j=0;

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

		desenharBotoes(window, media , j,2);

		window.display();

	}
}

void telaUsuario(RenderWindow &window, Data &media)
{
	RectangleShape quadrado;
	quadrado.setSize(Vector2f(600,50));
	quadrado.setOutlineThickness(0);
	quadrado.setOutlineColor(Color::Black);
	quadrado.setFillColor(Color(0,0,0,100));
	quadrado.setPosition(100,290);

	Font font;
	font.loadFromFile("font/sansation.ttf");

	media.usuario.nome="";

	std::string str;

	Text desc("Digite seu nome", font, 30);
	desc.setPosition(300, 230);
	desc.setColor(Color(80, 80, 80));

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
					if( (Keyboard::isKeyPressed(Keyboard::Return)) )
					{
						telaMenu(window,media);
					}

				break;

				// In event loop...
				case Event::TextEntered:
				    // Handle ASCII characters only
					if(event.text.unicode=='\b' && media.usuario.nome.size()>0)
					{
						media.usuario.nome.erase(media.usuario.nome.size()-1,1);
					}
					else if(event.text.unicode < 128 && media.usuario.nome.size()<35)
				    {
				    		media.usuario.nome+= static_cast<char>(event.text.unicode);
				    }
				break;
				// we don't process other types of events
				default:
					break;
			}
		}


		window.draw(media.imagens.fundo);




		Text nome(media.usuario.nome, font, 30);
		nome.setPosition(400 - nome.getLocalBounds().width/2, 300);
		nome.setColor(Color(255,255,255));

		window.draw(desc);
		window.draw(quadrado);
		window.draw(nome);
		window.display();


	}

}

int main()
{
	srand(time(NULL));

	// Create the main window
	RenderWindow window(VideoMode(800, 600), "Sudoku", Style::Default);
	window.setVerticalSyncEnabled(true);

	Data media;

	carregarImagens(media.imagens);
	carregarSons(media.musicas);
	carregarTextos(media.textos,MODOS);

	telaUsuario(window, media);
	telaMenu(window, media);


	return 0;
}
