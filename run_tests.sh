#!/usr/bin/env bash

gcc -std=c99 -Wall -Wpedantic -Wextra tests.c
./a.out
rm -rf a.out
