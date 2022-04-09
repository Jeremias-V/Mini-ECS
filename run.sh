#!/bin/bash

deactivate 2> /dev/null
source env/bin/activate
sudo ./ecs-agent.out &
sleep 0.3
sudo ./ecs.out &
sleep 0.3
python3 ecs-client.py
