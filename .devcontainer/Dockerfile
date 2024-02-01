# Use the official Ubuntu 20.04 image as a base
FROM ubuntu:20.04

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Update the apt package list
RUN apt-get update --fix-missing

# Install build-essential, cmake, libpcap-dev
RUN apt-get install -y git build-essential cmake libpcap-dev

# Install python3, python3-venv and pipx
RUN apt-get install -y python3 python3-venv python3-pip
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install pipx
RUN python3 -m pipx ensurepath

# Install poetry via pipx
RUN pipx install poetry

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=dialog
