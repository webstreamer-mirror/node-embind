FROM webstreamer/gyp-gcc54

LABEL maintainer="Mingyi Zhang <mingyi.z@outlooke.com>"


RUN wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.34.0/install.sh | bash \
    && echo 'export NVM_DIR="$HOME/.nvm"' >> "$HOME/.bashrc" \
	&& echo '[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm' >> "$HOME/.bashrc" \
	&& echo '[ -s "$NVM_DIR/bash_completion" ] && . "$NVM_DIR/bash_completion" # This loads nvm bash_completion' >> "$HOME/.bashrc" \
	&& bash -c 'source $HOME/.nvm/nvm.sh   && \
    nvm install node 10                    && \
    npm install -g doctoc urchin eclint dockerfile_lint && \
    npm install --prefix "$HOME/.nvm/"'
