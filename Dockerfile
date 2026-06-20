FROM ubuntu:22.04

# Avoid prompts during apt installations
ENV DEBIAN_FRONTEND=noninteractive

# Install g++, ttyd, vim (the in-app editor) and ncurses-bin (for `clear`)
RUN apt-get update && apt-get install -y \
    build-essential \
    ttyd \
    vim \
    ncurses-bin \
    && rm -rf /var/lib/apt/lists/*

# Default the in-app editor to vim
ENV EDITOR=vim

# Set working directory
WORKDIR /app

# Copy the source code
COPY . .

# Compile the hyperbole code
RUN g++ hyperbole.cpp -o hyperbole

# Expose port 8080 for web access
EXPOSE 8080

# Run ttyd serving the interactive CLI
# -W flag allows clients to write to the terminal (interactive)
ENTRYPOINT ["ttyd", "-W", "-p", "8080", "./hyperbole"]
