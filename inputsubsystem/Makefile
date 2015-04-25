all: key_input key_output mouse_input mouse_output
	
clean: 
	rm -f key_input key_output mouse_input mouse_output

key_input: key_input.c
	gcc $< -o $@
mouse_input: mouse_input.c
	gcc $< -o $@
key_output: key_output.c
	gcc $< -o $@
mouse_output: mouse_output.c
	gcc $< -o $@
