socket_listen.c:

    Run using gcc socket_list.c && ./a.out
    Open another terminal and use the command: netcat -u 127.0.0.1 22110
    to be able to communicate
    Any message send from the netcat terminal will appear on the terminal from where the program was run.
    
    The program can also increment a number and return it to the netcat terminal as well.
    To see how it works: 
        comment out line 49
        Uncomment lines: 53, 57,59,60,64-72
        Rerun the program and enter a numerical value into the netcat terminal
