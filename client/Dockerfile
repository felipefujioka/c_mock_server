FROM dgarros/tcpreplay:latest

RUN apt-get update && \
    apt-get -y install \
            netcat \
            tshark

COPY . .

CMD bash
