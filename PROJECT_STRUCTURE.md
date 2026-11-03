
- parcing cli argument
    - totale argument is 8 the first 7 valid integer and the number of coder must be greater than 0
    - last argument must be fifo or edf

- initialize table
    table is a struct that represent the table and other simulation status it hold:
     - list of coders and dongles
     - all arguments that i parse them from cli
     - two mutex for logs and state
     - monitor thread
