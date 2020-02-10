FROM php:7-apache

# OpCache
RUN docker-php-ext-install opcache && docker-php-ext-enable opcache

RUN apt-get update -y && apt-get install -y libpng-dev curl libcurl4-openssl-dev libmagickwand-dev libfreetype6-dev libjpeg62-turbo-dev

# Install Imagemagick & PHP Imagick ext
RUN pecl install imagick && docker-php-ext-enable imagick

# Install GD
RUN docker-php-ext-install gd
RUN docker-php-ext-configure gd --with-freetype=/usr/include/ --with-jpeg=/usr/include/

RUN docker-php-ext-install pdo pdo_mysql gd curl

# Composer
RUN curl -sSk https://getcomposer.org/installer | php -- --disable-tls && \
       mv composer.phar /usr/local/bin/composer

# Install PECL Uploadprogress
RUN pecl install uploadprogress \
  && echo 'extension=uploadprogress.so' > /usr/local/etc/php/conf.d/uploadprogress.ini

# Create dev user
RUN groupadd dev -g 999
RUN useradd dev -g dev -d /var/www -m
RUN chown -R dev:dev /var/www

# mod_rewrite for URL rewrite and mod_headers for .htaccess extra headers like Access-Control-Allow-Origin-
RUN a2enmod rewrite expires headers
RUN service apache2 restart
