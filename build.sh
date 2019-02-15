
sudo -E docker run --rm -v "$PWD:/home/conan/project" /bin/bash -c " cd project &&  npm install && node build.js && npm test"

