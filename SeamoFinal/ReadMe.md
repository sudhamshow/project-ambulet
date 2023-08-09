This directory contains 4 folders analyse, logs, the receiver code and the sender code.
The receiver folder contains the SeaMo receiver code (working directory) that is run on a cloud compute platform and pdr and loss computing application for a system behind the receiver(Now run on the same cloud system(loopback address)).

The sender code connects to **ALL** the available interfaces (broadcast and PTP) and sends the packets on them simultaneously. There is an application that creates empty packets to stuff in information (Client-Ambulet). Run-rules.sh has to be run before starting the app to configure policy based routes.

The analyse folder contains scripts to check and improve the algorithm. There is sender code which takes in interpacket delays(computed by the receiver log file), interface information and packet number from three different files and emulate the previous activity on the SeaMo receiver to see if the algorithm can be improved.

Logs contains delay, packet loss logs recorded by different Receiver Log files for future algorithm improvement(Upon repeating tests with the above said method).
