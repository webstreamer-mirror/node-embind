FROM webstreamer/gyp-gcc54

LABEL maintainer="Mingyi Zhang <mingyi.z@outlooke.com>"

ENV NVM_DIR /opt/nvm  
ENV NODE_VERSION 10.15.1  

RUN wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.34.0/install.sh | bash \
    && source $NVM_DIR/nvm.sh \
    && nvm install v$NODE_VERSION \
    && nvm use v$NODE_VERSION \
    && nvm alias default v$NODE_VERSION
	
ENV NODE_PATH $NVM_DIR/v$NODE_VERSION/lib/node_modules  
ENV PATH      $NVM_DIR/v$NODE_VERSION/bin:$PATH
	
