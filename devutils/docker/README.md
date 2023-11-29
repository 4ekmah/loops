# Docker file for remote development of loops. 

This docker file can be used for remote development via ssh, e.g. VScode.<br>
Configured for build and debug.<br>
Currently, most important case is development on Linux + Aarch64, when there is only Mac machine.<br>

## To create and run docker image:<br>
```
sudo docker build -t debloops .
sudo docker run -d -p 2022:22 debloops
```
## Ssh connection line:<br>
```
ssh devloops@<machine_ip> -p 2022
```
Password is "change_this_password". Same for root.<br>
**NOTE!** It's important to change passwords for root and devloops users.<br>

## To stop and remove docker image:<br>
```
sudo docker ps
```
## Look name and id of running container.<br>
```
sudo docker stop <container_name>
sudo docker rm <container_id>
sudo docker image rm debloops
```