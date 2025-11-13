#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 10
#define SKILL_N 7      
#define WATER 0
#define SHIP 3
#define AFFECTED 5
#define SHIP_AFFECTED 8

// Cria matriz inicializada com zeros
void init_matrix(int mat[SKILL_N][SKILL_N]) {
    for (int i = 0; i < SKILL_N; ++i)
        for (int j = 0; j < SKILL_N; ++j)
            mat[i][j] = 0;
}

// Constrói um "cone" apontando para baixo.
void build_cone(int mat[SKILL_N][SKILL_N]) {
    int center = SKILL_N / 2;
    init_matrix(mat);
    for (int r = 0; r < SKILL_N; ++r) {
        for (int c = 0; c < SKILL_N; ++c) {
            if ( (c >= center - r) && (c <= center + r) ) {
            
                mat[r][c] = 1;
            } else {
                mat[r][c] = 0;
            }
        }
    }
}

// Constrói uma "cruz" com ponto de origem no centro.
void build_cross(int mat[SKILL_N][SKILL_N]) {
    int center = SKILL_N / 2;
    init_matrix(mat);
    for (int i = 0; i < SKILL_N; ++i) {
        for (int j = 0; j < SKILL_N; ++j) {
            if (i == center || j == center) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
}

// Constrói um "octaedro" visto de frente (losango).

void build_octahedron(int mat[SKILL_N][SKILL_N]) {
    int center = SKILL_N / 2;
    int radius = SKILL_N / 2; // por exemplo, para 7x7 radius=3
    init_matrix(mat);
    for (int i = 0; i < SKILL_N; ++i) {
        for (int j = 0; j < SKILL_N; ++j) {
            int manhattan = (i > center ? i - center : center - i) + (j > center ? j - center : center - j);
            if (manhattan <= radius) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
}

/* Sobrepõe a matriz de habilidade ao tabuleiro, centralizando a matriz na linha/col origem.
 Se a matriz de habilidade tem 1 e estiver dentro do tabuleiro, marca como AFFECTED.
Se já houver um navio (SHIP), marcamos SHIP_AFFECTED para mostrar que foi atingido/afetado.
Atenção aos limites do tabuleiro.*/
void apply_skill_to_board(int board[BOARD_SIZE][BOARD_SIZE],
                          int skill[SKILL_N][SKILL_N],
                          int origin_row, int origin_col) {
    int center = SKILL_N / 2;
    for (int i = 0; i < SKILL_N; ++i) {
        for (int j = 0; j < SKILL_N; ++j) {
            if (skill[i][j] == 1) {
                int board_r = origin_row + (i - center);
                int board_c = origin_col + (j - center);
                // Verifica limites do tabuleiro
                if (board_r >= 0 && board_r < BOARD_SIZE && board_c >= 0 && board_c < BOARD_SIZE) {
                    if (board[board_r][board_c] == SHIP) {
                        board[board_r][board_c] = SHIP_AFFECTED; // navio também afetado
                    } else if (board[board_r][board_c] == SHIP_AFFECTED) {
                        // já marcado como navio atacado; nada a fazer
                    } else {
                        board[board_r][board_c] = AFFECTED;
                    }
                }
            }
        }
    }
}

// Mostra o tabuleiro no console com caracteres legíveis.
// . = água, S = navio, * = área afetada, X = navio afetado
void print_board(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("  ");
    for (int c = 0; c < BOARD_SIZE; ++c) printf(" %d", c);
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int v = board[i][j];
            char ch;
            if (v == WATER) ch = '.';
            else if (v == SHIP) ch = 'S';
            else if (v == AFFECTED) ch = '*';
            else if (v == SHIP_AFFECTED) ch = 'X';
            else ch = '?';
            printf(" %c", ch);
        }
        printf("\n");
    }
}

int main() {
    // Cria tabuleiro e inicializa com água
    int board[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = WATER;

    // Exemplo: posiciona alguns navios (valor 3) manualmente no tabuleiro.
    // (Simplificação do requisito: posições fixas no código.)
    board[2][2] = SHIP;
    board[2][3] = SHIP;
    board[2][4] = SHIP;
    board[4][5] = SHIP;
    board[7][7] = SHIP;
    board[6][7] = SHIP;

    // Matrizes de habilidade (7x7)
    int cone[SKILL_N][SKILL_N];
    int cross[SKILL_N][SKILL_N];
    int octa[SKILL_N][SKILL_N];

    // Construção dinâmica (usando condicionais e loops aninhados)
    build_cone(cone);
    build_cross(cross);
    build_octahedron(octa);

    // Define pontos de origem (lin, col) no tabuleiro para cada habilidade
    // Esses pontos são o "centro" da sobreposição.
    int origin_cone_r = 1, origin_cone_c = 4;   // ex.: cone iniciado próximo ao topo
    int origin_cross_r = 5, origin_cross_c = 2; // ex.: cruz no centro-esquerda
    int origin_octa_r = 7, origin_octa_c = 7;   // ex.: octaedro canto-inferior

    // Aplica as habilidades ao tabuleiro (com verificação de limites)
    apply_skill_to_board(board, cone, origin_cone_r, origin_cone_c);
    apply_skill_to_board(board, cross, origin_cross_r, origin_cross_c);
    apply_skill_to_board(board, octa, origin_octa_r, origin_octa_c);

    // Mostra as matrizes de habilidade (opcional) e o tabuleiro final:
    printf("Matriz - Cone (1 = afetado, 0 = nao afetado):\n");
    for (int i = 0; i < SKILL_N; ++i) {
        for (int j = 0; j < SKILL_N; ++j) printf("%d ", cone[i][j]);
        printf("\n");
    }
    printf("\nMatriz - Cruz:\n");
    for (int i = 0; i < SKILL_N; ++i) {
        for (int j = 0; j < SKILL_N; ++j) printf("%d ", cross[i][j]);
        printf("\n");
    }
    printf("\nMatriz - Octaedro (losango):\n");
    for (int i = 0; i < SKILL_N; ++i) {
        for (int j = 0; j < SKILL_N; ++j) printf("%d ", octa[i][j]);
        printf("\n");
    }

    printf("\nTabuleiro final ( . = agua, S = navio, * = area afetada, X = navio afetado ):\n");
    print_board(board);

    return 0;
}