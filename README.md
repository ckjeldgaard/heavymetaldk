[![Build Status](https://travis-ci.org/ckjeldgaard/heavymetaldk.svg?branch=master)](https://travis-ci.org/ckjeldgaard/heavymetaldk)

# Heavymetal.dk

> The Danish [Heavymetal.dk](http://heavymetal.dk) website.

## Docker instructions

Create a `.env` file in the root directory with the following contents:

```
PROJECT_NAME=heavymetaldk

DB_NAME=[SELECTED_DB_NAME]
DB_USER=[SELECTED_DB_USER]
DB_PASSWORD=[SELECTED_DB_PASSWORD]
DB_ROOT_PASSWORD=[SELECTED_DB_ROOT_PASSWORD]
```

``` bash
# Run webserver and database on localhost:80
docker-compose up

# Stop webserver and database
docker-compose stop

# Run webserver and database and force rebuild
docker-compose up -d --build
```

When the web server is running, [phpMyAdmin](https://www.phpmyadmin.net/) will be available from `http://localhost:8080`.

## Docker container terminal

``` bash
docker exec -u dev -it heavymetaldk_server bash
```

## Importing data to MySQL database

Given a backup SQL file named `backup.sql`, execute the following command to import data to the MySQL database running in the Docker image `heavymetaldk_db_1`:

``` bash
cat backup.sql | docker exec -i heavymetaldk_db_1 /usr/bin/mysql -u root --password=docker db_heavymetaldk
```

If this is the first installation, create a new MySQL user named `hmdk` with access to the `db_heavymetaldk` database:

``` bash
CREATE USER 'hmdk'@'%' IDENTIFIED WITH mysql_native_password AS '[STRONG_MYSQL_PASSWORD]'; GRANT USAGE ON *.* TO 'hmdk'@'%' REQUIRE NONE WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0 MAX_USER_CONNECTIONS 0; GRANT ALL PRIVILEGES ON `db_heavymetaldk`.* TO 'hmdk'@'%';
```

Afterwards, copy the `www/sites/default/default.settings.php` to a new file named `www/sites/default/settings.php` and insert the database access options as described in [this page from the offical Drupal 7 documentation](https://www.drupal.org/docs/7/install/step-3-create-settingsphp-and-the-files-directory).

## Deployment

A [Gulp](https://gulpjs.com/) task called `deploy` will upload all www-files changed since last Git commit through FTP. To run the task, run the following command:

``` bash
gulp deploy --user '[FTP_USERNAME]' --password '[FTP_PASSWORD]' --commitrange [COMMIT_RANGE]
```

The [`$TRAVIS_COMMIT_RANGE`](https://docs.travis-ci.com/user/environment-variables/#Default-Environment-Variables) environment variable is used as `[COMMIT_RANGE]` in Travis CI builds.
