#!/bin/bash

rm *.out
gcc -o ecs.out elastic-container-service.c
gcc -o ecs-agent.out ecs-agent.c
