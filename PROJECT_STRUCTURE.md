
- parcing cli argument
    - totale argument is 8 the first 7 valid integer and the number of coder must be greater than 0
    - last argument must be fifo or edf

- initialize table
    table is a struct that represent the table and other simulation status it hold:
     - list of coders and dongles
     - all arguments that i parse them from cli
     - two mutex for logs and state
     - monitor thread


return value of must functions that i implement is 'int'
    - when function failed in specific point it return 1
    - 0 in succus

- int_table
    - allocate space for cli argument, if aloocation failed return 1
    - init mutex for logs and state
    - create flag for check if the mutex initialized or no
    - any error happened it return 1 and the main function cleanup and exit with error status




