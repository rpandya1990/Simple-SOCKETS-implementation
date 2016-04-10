Simple Socket Implementation:

Implemented 3 basic functions: GET, BOUNCE and EXIT, examples as below:

    Client input: GET Test.txt  Server output: The content of Test.txt
    Clinet input BOUNCE Hello World Server output: Hello World
    For Exit:

          The difference between EXIT and EXIT code is:
          For EXIT command, you should print out the default value/message.
          For EXIT <code> command, you should print out the "code".
          For example, for EXIT CHROME_ERRORCODE_6.2.2.6613", your server end should print out the message "CHROME_ERRORCODE_6.2.2.6613".
          For EXIT, your server end should print out a message like "Normal_Exit". The output may differ, depends on your default exit code.
 
Note: You can apply any sign to replace "<>",  just make your server can be able to identify client input is OK.


Example:
1. Compile both server1.c and client1.c:   gcc server1.c -o server,  gcc client1.c -o client
2. Start server: ./server 9999   (Argument- PORT)
3. Start client: ./client localhost 9999  (Argument- IP,PORT)
4. Execute commands