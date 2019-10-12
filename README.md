# docker-tutorial

Make with docker
----------------

Directory `sphinx-docker` contains Dockerfile describing sphinx toolchain embedded in docker image.

Build it by typing:
```
cd sphinx-docker
docker build . -t sphinx:0.1
```

When the image is ready you can build the tutorial by typing in the main directory:

```
make html SPHINXBUILD="docker run --mount type=bind,source=${PWD},target=/sphinx-workdir sphinx:0.1 build"
```

Run inside docker
-----------------
The `Dockerfile` in the main directory creates image with compiled tutorial.
Just type:
```
docker build . -t docker-tutorial:0.1
docker run -p 8080:80 -d docker-tutorial:0.1
```
