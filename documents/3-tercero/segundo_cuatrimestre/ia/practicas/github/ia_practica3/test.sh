#!/bin/bash

# jugador 1
tipo=AI
nombre=jesus
nivel=11

# jugador 2
tipoRival=Ninja
nombreRival=maquina

# opciones generales
gui=--no-gui

# tests
./ParchisGame --p1 $tipo $nivel $nombre --p2 $tipoRival 1 $nombreRival $gui
./ParchisGame --p1 $tipoRival 1 $nombreRival $gui --p2 $tipo $nivel $nombre
./ParchisGame --p1 $tipo $nivel $nombre --p2 $tipoRival 2 $nombreRival $gui
./ParchisGame --p1 $tipoRival 2 $nombreRival $gui --p2 $tipo $nivel $nombre
./ParchisGame --p1 $tipo $nivel $nombre --p2 $tipoRival 3 $nombreRival $gui
./ParchisGame --p1 $tipoRival 3 $nombreRival $gui --p2 $tipo $nivel $nombre