FROM ubuntu:22.04 as build

WORKDIR /src 
COPY CMakeLists.txt /src
COPY simulation /src/simulation
COPY include /src/include 
RUN apt-get update && \
    apt-get install -y \
    build-essential cmake && \
    apt-get clean

WORKDIR /src/build 
RUN cmake .. && make

# Second stage for deployment, without any build dependencies or source files
FROM ubuntu:22.04 as run 

WORKDIR /app
COPY --from=build /src/build/hpc_disease_simulation /app
COPY --from=build /src/build/test /app

# install required runtime dependancies
# ---

# Run the tests just to be sure (not required if tests are part of a 
# gitlab CICD pipeline
RUN /app/test

# Change to a working directory
WORKDIR /scratch

# Execute the simulation on image start
ENTRYPOINT ["/app/hpc_disease_simulation"]