# Nome do arquivo PKG final que vai aparecer no seu PS3
TARGET		:= slitherio_offline

# ID do Jogo (Garante que apareça na aba de jogos/XMB)
TITLE_ID	:= SLITH0001
VENDOR		:= OFFLINE
TITLE		:= Slither.io Offline PS3
APP_VERSION	:= 01.00

# Caminho das ferramentas dentro do servidor do GitHub
PS3DEV		?= /usr/local/ps3dev
include $(PS3DEV)/ps3dev.mk

# Informa ao compilador para ler o arquivo main.c
OBJS = main.o

CFLAGS	+= -O2 -Wall
LDFLAGS	+= 

include $(PS3DEV)/ppu.mk
