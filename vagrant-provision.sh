#!/bin/bash

# Set teminal mode as noninteractive
echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

# Install docker

# Remove old installations
sudo apt-get remove docker docker-engine docker.io containerd runc
sudo apt update
sudo apt-get install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

# Add Docker repository
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

# Install from repo
sudo apt-get update
sudo apt-get install -y docker-ce docker-ce-cli containerd.io
sudo groupadd docker
sudo usermod -aG docker vagrant
