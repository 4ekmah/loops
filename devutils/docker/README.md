This docker file can be used for remote development via ssh.

To create and run docker image: 
sudo docker build -t debloops .
sudo docker run -d -p 2022:22 debloops
ssh devloops@localhost -p 2022

To stop and remove docker image: 
sudo docker ps 
sudo docker stop zen_morse
sudo docker rm 492c2f4ed1de
sudo docker image rm debloops
