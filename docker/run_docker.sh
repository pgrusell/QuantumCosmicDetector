#!/bin/bash

docker build . -t QDdocker
docker run -it -v "$(pwd)/..:/app/shared" QDdocker
