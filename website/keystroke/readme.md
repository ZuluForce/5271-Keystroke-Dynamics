# Minnesota Web Development
## mnwd-website

This is the repository for hacking on the MNWD website, usually located at http://mnwd.org.

To get up and running, simply drop this in a live docroot and go.  The site is entirely static at the moment.

### We are looking for contributions related to:
* Resources page
* Basic styles (no actual branding or themeing)
* Fixing of trivial errors
* Other cool things that you might develop and send to us in a pull request

### Sammple Apache config
```
<VirtualHost *:80>
    DocumentRoot "/Sites/mnwd/public"
    ServerName mnwd.org
</VirtualHost>
```

### Sample NGiNX config
```
server {
    listen       *:80;
    server_name  mnwd.org www.mnwd.org;

    location / {
        root   /www/mnwd.org/public;
        index  index.html index.htm;
    }
}
```
