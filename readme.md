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


*USE THIS TO RUN THE PROGRAM*
gcc driver.c LinkedList/list.c socket_communications.c thread_functions.c -lpthread -o test.out 


pthread_example:
gcc -o pthread_example pthread_example.c -lpthread


gcc -o receive_test socket_testing/socket_receive_test.c socket_communications.c

gcc -o send_test socket_testing/socket_send_test.c socket_communications.c

gcc -o s-talk driver.c socket_communications.c thread_functions.c LinkedList/list.c -lpthread
