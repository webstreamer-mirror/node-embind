imageID=$1
tag=$2

sudo docker login
docker tag $imageID webstreamer/node-embind-gcc54:$tag
docker push webstreamer/node-embind-gcc54:$tag