#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define DELAY 1000000

#ifdef _WIN32
	#define clearScreen() 	system("cls")
	void CursorUp(int y)
	{
		printf("\e[%dF", y);
	}
#else
	// #define getch() 	getchar()
	#define getch() 	getchar()
	#define clearScreen() 	system("clear")
  	// Unix machine
	void CursorUp(int y)
	{
		printf("\e[%dF", y);
	}
#endif

#define max(a,b) 	a > b ? a : b
#define min(a,b) 	a < b ? a : b

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
struct Tiro
{
	int v;
	int theta;	
};

struct feedback 
{
	int 	c1x;
	int 	c1y;
	int 	c2x;	
	int 	c2y;
	int 	v_winner;
	int 	theta_winner;
	int 	distancia_projectil;
	char 	winner[15];
};


void mainmenu(void);
void posicaocanhao(char * nivel, int * canhao);

void fechar()
{
	exit(0);
}

void getTiro(int jogador, struct Tiro * tiro)
{

	printf("Jogador %d => Introduza os valores para as seguintes variaveis:\nVelocidade: ", jogador);
	scanf("%d",&(tiro->v));
	printf("Angulo: ");
	scanf("%d",&(tiro->theta));
}

void help()
{	
	int decision3;
	
	clearScreen();

	printf("No modo jogar, introduzir os valores da velocidade e do angulo. O objectivo e aniquilar o canhao adversario\n... escolha os valores com sabedoria.\n");
	printf("\nPrima qualquer tecla para continuar...\n");
	getch();
}



void posicaocanhao(char * nivel, int * canhao1)
{

	FILE *F1;
    
	F1 = fopen(nivel,"r");
	if(F1 == NULL)
	{
		printf("Erro ao abrir o ficheiro '%s': %d - %s\n", nivel, errno, strerror(errno));
		exit(90);
	}

	int i;

	for (i = 0; i < 4; i++)
	{
		// TODO: test canhao1[i}
		fscanf(F1, "%d", canhao1+i);
	}
    
	fclose( F1 );
}  
 

void calculaTrajectoria(int limite_x,int limite_y,int matriz[],int h, double theta, double v, int *xmaximo, int *ymaximo, double *speed)
{
	double radiano=((theta*(2*M_PI))/360);
	double g = 9.8;
	double tangente=(sin(radiano)/cos(radiano));
	
	double y;
	int x = 1;

	double vcos = g/(2*(v*cos(radiano))*(v*cos(radiano)));
	for(x = 1; y > 0 && y < limite_y && x < limite_x; x++)
	{
		y = h+((x*tangente)-(pow(x,2)*vcos));
		int Y =(int)round(y);
		if(Y > *ymaximo)
			*ymaximo=Y;
		
		if (Y > 0)
			matriz[x]=Y;
		//printf("DEBUG: X=%d Y=%f\n", x, y);
	}

	*xmaximo = x;

	for(; x < limite_x; x++)
		matriz[x]=0;

	*speed = (double)(DELAY/(v * cos(radiano)));
}

void primeironivel(int c1x,int c1y,int c2x,int c2y)
{

	clearScreen();
	int i;
	int j;
	int xmax;
	xmax = max(c1y,c2y);
	
	printf("\n\n\n                                                    NIVEL 1\n");
	
		for (i=xmax+10;i>0;i--)
		{
			// para cada coluna
			for (j=1; j <= c2x+10; j++)
			{
					//printf("i=%d; j=%d",i,j);
				if(j == c1x && i == c1y)
				{
					printf("1");
						
				}
				
				if(j == c2x && i == c2y)
				{
					printf("2");
				}			
	
				else 
				{
					printf(" ");
				}
			}
			printf("\n");
		}
		
		int l;
		for(l=0;l<c2x+10;l++)
		{
			printf("^");
	
		}
		
			printf("\n");
		
}


void segundonivel(int c1x,int c1y,int c2x,int c2y)
{

	clearScreen();
	int i;
	int j;
	int xmax;
	xmax = max(c1y,c2y);
	
		printf("\n\n\n                                         NIVEL 2\n");
	
		for (i=xmax+10;i>0;i--)
		{
			// para cada coluna
			for (j=1; j <= c2x+10; j++)
			{
					//printf("i=%d; j=%d",i,j);
				if(j == c1x && i == c1y)
				{
					printf("1");
						
				}
			
				if(j == c2x && i == c2y)
				{
					printf("2");
				}			
		
				else if(j==c1x+25 && i<=20)
				{
					printf("|");
				}
			
				else if(j == c1x+1 && i == c1y)
				{
					
				}
				else 
				{
					printf(" ");
				}
			}
			printf("\n");
		}
		
		int l;
		for(l=0;l<c2x+10;l++)
		{
			printf("^");
	
		}
		
			printf("\n");
		
}



void imprimeTrajectoria(int limite_x,int limite_y,int matriz[],int c1x,int c1y,int c2x, int c2y, int dir, int l, int h, double speed, int acertou,char *nivel,int acertouobstaculo)
{
	
char nivel1txt[]="nivel1.txt";
char nivel2txt[]="nivel2.txt";
int nivelatual;
 
 
if (strcmp(nivel,nivel1txt) == 0)
      nivelatual=1;

 
else if (strcmp(nivel,nivel2txt) == 0)
      nivelatual=2;

if(nivelatual==1)
	printf("\n\n\n                                         NIVEL 1\n\n\n");	
else if(nivelatual==2)
	printf("\n\n\n                                         NIVEL 2\n\n\n");	
	
	//Por colocar
	int xmaximo;
	int offset;
	
	xmaximo = max(c1x, c2x);
	if(dir == 1)
	{
		xmaximo = max(xmaximo, c1x+l);
		offset = c1x;
	}
	else
	{
		offset = c2x;
	}
	int ymaximo = h;
	ymaximo = max(c1y, ymaximo);
	ymaximo = max(c2y, ymaximo);

	int i;
	int j;
	int meux;

	//printf("xmaximo = %d ymaximo = %d speed = %f\n", xmaximo, ymaximo, speed);

	int xinit, xfin, xmin, xmax, last = 0;
	char rasto = ' ';
	
	if(dir == 1)
	{
		xinit = c1x;
		if(acertou)
		{
			xfin = c2x;
		}
		
		else if(acertouobstaculo)
		{
			xfin=c1x+25;
		}
		else
		{
			xfin = c1x + l;
		}
	}
	else
	{
		xinit = c2x;
		if(acertou)
		{
			xfin = c1x;
		}
		else if(acertouobstaculo)
		{
			xfin=c1x+25;
		}
		else
		{
			
			xfin = c2x - l;
		}
		xfin = xfin >= 0 ? xfin : 0;
	}

	xmin = min(xinit, xfin);
	xmax = max(xinit, xfin);

	//printf("Janela com projetil entre xmin: %d e xmax: %d (dir: %d, l: %d, xinit: %d, xfin: %d)\n", xmin, xmax, dir, l, xinit, xfin);
	// Ate ao ponto em que vai mostrar o projetil
	for (meux=xinit; !last ; meux+=dir)
	{
		int maxcol = meux;
		if((dir == -1 && meux > xmin) || (dir == 1 &&  meux < xmax))
		{
			// impressao do quadro (exceto ultima)
		}
		else
		{
			// ultima impressao do quadro
			rasto = '.';
			last = 1;
			maxcol = xmax;
		}
		// para cada linha
		for (i=(ymaximo<limite_y? ymaximo:limite_y);i>0;i--)
		{
			// para cada coluna
			for (j=1; j <= xmax; j++)
			{
				int posicao = (dir == 1 ? j - c1x+1 : c2x - j-1);
				if(j == c1x && i == c1y)
				{
					if(meux == xmin && posicao == 0 && posicao <= l)
					{
						printf("bang");
						j+=3;
					}
					else
					{
						printf("1");
					}
				}
				else if(j == c2x && i == c2y)
				{
					if(meux == xmax && posicao > 0 && posicao <= l)
					{
						printf("BANG");
						j+=3;
					}
					else
					{
						printf("2");
					}
				}	
				
				else if(nivelatual==2 && i <= 20 && j==c1x+25)
				{
						
						printf("|");
				}		
					
				else if(posicao > 0 && posicao < l && matriz[posicao] == i)
				{
					if(nivelatual==2 && dir==-1 && last==1 && acertouobstaculo!=0)
					
					{
					printf("%c", (j <= c1x+25 ? ' ' : rasto));
					}
		
					
					else
					{
					printf("%c", (j == meux ? 'o' : rasto));
					}
				}
		
				else 
				{
					printf(" ");
				}
			}

		if(nivelatual==1)
		{
		
			int maxx;
			maxx = meux;
			if(i == c1y && c1x > maxx)
				maxx = c1x;
			if(i == c2y && c2x > maxx)
				maxx = c2x;
			for(j=meux+1; j <= maxx; j++)
			{
				if(j == c1x && i == c1y)
					printf("1");
				else if(j == c2x && i == c2y)
					printf("2");
			}
		}
			
			printf(" \n");	
		}
		for(i=0;i<=xmaximo;i++)
		{
			printf("^");
		}
		//printf(" i: %d; j: %d; meux: %d; last: %d", i, j, meux, last);
		usleep(speed);
		if(!last)
		{
			// http://stackoverflow.com/questions/11474391/is-there-go-up-line-character-opposite-of-n
			CursorUp(ymaximo);
		}
	}
	
	printf("\n\n");	
}


int verificaAcertou(int canhao_x, int matriz[], int alvo_x, int alvo_y)
{
	// Calcular a distancia entre o canhao e o alvo
	/*
	if ()
	{
		
	}
	*/
	int distancia = abs(canhao_x - alvo_x);
	// Verificar se o projetil chega ao alvo na altura certa
	return(matriz[distancia] == alvo_y ? distancia : 0);
	
	
	
}

int Acertouobstaculo(int canhao_x, int matriz[], int alvo_x, int alvo_y)
{
	//Posição do obstaculo relativamente ao C1x
	int posicaoobstaculo=25;
	int alturaobstaculo=20;
	// Verificar se o projetil chega ao alvo na altura certa
	return((matriz[posicaoobstaculo] <= alturaobstaculo) ? posicaoobstaculo : 0);
}


void writefilefeedback(struct feedback Feedback)
{
	FILE *fp;

	fp = fopen("ficheirofeedback2.txt", "w+");

	fprintf(fp,"Coordenadas do canhão1\n");
	fprintf(fp,"x | %d\n",Feedback.c1x);
	fprintf(fp,"y | %d\n",Feedback.c1y);
	fprintf(fp,"Coordenadas do canhão2\n");
	fprintf(fp,"x | %d\n",Feedback.c2x);
	fprintf(fp,"y | %d\n",Feedback.c2y);
	fprintf(fp,"Distancia percorrida pelo projectil (eixo dos xx): %d\n",Feedback.distancia_projectil);
	fprintf(fp,"winner | %s\n",Feedback.winner);

	fclose(fp);
}

void calculaEscreveFeedback(int c1x,int c1y,int c2x, int c2y, int l, double theta, double v, char * winner)
{
	struct feedback fb;
	 
	fb.c1x = c1x;
	fb.c1y = c1y;
	fb.c2x = c2x;
	fb.c2y = c2y;

	//fb.winner = winner; Não funciona porque precisamos de copiar a string para a estrutura.
	snprintf(fb.winner, 15, "%s", winner);
	fb.v_winner = v;
	fb.theta_winner = theta;
	
	fb.distancia_projectil = l;

	writefilefeedback(fb);
}




void apresentacao()
{

	char matriz[252];
	
	FILE *F3;
    
	F3 = fopen("Menu.txt","r");
	
	int i;
	int j;

for(j = 0; j < 252; j++)
{

		fscanf(F3, "%c", &matriz[j]);
}
	
char ponto[]=".";

int m=0;

for(i = 1; i < 8; i++)
{
	for (j = 1; j < 37; j++)
	{
		/*	
		if(((i==2 || i==3 || i==5 || i==6) && j>=2 && j<=35)||(i==4 && (j==2 ||j==3 || j==35)))
		{
		printf("3");
		}
		
		else 
		{
		*/
		printf("%c", matriz[m]);		
		m++;
		//}
	
		// TODO: test canhao1[i}
	

	}
	printf("\n");
	
}


	fclose( F3 );	

 	sleep(3);
	clearScreen();
	sleep(1);

}


 void apresentacao2()
{

	char matriz[252];
	
	FILE *F3;
    
	F3 = fopen("Menu2.txt","r");
	
	int i;
	int j;

for(j = 0; j < 252; j++)
{

		fscanf(F3, "%c", &matriz[j]);
}
	
char ponto[]=".";

int m=0;

for(i = 1; i < 8; i++)
{
	for (j = 1; j < 37; j++)
	{
		/*	
		if(((i==2 || i==3 || i==5 || i==6) && j>=2 && j<=35)||(i==4 && (j==2 ||j==3 || j==35)))
		{
		printf("3");
		}
		
		else 
		{
		*/
		printf("%c", matriz[m]);		
		m++;
		//}
	
		// TODO: test canhao1[i}
	

	}
	printf("\n");
	
}


	fclose( F3 );	

 	sleep(3);
	clearScreen();
	sleep(1);

}


 


int correrJogo(struct Tiro *t1, struct Tiro *t2,char *nivel)
{
	clearScreen();
	
		char nivel1txt[]="nivel1.txt";
char nivel2txt[]="nivel2.txt";
int nivelatual;
 
 
if (strcmp(nivel,nivel1txt) == 0)
      nivelatual=1;

 
else if (strcmp(nivel,nivel2txt) == 0)
      nivelatual=2;


	int limite_x=120;
	int limite_y=50; // Pode ir até MAX_INT

	int matriz[limite_x+1];

	int canhao[4];

	posicaocanhao(nivel, canhao);

	int c1x = canhao[0];
	if(c1x > limite_x || c1x < 1)
	{
		//printf("ERROR: Posicao X para o canhao 1 (%d) acima do limite (%d) ou inferior a 1\n", c1x, limite_x);
	}
	int c1y = canhao[1];
	int c2x = canhao[2];
	int c2y = canhao[3];
	int acertou = 0;
	int acertouobs = 0;

	int xmaximo = 0, ymaximo = 0, x;
	double speed = 0;
	
	// Se ha tiro do canhao 1
	if(t1)
	{
		// calcular trajetoria (matriz comeca na posicao x do canhão que dispara)
		calculaTrajectoria(limite_x, limite_y, matriz, c1y, t1->theta, t1->v, &xmaximo, &ymaximo, &speed);
		// verifica se o tiro foi certeiro
		acertou = verificaAcertou(c1x, matriz, c2x, c2y);
		if(nivelatual==2)
		{
			acertouobs = Acertouobstaculo(c1x, matriz, c2x, c2y);
			acertou==acertouobs;
		}
		
		
		//printf("A limpar matrix de %d a %d \n", acertou, limite_x);
		// limpar trajetoria depois do alvo
		if(acertou)
			for(x = acertou; x < limite_x; x++)
				matriz[x]=0;

		// Mostra disparo ao jogador
		imprimeTrajectoria(limite_x, limite_y, matriz, c1x, c1y, c2x, c2y, 1, xmaximo, ymaximo, speed, acertou,nivel,acertouobs);
	}

	acertou = 0;
	acertouobs = 0;
	// Se o jogador 1 acertou
	if(acertou)
	{
		calculaEscreveFeedback(c1x, c1y, c2x, c2y, xmaximo, t1->theta, t1->v, "Jogador 1");
	}
	// Se nao acertou e ha tiro do canhao 2
	
	else if(t2)
	{
		calculaTrajectoria(limite_x, limite_y, matriz, c2y, t2->theta, t2->v, &xmaximo, &ymaximo, &speed);
		
		acertou = verificaAcertou(c2x, matriz, c1x, c1y);
		
		if(nivelatual==2)
		{
			acertouobs = Acertouobstaculo(c1x, matriz, c2x, c2y);
		
			acertou==acertouobs;
		}
		
		if(acertou)
			for(x = acertou; x < limite_x; x++)
				matriz[x]=0;
				
		imprimeTrajectoria(limite_x, limite_y, matriz, c1x, c1y, c2x, c2y, -1, xmaximo, ymaximo, speed, acertou,nivel,acertouobs);

		// Se o jogador 2 acertou
		if(acertou)
		{
			calculaEscreveFeedback(c1x, c1y, c2x, c2y, xmaximo, t2->theta, t2->v, "Jogador 2");
		}
	}
}

int jogar()
{	

	char nivel1[12]="nivel1.txt";
	char nivel2[12]="nivel2.txt";
	char nivel3[12]="nivel3.txt";
	
	int decision;
	
	clearScreen();

	printf("1 | Player vs Computer\n");
	printf("2 | Player vs Player\n");
	printf("3 | Voltar\n");
	//scanf("%d",&decision2);
	
	do
	{
		decision = getch() - '0';
	} 
	while(decision < 1 || decision > 3);

	struct Tiro 	t1, t2;
	t1.v = 4;
	t1.theta = 5;	
	int restart;
	do
	{
		switch(decision)
		{
			case 1:
				niveis(decision,nivel1,nivel2);
			break;
			case 2:
				//jogar2player();
				niveis(decision,nivel1,nivel2);
			break;
			case 3:
				mainmenu();
			break;
		}	
	}
	while(restart == 1);
	
}


int niveis(int decision,char*nivel_1,char*nivel_2)
{	

	int limite_x=120;
	int limite_y=100; // Pode ir até MAX_INT
	
	int canhao1[4];

	posicaocanhao(nivel_1, canhao1);

	int c1x1 = canhao1[0];
	int c1y1 = canhao1[1];
	int c2x1 = canhao1[2];
	int c2y1 = canhao1[3];
	
	int canhao2[4];

	posicaocanhao(nivel_2, canhao2);

	int c1x2 = canhao2[0];
	int c1y2 = canhao2[1];
	int c2x2 = canhao2[2];
	int c2y2 = canhao2[3];

	char nivel1[12]="nivel1.txt";
	char nivel2[12]="nivel2.txt";
	char nivel3[12]="nivel3.txt";
	
	int decision2;
	
	clearScreen();

	printf("1 | Nivel 1\n");
	printf("2 | Nivel 2\n");
	printf("3 | Voltar\n");
	//scanf("%d",&decision2);
	
	do
	{
		decision2 = getch() - '0';
	} 
	while(decision2 < 1 || decision2 > 3);

	struct Tiro 	t1, t2;
	t1.v = 4;
	t1.theta = 5;	
	int restart;
	do
	{
		switch(decision2)
		{
			case 1:
				if(decision==1)
				{
				primeironivel(c1x1,c1y1,c2x1,c2y1);
				getTiro(1, &t1);
				correrJogo(&t1, (struct Tiro  *)NULL,nivel1);	
				}
				else if(decision==2)
				{
				primeironivel(c1x1,c1y1,c2x1,c2y1);
				getTiro(1, &t1);
				getTiro(2, &t2);
				correrJogo(&t1, &t2,nivel1);	
				}
			break;
			case 2:
				
				if(decision==1)
				{
				segundonivel(c1x2,c1y2,c2x2,c2y2);
				getTiro(1, &t1);
				correrJogo(&t1, (struct Tiro  *)NULL,nivel2);	
				}
				else if(decision==2)
				{
				segundonivel(c1x2,c1y2,c2x2,c2y2);
				getTiro(1, &t1);
				getTiro(2, &t2);
				correrJogo(&t1, &t2,nivel2);	
				}
				
			break;
			case 3:
				jogar();
			break;
		}	
		// while(getch());
		printf ("Quer jogar outra vez? 1. SIM 2.NAO\n");
		// scanf ("%d",&restart);
		do
		{
			restart = getch() - '0';
		}
		while(restart < 1 || restart > 2);
		//printf ("Selecionado: %d\n", restart);
		//getch();
	}
	while(restart == 1);
}


void mainmenu(void)
{
	int decision1;
	
	do
	{
		clearScreen();

		printf("1 | Jogar\n");	
		printf("2 | Help\n");
		printf("3 | Quit\n");
		//scanf("%d",&decision1);
		do
		{
			decision1=getch() - '0';
		}while(decision1 < 1 || decision1 > 3);
	
		if(decision1==1)
		{
			jogar();
		}
		else if(decision1==2)
		{
			help();
		}
	}while(decision1 != 3);
}




int main()
{
	apresentacao();
	apresentacao2();
	mainmenu();

	return 0;
}
