echo "brokenuser:x:999999999:999999999:Bad User:/nonexistent:/bin/false" | sudo tee -a /etc/passwd
getent passwd brokenuser
