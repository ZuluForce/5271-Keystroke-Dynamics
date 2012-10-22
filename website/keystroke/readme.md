# Keystroke Dynamics Data Collection

### Data Collected
* For All (k1, k2): Fly Times k1 –> k2
* For all (k): Dwell Times

### Sample Apache + PHP Configuration
```
<VirtualHost *:80>
    DocumentRoot "/Sites/keystroke/public"
    ServerName keystroke.org
    ServerAlias test.keystroke.org

    php_value include_path "/Sites/keystroke"
</VirtualHost>
```
