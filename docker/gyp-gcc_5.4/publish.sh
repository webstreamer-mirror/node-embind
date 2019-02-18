imageID=$1
tag=$2

sudo docker login
docker tag $imageID webstreamer/gyp-gcc54:$tag
docker push webstreamer/gyp-gcc54:$tag