CC = gcc
CFLAGS = -Iprotocols
LDFLAGS = -lwayland-client -lm

SRC = $(wildcard src/*.c) protocols/ext-session-lock.c
OBJ = ${SRC:.c=.o}
OUTDIR = bin/
OUT = epilock

all: build

build: ${OBJ}
	mkdir -p ${OUTDIR}
	${CC} $^ -o ${OUTDIR}${OUT} ${LDFLAGS}

run: build
	PATH=$(shell pwd)/${OUTDIR}:${PATH} && Hyprland --config hyprlandd.conf

local: build
	${OUTDIR}${OUT}

.PHONY: clean

clean:
	${RM} -r ${OUTDIR}
	${RM} ${OBJ}
	${RM} ${DEP}
