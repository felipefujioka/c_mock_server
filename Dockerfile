FROM gcc:4.9

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp

RUN make

ENTRYPOINT ["./mock_server", "4200"]
