mockserver:
	gcc -o mock_server main.c

string_converter:
	gcc -o char_array_to_string_literal char_array_to_string_literal.c

.PHONY: clean
clean:
	rm mock_server
	rm char_array_to_string_literal
