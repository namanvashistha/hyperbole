FROM ubuntu:22.04

# Avoid prompts during apt installations
ENV DEBIAN_FRONTEND=noninteractive

# Install g++ and ttyd
RUN apt-get update && apt-get install -y \
    build-essential \
    ttyd \
    && rm -rf /var/lib/apt/lists/*

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
