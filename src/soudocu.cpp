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
#include <time.h>

#define MODOS 1
#define DIFICULDADE 2
#define MENU 3

using namespace std;
using namespace sf;

struct Entradas{
	int val;
	string valor;
	Text tile;
};

struct Dicatype{
	int dica;
	bool trocar;
};
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
	Sprite load;
};
struct UserData{
	string nome;
};

struct SoundData{
	Music bach;
	Music ravel;
	Music botao;
	Music enter;
	Music erro;
};

struct TextData{
	string modos[5];
	string dicas[10];
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

	Texture *laodTexture = new Texture;
	laodTexture->loadFromFile("img/load.png");
	imagens.load.setTexture(*laodTexture);
}

void carregarSons(SoundData &musicas)
{
	musicas.bach.openFromFile("som/bach.ogg");
	musicas.ravel.openFromFile("som/ravel.ogg");
	musicas.botao.openFromFile("som/botao.ogg");
	musicas.enter.openFromFile("som/metal.ogg");
	musicas.erro.openFromFile("som/erro.ogg");
}

void carregarTextos(TextData &texto, int tipo)
{
	if(tipo == MENU)
	{
		texto.modos[0]="Jogo";
		texto.modos[1]="Rank";

		texto.dicas[0]="� um pa�s da europa";
		texto.dicas[1]="O nivel de dificuldade pode te dar bonus de tempo";
		texto.dicas[2]="O nivel de dificuldade pode melhorar sua coloca��o";
		texto.dicas[3]="ESC para retornar ao menu";
		texto.dicas[4]="Seu idioma � o Holand�s";
		texto.dicas[5]="ESC para retornar ao menu";
		texto.dicas[6]="Menos erros = Menos tempo";
		texto.dicas[7]="O nivel de dificuldade pode melhorar sua coloca��o";
		texto.dicas[8]="Terra das flores e dos moinhos de vento";
		texto.dicas[9]="Venceu o Brasil na copa de 2010";
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
		texto.modos[0]=" F�cil ";
		texto.modos[1]="M�dio ";
		texto.modos[2]="Dif�cil";
	}


}
void telaMenu(RenderWindow &window, Data &media);
void telaModos(RenderWindow &window, Data &media);
int tratamentoTeclas(RenderWindow &window, int &j, Data &media, int tipo);
void desenharBotoes(RenderWindow &window, Data &media, int j, int indice);

void lerArquivo(int tamanho, Data &media)
{
	ifstream leitura;

	switch(tamanho)
	{
	case 6:
		leitura.open("rank/6x6.rank");
		break;
	case 9:
		leitura.open("rank/9x9.rank");
		break;
	case 12:
		leitura.open("rank/12x12.rank");
		break;
	case 16:
		leitura.open("rank/16x16.rank");
		break;
	case 10:
		leitura.open("rank/diagxdiag.rank");
		break;

	}

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

void escreverArquivo(int tamanho, Data &media, bool diag)
{
	if(diag)
	{
		tamanho=10;
	}
	ofstream escrita;

	switch(tamanho)
		{
		case 6:
			escrita.open("rank/6x6.rank");
			break;
		case 9:
			escrita.open("rank/9x9.rank");
			break;
		case 12:
			escrita.open("rank/12x12.rank");
			break;
		case 16:
			escrita.open("rank/16x16.rank");
			break;
		case 10:
			escrita.open("rank/diagxdiag.rank");
			break;

		}

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
bool validarDiagonal(int **m, int tamanho,int linha, int coluna, int valor)
{
	if(linha==coluna || linha+coluna==tamanho-1)
	{
		for(int i=0; i<tamanho;i++)
		{
			for(int j=0; j<tamanho;j++)
			{
				if((i==j && linha==coluna) && (i!=linha && j!=coluna))
				{
					if(m[i][j] == valor)
					{
						return false;
					}
				}
				if((i+j==tamanho-1 && linha+coluna==tamanho-1) && (i!=linha && j!=coluna))
				{
					if(m[i][j] == valor)
					{
						return false;
					}
				}
			}
		}
		return true;
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

void selecionarTile(RenderWindow &window, int tamanho,int &indice, int **mapa, bool **bloc,Data &media, int &erros, bool diag, Entradas &entrada)
{
	if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		indice--;
		if(indice<0)
		{
			indice=tamanho*tamanho-1;
		}
		entrada.valor="";
	}

	if(Keyboard::isKeyPressed(Keyboard::Right))
	{
		indice++;
		if(indice>tamanho*tamanho-1)
		{
			indice=0;
		}
		entrada.valor="";
	}
	if(Keyboard::isKeyPressed(Keyboard::Up))
	{
		indice-=tamanho;
		if(indice<0)
		{
			indice=((tamanho*tamanho))-tamanho+((indice+tamanho)%tamanho);
		}
		entrada.valor="";
	}

	if(Keyboard::isKeyPressed(Keyboard::Down))
	{
		indice+=tamanho;
		if(indice>tamanho*tamanho-1)
		{
			indice=indice%tamanho;
		}
		entrada.valor="";
	}
	if(!bloc[indice/tamanho][indice%tamanho] && entrada.valor.size()<=1)
	{
		if(Keyboard::isKeyPressed(Keyboard::Num0) || Keyboard::isKeyPressed(Keyboard::Numpad0))
		{
			entrada.valor+='0';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1))
		{
			entrada.valor+='1';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2))
		{
			entrada.valor+='2';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num3) || Keyboard::isKeyPressed(Keyboard::Numpad3))
		{
			entrada.valor+='3';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num4) || Keyboard::isKeyPressed(Keyboard::Numpad4))
		{
			entrada.valor+='4';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num5) || Keyboard::isKeyPressed(Keyboard::Numpad5))
		{
			entrada.valor+='5';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num6) || Keyboard::isKeyPressed(Keyboard::Numpad6))
		{
			entrada.valor+='6';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num7) || Keyboard::isKeyPressed(Keyboard::Numpad7))
		{
			entrada.valor+='7';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num8) || Keyboard::isKeyPressed(Keyboard::Numpad8))
		{
			entrada.valor+='8';
		}
		if(Keyboard::isKeyPressed(Keyboard::Num9) || Keyboard::isKeyPressed(Keyboard::Numpad9))
		{
			entrada.valor+='9';
		}
	}
	if(!bloc[indice/tamanho][indice%tamanho])
	{
		if(Keyboard::isKeyPressed(Keyboard::Return))
		{
			entrada.val = atoi(entrada.valor.c_str());

			bool valido;
			if(diag)
			{
				valido=validarColuna(mapa,tamanho,indice%tamanho,entrada.val) && validarLinha(mapa,tamanho,indice/tamanho,entrada.val) && validarQuadrado(mapa,tamanho,indice/tamanho,indice%tamanho, entrada.val) && validarDiagonal(mapa,tamanho,indice/tamanho,indice%tamanho, entrada.val);
			}
			else
			{
				valido=validarColuna(mapa,tamanho,indice%tamanho,entrada.val) && validarLinha(mapa,tamanho,indice/tamanho,entrada.val) && validarQuadrado(mapa,tamanho,indice/tamanho,indice%tamanho, entrada.val);
			}

			if(entrada.val>0 && entrada.val<=tamanho && valido)
			{
				mapa[indice/tamanho][indice%tamanho]=entrada.val;
				entrada.valor="";
			}
			else
			{
				entrada.valor="";
				erros++;
				media.musicas.erro.play();
			}
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
	    {
	    	 returnvalue+=temp[temp.length()-i-1];
	    }

	    return returnvalue;
}

void desenharMapa(RenderWindow &window, TextData texto, int **mapa,bool **bloc, int tamanho, int indice, int tempo, int errados, int coeficienteDificuldade, Dicatype &dica)
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
	int TAMANHOTILE, TAMANHOFONTE;

	if(tamanho==16)
	{
		TAMANHOTILE=30;
		TAMANHOFONTE=15;
	}
	else
	{
		TAMANHOTILE=40;
		TAMANHOFONTE=20;
	}

	Font font;
	font.loadFromFile("font/sansation.ttf");

	string tempopass= "tempo: "+ intTOstring(tempo);
	Text relogio(tempopass, font, 20);
	relogio.setPosition(650, 40);
	relogio.setColor(Color(80, 80, 80));

	window.draw(relogio);

	string errostext ="Erros: " + intTOstring(errados);
	Text erros(errostext, font, 20);
	erros.setPosition(680, 550);
	erros.setColor(Color::Red);

	string soma= "+ ";
	soma += intTOstring(errados*coeficienteDificuldade);
	Text somado(soma, font, 20);
	somado.setPosition(685, 70);
	somado.setColor(Color::Red);

	if(tempo%10==0)
	{
		if(dica.trocar)
		{
			int aux=dica.dica;
			while(aux==dica.dica)
			{
				dica.dica = rand()%10;
			}
			dica.trocar=false;
		}
	}
	else
	{
		dica.trocar=true;
	}

	string text="Dica: ";
	text+=texto.dicas[dica.dica];
	Text dicas(text, font, 20);
	//dicas.setPosition(0, 550);
	dicas.setPosition(400 - dicas.getLocalBounds().width/2, 550);
	dicas.setColor(Color::Blue);

	window.draw(erros);
	window.draw(somado);
	window.draw(dicas);

	RectangleShape quadradog;
	quadradog.setSize(Vector2f(TAMANHOTILE*quadx, TAMANHOTILE*quady));
	quadradog.setOutlineThickness(2);
	quadradog.setOutlineColor(sf::Color::Black);
	//quadradog.setOutlineColor(sf::Color::Red);
	quadradog.setFillColor(sf::Color::Transparent);

	RectangleShape quadradofull;
	quadradofull.setSize(Vector2f(TAMANHOTILE*tamanho, TAMANHOTILE*tamanho));
	quadradofull.setOutlineThickness(4);
	quadradofull.setOutlineColor(sf::Color::Black);
	//quadradofull.setOutlineColor(sf::Color::Red);
	quadradofull.setFillColor(sf::Color::Transparent);
	quadradofull.setPosition((400-tamanho/2*TAMANHOTILE), (300-tamanho/2*TAMANHOTILE));

	RectangleShape quadrado;
	quadrado.setSize(Vector2f(TAMANHOTILE, TAMANHOTILE));
	quadrado.setOutlineThickness(1);
	quadrado.setOutlineColor(Color(200,200,200));
	quadrado.setFillColor(sf::Color::Transparent);

	for(int i=0; i<tamanho; i++)
	{
		for(int j=0; j<tamanho; j++)
		{
			quadrado.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE,(300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE);
			window.draw(quadrado);
			string texto;
			if(mapa[i][j]>0)
			{
				texto = intTOstring(mapa[i][j]);
			}
			else
			{
				texto="";
			}
			if(indice%tamanho==j && indice/tamanho==i)
			{
				quadrado.setFillColor(Color(0,255,0,210));
				quadrado.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE,(300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE);
				window.draw(quadrado);
				quadrado.setFillColor(sf::Color::Transparent);
			}

			Text tile(texto, font, TAMANHOFONTE);
			tile.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE+TAMANHOFONTE/2, (300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE+TAMANHOFONTE/2);
			if(bloc[i][j])
			{
				tile.setColor(Color(80, 80, 80));
			}
			else
			{
				tile.setColor(Color::Blue);
			}


			window.draw(tile);
		}
	}

	for(int i=0; i<tamanho; i++)
	{
		for(int j=0; j<tamanho; j++)
		{
			if(i%quady==0 && j%quadx==0)
			{
				quadradog.setPosition((400-tamanho/2*TAMANHOTILE)+j*TAMANHOTILE,(300-tamanho/2*TAMANHOTILE)+i*TAMANHOTILE);
				window.draw(quadradog);
			}
		}
	}
	window.draw(quadradofull);
}

void telaCarregamento(RenderWindow &window, Data &media)
{
	media.imagens.load.setOrigin(64,64);
	media.imagens.load.setPosition(400,300);
	media.imagens.load.rotate(5);
	window.draw(media.imagens.fundo_jogo);

	window.draw(media.imagens.load);

	window.display();
}
void preencherDiagonais(int **m, int tamanho)
{
	int val[16];
	bool valbol[16];

	for(int i=0; i<16; i++)
	{
		val[i]=i+1;
		valbol[i]=true;
	}

	int aux=(rand()%tamanho);

	int resets=0;

	for(int i=0; i<tamanho; i++)
	{
		for(int j=0; j<tamanho; j++)
		{

			if(i==j || i+j==tamanho-1)
			{
				if(valbol[aux])
				{
					bool valido;

					valido = validarLinha(m,tamanho,i,val[aux])&& validarColuna(m,tamanho,j,val[aux]) && validarQuadrado(m,tamanho,i,j,val[aux]) && validarDiagonal(m,tamanho,i,j,val[aux]);

					if(valido)
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
						resets++;
						if(resets>1000)
						{
							i=-1;
							j=-1;
							resets=0;
							for(int i2 = 0; i2 < tamanho; i2++)
							{
								for(int j2 = 0; j2 < tamanho; j2++)
								{
									m[i2][j2]=0;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}

void preencherMapa(int **m, int &tamanho, RenderWindow &window, Data &media, bool &diag)
{
	time_t tempo_inicial, tempo_atual, tempo_decorrido;

	time(&tempo_inicial);

	int val[16];
	bool valbol[16];
	if(tamanho==10)
	{
		tamanho=9;
		diag=true;
	}
	for(int i=0; i<16; i++)
	{
		val[i]=i+1;
		valbol[i]=true;
	}

	int aux=(rand()%tamanho);
	int resets=0;
	if(diag)
	{
		preencherDiagonais(m, tamanho);
	}
	for(int i = 0; i < tamanho; i++)
	{
		for(int j = 0; j < tamanho; j++)
		{
			time(&tempo_atual);
			tempo_decorrido=(tempo_atual-tempo_inicial);

			if(tempo_decorrido%300==0)
			{
				telaCarregamento(window, media);
			}

			if(valbol[aux])
			{
				bool valido;
				if(diag)
				{
					valido=validarLinha(m,tamanho,i,val[aux])&& validarColuna(m,tamanho,j,val[aux]) && validarQuadrado(m,tamanho,i,j,val[aux]) && validarDiagonal(m,tamanho,i,j,val[aux]);
				}
				else
				{
					valido=validarLinha(m,tamanho,i,val[aux])&& validarColuna(m,tamanho,j,val[aux]) && validarQuadrado(m,tamanho,i,j,val[aux]);
				}

				if(valido)
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
					resets++;
					j=-1;
					if(resets>1000)
					{
						i=-1;
						j=-1;
						resets=0;
						for(int i2 = 0; i2 < tamanho; i2++)
						{
							for(int j2 = 0; j2 < tamanho; j2++)
							{
								m[i2][j2]=0;
							}
						}
						if(diag)
						{
							preencherDiagonais(m, tamanho);
						}
						break;
					}
				}
			}
		}
	}

}

void adicionarDificuldade(int **mapa, bool **b, int tamanho, int dificuldade)
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
			b[linha][coluna]=false;
		}
		else
		{
			linha=rand()%tamanho;
			coluna=rand()%tamanho;
			i--;
		}
	}
}

bool testarVenceu(int **m, int tamanho)
{
	for(int i=0; i<tamanho; i++)
	{
		for(int j=0; j<tamanho; j++)
		{
			if(m[i][j]==0)
			{
				return false;
			}
		}
	}
	return true;
}

bool validarRank(int tamanho,int tempo_decorrido,Data &media, bool diag)
{
	if(diag)
	{
		lerArquivo(10, media);
	}
	else
	{
		lerArquivo(tamanho, media);
	}

	for(int i=0;i<5;i++)
	{
		if(media.rank.usuario[i].tempo > tempo_decorrido)
		{
			for(int j=4; j>i; j--)
			{
				media.rank.usuario[j].tempo=media.rank.usuario[j-1].tempo;
				media.rank.usuario[j].nome=media.rank.usuario[j-1].nome;
			}
			media.rank.usuario[i].tempo=tempo_decorrido;
			media.rank.usuario[i].nome=media.usuario.nome;
			escreverArquivo(tamanho, media, diag);
			return true;
		}
	}
	return false;
}

void desenharRank(RenderWindow &window, Data &media,int x, int y, int tamanho)
{
	Font font;
	font.loadFromFile("font/sansation.ttf");

	window.draw(media.imagens.fundo_jogo);

	string tam =intTOstring(tamanho);
	string titulo;
	int newx;
	if(tamanho!=10)
	{
		titulo = tam + " x " +  tam ;
		if(tamanho<10)
		{
			newx=x+70;
		}
		else
		{
			newx=x+60;
		}
	}
	else
	{
		titulo = "Diagonal";
		newx=x+30;
	}

	Text title(titulo, font, 50);
	title.setPosition(newx, y);
	title.setColor(Color(0,128,0));

	Text setaesq("<", font, 100);
	setaesq.setPosition(30, 250);
	setaesq.setColor(Color(0,128,0));

	Text setadir(">", font, 100);
	setadir.setPosition(700, 250);
	setadir.setColor(Color(0,128,0));

	window.draw(setaesq);
	window.draw(setadir);
	window.draw(title);

	y+=80;
	for(int i=0; i<5; i++)
	{
		Text nome(media.rank.usuario[i].nome, font, 30);
		nome.setPosition(x, y);
		nome.setColor(Color(80, 80, 80));

		string time= "";
		time = intTOstring(media.rank.usuario[i].tempo);
		Text temp(time, font, 30);
		temp.setPosition(x+ 200, y);
		temp.setColor(Color(80, 80, 80));


		window.draw(nome);
		window.draw(temp);
		y+=60;
	}
	window.display();
}

void telaRank(RenderWindow &window, Data &media)
{
	int modo=0;

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
					if( (Keyboard::isKeyPressed(Keyboard::Left)) )
					{
						modo--;
						if(modo<0)
						{
							modo=4;
						}
					}
					if( (Keyboard::isKeyPressed(Keyboard::Right)) )
					{
						modo++;
						if(modo>4)
						{
							modo=0;
						}
					}
				break;

				// we don't process other types of events
				default:
					break;
			}

		}

		int tamanho;
		switch (modo)
		{
		case 0:
			tamanho=6;
			break;
		case 1:
			tamanho=9;
			break;
		case 2:
			tamanho=12;
			break;
		case  3:
			tamanho=16;
			break;
		case 4:
			tamanho=10;
			break;
		}
		lerArquivo(tamanho,media);
		desenharRank(window, media, 300, 100, tamanho);
	}
}

void telaFim(RenderWindow &window, Data &media, bool venceu)
{
	Font font;
	font.loadFromFile("font/sansation.ttf");

	string texto;
	if(venceu)
	{
		texto="Voc� Venceu!";
	}
	else
	{
		texto="Voc� Perdeu.";
	}

	Text conteudo(texto, font, 60);
	conteudo.setPosition(225, 250);
	conteudo.setColor(Color(80, 80, 80));
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
				break;

				// we don't process other types of events
				default:
					break;
			}

		}


		window.draw(media.imagens.fundo_jogo);
		window.draw(conteudo);

		window.display();

	}
}

void telaTamanho(RenderWindow &window, Data &media, int dificuldade, int tam)
{
	int coeficienteDificuldade=(3-dificuldade)*10;
	int tamanho=tam;
	int **m = new int*[20];
	m[0] = new int[20*20];

	for (int i=1; i<20; i++)
	{
		m[i] = m[i-1]+20;
	}

	bool **b = new bool*[20];
	b[0] = new bool[20*20];

	for (int i=1; i<20; i++)
	{
		b[i] = b[i-1]+20;
	}

	for(int i=0;i<20;i++)
	{
		for(int j=0;j<20;j++)
		{
			b[i][j]=true;
		}

	}

	bool venceu=false;
	bool diag=false;
	preencherMapa(m,tamanho, window, media, diag);
	adicionarDificuldade(m,b,tamanho,dificuldade);

	int erros=0;

	time_t tempo_inicial, tempo_atual, tempo_decorrido;

	time(&tempo_inicial);

	int indice = 0;
	Dicatype dica;
	dica.dica=rand()%10;
	dica.trocar=false;

	Entradas entrada;
	entrada.val=0;
	entrada.valor="";

	Font font;
	font.loadFromFile("font/sansation.ttf");

	int TAMANHOTILE, TAMANHOFONTE;

	if(tamanho==16)
	{
		TAMANHOTILE=30;
		TAMANHOFONTE=15;
	}
	else
	{
		TAMANHOTILE=40;
		TAMANHOFONTE=20;
	}

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
					delete []m[0];
					delete []b[0];
					window.close();
					break;

				case Event::KeyPressed:

					if( (Keyboard::isKeyPressed(Keyboard::Escape)) )
					{
						media.musicas.ravel.stop();
						delete []m[0];
						delete []b[0];
						telaMenu(window,media);
					}
					selecionarTile(window, tamanho,indice, m,b, media, erros, diag, entrada);
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
		desenharMapa(window,media.textos, m , b, tamanho, indice, tempo_decorrido, erros, coeficienteDificuldade, dica);

		entrada.tile.setCharacterSize(20);
		entrada.tile.setColor(Color(80, 80, 80));
		entrada.tile.setFont(font);
		entrada.tile.setString(entrada.valor);
		entrada.tile.setPosition(10,10);
		entrada.tile.setPosition((400-tamanho/2*TAMANHOTILE)+indice%tamanho*TAMANHOTILE+TAMANHOFONTE/2, (300-tamanho/2*TAMANHOTILE)+indice/tamanho*TAMANHOTILE+TAMANHOFONTE/2);

		window.draw(entrada.tile);
		window.display();

		if(erros>20)
		{
			telaFim(window, media,venceu);
		}
		if(testarVenceu(m, tamanho))
		{
			delete []m[0];
			delete []b[0];
			venceu=true;
			int tempoSomado = tempo_decorrido+(coeficienteDificuldade*erros)-((20-erros)*dificuldade);
			if(validarRank(tamanho,tempoSomado , media, diag))
			{
				telaRank(window, media);
			}
			else
			{
				telaFim(window, media,venceu);
			}
		}
	}
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
