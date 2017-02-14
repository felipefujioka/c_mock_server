#!/bin/bash

docker stop c_server_container
docker rm c_server_container

docker run --rm --name c_server_container -it c_mock_image