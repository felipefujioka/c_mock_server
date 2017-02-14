mockserver:
	gcc -o mock_server main.c

.PHONY: clean
clean:
	rm mock_server
