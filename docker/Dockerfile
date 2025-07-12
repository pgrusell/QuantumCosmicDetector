FROM ubuntu:22.04

# Dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential cmake git wget tar \
    libxerces-c-dev libexpat1-dev \
    libglu1-mesa-dev libx11-dev libxmu-dev libxi-dev \
    qtbase5-dev libqt5opengl5-dev \
    && rm -rf /var/lib/apt/lists/*


# g4 installation
WORKDIR /opt

RUN git clone https://github.com/Geant4/geant4.git

RUN mkdir /opt/build && cd /opt/build \
    && cmake ../geant4 \
    -DCMAKE_INSTALL_PREFIX=/usr/local/geant4 \
    -DGEANT4_BUILD_MULTITHREADED=ON \
    -DGEANT4_USE_OPENGL_X11=ON \
    -DGEANT4_USE_QT=OFF \
    -DGEANT4_USE_G3TOG4=ON \
    -DGEANT4_USE_USOLIDS=OFF \
    -DGEANT4_INSTALL_DATA=ON \
    -DGEANT4_INSTALL_DATADIR=/usr/local/geant4/data \
    -DGEANT4_BUILD_EXAMPLES=ON \
    -DGEANT4_INSTALL_EXAMPLES=ON \
    -DGEANT4_USE_SYSTEM_EXPAT=ON \
    -DGEANT4_USE_SYSTEM_XERCES=ON


RUN cd /opt/build \
    && make -j$(nproc)\
    && make install

# env variables
ENV G4INSTALL=/usr/local/geant4
ENV G4DATA=/usr/local/geant4/data
ENV PATH=${G4INSTALL}/bin:${PATH}
ENV LD_LIBRARY_PATH=${G4INSTALL}/lib:${LD_LIBRARY_PATH}
RUN echo "source /usr/local/geant4/bin/geant4.sh" >> /etc/bash.bashrc

# run the macro
ENTRYPOINT ["bash"]