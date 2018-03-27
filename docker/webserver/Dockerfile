FROM php:7-apache

RUN apt-get update -y && apt-get install -y libpng-dev curl libcurl4-openssl-dev libmagickwand-dev libfreetype6-dev libjpeg62-turbo-dev

# Install Imagemagick & PHP Imagick ext
RUN pecl install imagick && docker-php-ext-enable imagick

# Install GD
RUN docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/

RUN docker-php-ext-install pdo pdo_mysql gd curl

RUN a2enmod rewrite expires
RUN service apache2 restart
