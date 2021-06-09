#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

enum { NORTE, SUL, LESTE, OESTE };

struct Carrinho {
  int lin, col, dir, cor;
};

char ** criamatriz( int lin, int col ) {
  char ** res = malloc( lin * sizeof( char * ) );  // Vertical
  int i;
  for(i = 0; i < lin; i++)
    res[i] = malloc( col * sizeof( char ) );  // Cada linha
  return res;
}

void liberamatriz( char ** m, int lin ) {
  int i;
  for(i = 0; i < lin; i++)
    free( m[i] );
  free (m);
}

void printmatriz( char ** m, int lin, int col ) {
  int l, c;
  for(l = 0; l < lin; l++) {
    for(c = 0; c < col; c++)
      printf("%c", m[l][c] );
    printf("\n");
  }
}

void preenchematriz( char ** m, int lin, int col ) {
  int l,c;
  for(l = 0; l < lin; l++)
    for(c = 0; c < col; c++) {
      m[l][c] = ' ';
      int aux = rand() % 20;
      if ( aux == 0 ) m[l][c] = '\\';
      if ( aux == 1 ) m[l][c] = '/';
    }
  for(l = 0; l < lin; l++)
    m[l][0] = m[l][col - 1] = '|';
  for(c = 0; c < col; c++)
    m[0][c] = m[lin - 1][c] = '-';
}

// carro  ----------------->   [ dir = ... lin = ... col = .... cor = ... ]

void movecarrinho( struct Carrinho * carro, int lin, int col ) {
  switch ( carro -> dir ) {
    case NORTE: carro->lin--; break;
    case SUL:   carro->lin++; break;
    case LESTE: carro->col++; break;
    case OESTE: carro->col--; break;
  }
  if ( carro -> lin == lin - 1 ) carro -> lin = 1;
  if ( carro -> col == col - 1 ) carro -> col = 1;
  if ( carro -> lin == 0 )   carro -> lin = lin - 2;
  if ( carro -> col == 0 )   carro -> col = col - 2;
}

#define COLOR  "\033[47;%dm"
#define NORMAL "\033[0m"

void acertadirecao( char **mat, struct Carrinho * carro ) {
  int l = carro -> lin;
  int c = carro -> col;
  switch ( mat[ l ][ c ] ) {
    case ' ' : return;
    case '-' :
      if ( carro -> dir == NORTE ) carro -> dir = SUL;
                             else  carro -> dir = NORTE;
      return;
    case '|':
      if ( carro -> dir == LESTE ) carro -> dir = OESTE;
                             else  carro -> dir = LESTE;
      return;
    case '/':
      mat[l][c] = '\\';
      printf( COLOR "\033[%d;%dH%c" NORMAL, carro->cor, 1+l, 1+c, mat[l][c]);
      if ( carro -> dir == NORTE ) { carro -> dir = LESTE; return; }
      if ( carro -> dir == SUL )   { carro -> dir = OESTE; return; }
      if ( carro -> dir == LESTE ) { carro -> dir = NORTE; return; }
      if ( carro -> dir == OESTE ) { carro -> dir = SUL; return; }
    case '\\':
      mat[l][c] = '/';
      printf( COLOR "\033[%d;%dH%c" NORMAL, carro->cor, 1+l, 1+c, mat[l][c]);
      if ( carro -> dir == NORTE ) { carro -> dir = OESTE; return; }
      if ( carro -> dir == SUL )   { carro -> dir = LESTE; return; }
      if ( carro -> dir == LESTE ) { carro -> dir = SUL; return; }
      if ( carro -> dir == OESTE ) { carro -> dir = NORTE; return; }
  }
}

#define MAX 10

int main( void ) {
  
  srand( time( NULL ) );
	
  int lin, col;
  printf("Linhas e colunas: " );
  scanf("%d %d", &lin, &col );
  
  char **mat = criamatriz( lin, col );
  preenchematriz( mat, lin, col );
  
  struct Carrinho car[MAX];

  int i;
  for( i = 0; i < MAX; i++ ) {
    car[i].lin = 1 + rand() % lin;
    car[i].col = 1 + rand() % col;
    car[i].dir = NORTE;
    car[i].cor = 101 + i % 7;
  }
  
  printf("\033[2J\033[H\033[?25l");
  printmatriz( mat, lin, col );
  
  while (1) {
    for( i = 0; i < MAX; i++ ) {
      movecarrinho( &car[i], lin, col );
      printf(COLOR "\033[%d;%dH%c" NORMAL, car[i].cor, 1+car[i].lin, 1+car[i].col, mat[car[i].lin][car[i].col]);
      acertadirecao( mat, &car[i] );
    }
    fflush( stdout );
    usleep( 80000 );
  }
}