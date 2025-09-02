#!/bin/bash

gcc -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L unity.c -o test
