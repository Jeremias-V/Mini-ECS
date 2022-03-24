#!/bin/bash

deactivate
source env/bin/activate
sudo ./e1.out &
sudo ./e2.out &
sudo ./a.out &
python3 ecs-client.py
