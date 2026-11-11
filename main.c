#include <ppu-lv2.h>
#include <sysutil/video.h>
#include <rsx/rsx.h>
#include <stdio.h>

int main() {
    printf("Slither PS3 Teste OK\n");
    // espera 5 segundos so pra nao fechar na hora
    for(volatile long i=0; i<500000000; i++);
    return 0;
}