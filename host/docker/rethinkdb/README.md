# Rethinkdb

RethinkDB is the database used by the domos project.

It provides a simple REST API to read and write into the DB.

It also provides, through Horizon, a realtime synchronisation facility.

At the time of this writing, there is no rethinkdb binary package for ARM. So the
porpose of this folder is to provide this package and a way to easily reproduce it.

# Content

In this folder you will find:
* A Dockerfile to create a container allowing you to compile rethinkdb for the raspberry.
* A binary debian package for ARM ready to be installed on a raspbery.
