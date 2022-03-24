#!/bin/bash

rm *.out
gcc elastic-container-service.c
gcc -o e1.out ecs-agent1.c
gcc -o e2.out ecs-agent2.c