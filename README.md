[![Build Status](https://travis-ci.org/ckjeldgaard/heavymetaldk.svg?branch=master)](https://travis-ci.org/ckjeldgaard/heavymetaldk)

# Heavymetal.dk

> The Danish [Heavymetal.dk](http://heavymetal.dk) website.

## Docker instructions

``` bash
# Run webserver and database on localhost:80
docker-compose up

# Stop webserver and database
docker-compose stop

# Run webserver and database and force rebuild
docker-compose up -d --build
```

## Deployment

A [Gulp](https://gulpjs.com/) task called `deploy` will upload all www-files changed since last Git commit through FTP. To run the task, run the following command:

``` bash
gulp deploy --user '[FTP_USERNAME]' --password '[FTP_PASSWORD]'
```
