#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io/pad.h>

#define MAX_SNAKE_LENGTH 300
#define NUM_FOODS 40
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef struct {
    float x;
    float y;
} Point2D;

typedef struct {
    Point2D body[MAX_SNAKE_LENGTH];
    int length;
    float radius;
    float speed;
    float angle;
} SnakeGame;

typedef struct {
    Point2D pos;
    int active;
} SnakeFood;

// Variáveis globais do jogo
SnakeGame player;
SnakeFood foods[NUM_FOODS];

// Inicializa ou reinicia o estado do Slither.io
void init_slither_game() {
    player.length = 15;
    player.radius = 12.0f;
    player.speed = 3.5f;
    player.angle = 0.0f;
    
    // Posiciona a cabeça e o corpo no centro da tela
    for(int i = 0; i < player.length; i++) {
        player.body[i].x = SCREEN_WIDTH / 2.0f;
        player.body[i].y = SCREEN_HEIGHT / 2.0f + (i * 8);
    }

    // Distribui as comidas pelo mapa de forma aleatória
    for(int i = 0; i < NUM_FOODS; i++) {
        foods[i].pos.x = (float)((rand() % (SCREEN_WIDTH - 100)) + 50);
        foods[i].pos.y = (float)((rand() % (SCREEN_HEIGHT - 100)) + 50);
        foods[i].active = 1;
    }
}

// Atualiza a lógica de movimento e colisões
void update_slither_game(padData pad_input) {
    // Captura os valores do analógico esquerdo do controle do PS3
    // O padrão do SDK do PS3 varia de 0 a 255 (128 é o centro neutro)
    float stick_x = (float)pad_input.ANA_L_X - 128.0f;
    float stick_y = (float)pad_input.ANA_L_Y - 128.0f;

    // Se o analógico for movido para fora da zona morta (mínimo de 15 de força)
    if (fabs(stick_x) > 15.0f || fabs(stick_y) > 15.0f) {
        player.angle = atan2f(stick_y, stick_x);
    }

    // Faz cada segmento do corpo seguir o segmento anterior
    for (int i = player.length - 1; i > 0; i--) {
        player.body[i] = player.body[i - 1];
    }

    // Move a cabeça na direção do ângulo atual
    player.body[0].x += cosf(player.angle) * player.speed;
    player.body[0].y += sinf(player.angle) * player.speed;

    // Mantém a cobra dentro dos limites da tela (Teleporte para o lado oposto)
    if (player.body[0].x < 0) player.body[0].x = SCREEN_WIDTH;
    if (player.body[0].x > SCREEN_WIDTH) player.body[0].x = 0;
    if (player.body[0].y < 0) player.body[0].y = SCREEN_HEIGHT;
    if (player.body[0].y > SCREEN_HEIGHT) player.body[0].y = 0;

    // Verifica colisão com as esferas de comida
    for (int i = 0; i < NUM_FOODS; i++) {
        if (foods[i].active) {
            float dx = player.body[0].x - foods[i].pos.x;
            float dy = player.body[0].y - foods[i].pos.y;
            float distance = sqrtf(dx * dx + dy * dy);

            // Se a distância for menor que o tamanho da cabeça + comida, ela come
            if (distance < (player.radius + 6.0f)) {
                foods[i].active = 0;
                
                // Aumenta o tamanho se não tiver atingido o máximo permitido
                if (player.length < MAX_SNAKE_LENGTH) {
                    player.length++;
                }
                
                // Faz a comida reaparecer em outra coordenada do mapa
                foods[i].pos.x = (float)((rand() % (SCREEN_WIDTH - 100)) + 50);
                foods[i].pos.y = (float)((rand() % (SCREEN_HEIGHT - 100)) + 50);
                foods[i].active = 1;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    padInfo pad_info;
    padData pad_data;
    
    // Inicializa o sistema de controles do PS3 (suporta até 7 controles)
    ioPadInit(7);
    init_slither_game();

    // Loop infinito padrão de jogos
    while(1) {
        ioPadGetInfo(&pad_info);
        
        // Verifica se o controle 0 está conectado e ativo
        if(pad_info.status & PAD_STATUS_CONNECTED) {
            ioPadGetData(0, &pad_data);
            
            // Se o botão START for pressionado, o jogo reinicia na hora
            if(pad_data.BTN_START) {
                init_slither_game();
            }
            
            update_slither_game(pad_data);
        }
    }

    // Finaliza o sistema de inputs ao sair (boa prática)
    ioPadEnd();
    return 0;
}
